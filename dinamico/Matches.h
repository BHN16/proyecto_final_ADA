#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "Image.h"
#include "Dynamic.h"
#include "Greedy.h"
#include "DynamicAverage.h"

using namespace std;
typedef vector<pair<int,int>> Tuples;
typedef pair<double,Tuples> Pesos;
typedef vector<Pesos> OptimalMatches;
typedef vector<Tuples> Blocks;



class Matches {
private:
    Image A_image;
    Image B_image;
    Dynamic* dynamic_solution;
    Greedy greedy_solution;
    DynamicAverage dynamicAverage_solution;
public:
    Matches(const string& imageA, const string& imageB, string method) {//el input de main.cpp es 2 strings con los nombres del archivo y una opcion para escoger el metodo de transformacion
        this->A_image.readFile(imageA);
        this->B_image.readFile(imageB);
        this->dynamic_solution = nullptr;
        if(method == "1") {
            this->dynamicSolution();
        } else if(method == "2") {
            this->greedySolution();
        } else {
            this->dynamicAverageSolution();
        }
    }

    void dynamicSolution() {
        this->dynamic_solution = new Dynamic();
        for(int i = 0; i < A_image.matrix.size(); i++) {
            auto response = this->dynamic_solution->dynamicSolution(A_image.weights[i].size()-1, B_image.weights[i].size()-1,&A_image.blocks[i], &B_image.blocks[i] ,&A_image.weights[i], &B_image.weights[i], &A_image.accumulated_weights[i], &B_image.accumulated_weights[i]);
            cout << response << endl;
        }
        for(int i = 0; i < A_image.matrix.size(); i++) {
            this->transformationProcess((*dynamic_solution->matches)[i],this->A_image.blocks[i], this->B_image.blocks[i]);
        }
    }

    void greedySolution() {
        cout << "voraz\n";
    }

    void dynamicAverageSolution() {
        cout << "dinamico mejorado\n";
    }


    void transformationProcess(Helper& mapo, Tuples& A, Tuples& B) {
        cout << "A: " << A;
        cout << "B: " << B;
        int midCase = 20;// future constant for multiple mid matrices
        vector<Tuples *> pasos;
        for(auto it = mapo.rbegin(); it != mapo.rend(); ++it) {
            cout << it->second << endl;
        }
        for(int i=0;i<midCase;i++){
            pasos.push_back(new Tuples);
        }

        for(auto it = mapo.rbegin(); it != mapo.rend(); ++it) {
            vector<float> weights;
            float temp_weightA = 0;
            float temp_weightB = 0;
            if(it->second.size() == 1) {

                temp_weightA = A[it->second[0].first].second - A[it->second[0].first].first + 1;
                temp_weightB = B[it->second[0].second].second-B[it->second[0].second].first + 1;

                int temp_tam;
                for(int i=0; i<midCase; i++){
                    int des_temp;
                    if(A[it->second[0].first].first > B[it->second[0].second].first){
                        des_temp = A[it->second[0].first].first - (A[it->second[0].first].first - B[it->second[0].second].first)*float((i+1))/(midCase+1);
                    }
                    else{
                        des_temp = A[it->second[0].first].first + (B[it->second[0].second].first - A[it->second[0].first].first)*float((i+1))/(midCase+1);

                    }
                    if(temp_weightA>temp_weightB){
                        temp_tam = temp_weightA - temp_weightB;
                        pasos[i]->push_back(make_pair(des_temp,des_temp-((temp_tam)*(float((i+1))/(midCase+1)))));
                    }
                    else{
                        temp_tam = temp_weightB - temp_weightA;
                        pasos[i]->push_back(make_pair(des_temp,des_temp+((temp_tam)*(float((i+1))/(midCase+1)))));
                    }
                }

            } else if(it->second[0].first == it->second[1].first) { // DIVISON
                temp_weightA = A[it->second[0].first].second - A[it->second[0].first].first + 1;
                for(int i = 0; i < it->second.size(); i++) {
                    int temp = (B[it->second[i].second].second - B[it->second[i].second].first) + 1;
                    weights.push_back(temp);
                    temp_weightB += temp;
                }

                vector<int> FraccionAcumulada;
                vector<int> PosicionesFinales;
                vector<int> Fracciones;
                Fracciones.push_back(0);
                FraccionAcumulada.push_back(0);
                PosicionesFinales.push_back(A[it->second[0].first].first);

                for(int i=1;i<weights.size();i++){
                    FraccionAcumulada.push_back(FraccionAcumulada[FraccionAcumulada.size() - 1] + (temp_weightA / temp_weightB) * weights[i - 1]);
                    Fracciones.push_back((temp_weightA / temp_weightB) * weights[i - 1]);
                    PosicionesFinales.push_back(A[it->second[0].first].first+FraccionAcumulada[i]);
                }
                for(int i = 0; i < midCase; i++) {
                    for(int j=0;j<weights.size();j++){
                        int tamp_temp = (((weights[j]-Fracciones[j])*float((i+1))/(midCase+1)));
                        int desp_temp;
                        if(A[it->second[j].first].first > B[it->second[0].second].first){
                            desp_temp = PosicionesFinales[j] - (B[it->second[j].second].first - PosicionesFinales[j])*float((i+1))/(midCase+1)  ;
                        }
                        else{
                            desp_temp = PosicionesFinales[j] + (B[it->second[j].second].first  - PosicionesFinales[j])*float((i+1))/(midCase+1);
                        }
                        if(weights.size() != (j-1)){
                            pasos[i]->push_back(make_pair(desp_temp,desp_temp+tamp_temp));
                        }
                        else{
                            pasos[i]->push_back(make_pair(desp_temp,B[it->second[j].second].second));
                        }
                    }
                }
            } else {
                temp_weightB = (B[it->second[0].second].second-B[it->second[0].second].first)+1;
                for(int i = 0; i < it->second.size(); i++) { // AGRUPAMIENTO
                    int temp = (A[it->second[i].first].second-A[it->second[i].first].first)+1;
                    weights.push_back(temp);
                    temp_weightA += temp;
                }
                vector<int> FraccionAcumulada;
                vector<int> PosicionesFinales;
                vector<int> Fracciones;
                Fracciones.push_back(0);
                FraccionAcumulada.push_back(0);
                PosicionesFinales.push_back(B[it->second[0].second].first);
                //Suma si A es menor que B fraccionesa acumuladas
                //Restar si A es mayor que B fracciones acumuladas
                for(int i=1;i<weights.size();i++){
                    FraccionAcumulada.push_back(FraccionAcumulada[FraccionAcumulada.size() - 1] + (temp_weightB / temp_weightA) * weights[i - 1]);
                    Fracciones.push_back((temp_weightB / temp_weightA) * weights[i - 1]);
                    PosicionesFinales.push_back(B[it->second[0].second].first+FraccionAcumulada[i]);
                }
                if(temp_weightA > temp_weightB) {
                    for(int i = 0; i < midCase; i++) {
                        for(int j=0;j<weights.size();j++){
                            int tamp_temp = weights[j] - (((weights[j]-Fracciones[j])*float((i+1))/(midCase+1)));
                            int desp_temp;
                            if(A[it->second[j].first].first > B[it->second[0].second].first){
                                desp_temp = A[it->second[j].first].first - (A[it->second[j].first].first - PosicionesFinales[j])*float((i+1))/(midCase+1);
                            }
                            else{
                                desp_temp = A[it->second[j].first].first + (PosicionesFinales[j] - A[it->second[j].first].first)*float((i+1))/(midCase+1);
                            }
                            if(weights.size() != (j-1)){
                                pasos[i]->push_back(make_pair(desp_temp,desp_temp+tamp_temp));
                            }
                            else{
                                pasos[i]->push_back(make_pair(desp_temp,B[it->second[0].second].second));
                            }
                        }
                    }
                }
                else{
                    for(int i = 0; i < midCase; i++) {
                        for(int j=0;j<weights.size();j++){
                            int tamp_temp = weights[j] + (((weights[j]-Fracciones[j])*float((i+1))/(midCase+1)));
                            int desp_temp;
                            if(A[it->second[j].first].first > B[it->second[0].second].first){
                                desp_temp = A[it->second[j].first].first - (A[it->second[j].first].first - PosicionesFinales[j])*float((i+1))/(midCase+1);
                            }
                            else{
                                desp_temp = A[it->second[j].first].first + (PosicionesFinales[j] - A[it->second[j].first].first)*float((i+1))/(midCase+1);
                            }
                            if(weights.size() != (j-1)){
                                pasos[i]->push_back(make_pair(desp_temp,desp_temp+tamp_temp));
                            }
                            else{
                                pasos[i]->push_back(make_pair(desp_temp,B[it->second[0].second].second));
                            }
                        }
                    }
                }
            }
        }
        int i=1;
        for (auto val : pasos){
            cout<<"VECTOR NUMERO "<<i<<endl;
            for(auto par : *val){
                cout<<"("<<par.first<<";"<<par.second<<") - ";
            }
            cout<<"\n-----------"<<endl;
            i++;
        }
    }
};
