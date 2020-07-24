#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

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
    int posicion = 0;
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
            cout<<"Estoy en la iteracion "<<i<<endl;
            this->transformationProcess((*dynamic_solution->matches)[i],this->A_image.blocks[i], this->B_image.blocks[i], A_image.matrix[0].size());
        }
    }

    void greedySolution() {
        cout << "voraz\n";
    }

    void dynamicAverageSolution() {
        cout << "dinamico mejorado\n";
    }


    void transformationProcess(Helper& mapo, Tuples& A, Tuples& B, int midVectorSize) {
        int midCase =  20;// future constant for multiple mid matrices
        vector<vector<int>> midVectors(midCase);
        midVectors.clear();
        cout<<"ESTOY EN LA FILA "<<posicion<<endl;
        posicion++;
        for (int i = 0; i < midCase; i++) {
            vector<int> temp(midVectorSize, 0);
            midVectors[i] = temp;
        }
        vector<Tuples *> pasos;
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
                cout<<"ESTOY ACA AGARRANDO UNO CON UNO" <<temp_weightA<<endl;
                int temp_tam;

                for(int i=0; i<midCase; i++){
                    float des_temp;
                    if(A[it->second[0].first].first > B[it->second[0].second].first){
                        des_temp = A[it->second[0].first].first - (A[it->second[0].first].first - B[it->second[0].second].first)*float((i+1))/(midCase+1);
                    }
                    else{
                        des_temp = A[it->second[0].first].first + (B[it->second[0].second].first - A[it->second[0].first].first)*float((i+1))/(midCase+1);

                    }
                    if(temp_weightA>temp_weightB){
                        temp_tam = temp_weightA - temp_weightB;
                        pasos[i]->push_back(make_pair(des_temp,des_temp+((temp_tam)*(float((i+1))/(midCase+1)))));
                    }
                    else{
                        temp_tam = temp_weightB - temp_weightA;
                        pasos[i]->push_back(make_pair(des_temp,des_temp+((temp_tam)*(float((i+1))/(midCase+1)))));
                    }
                }

            } else if(it->second[0].first == it->second[1].first) { // DIVISON
                cout<<"ENTRO EN DIVISION"<<endl;
                temp_weightA = A[it->second[0].first].second - A[it->second[0].first].first + 1;
                for(int i = 0; i < it->second.size(); i++) {
                    int temp = (B[it->second[i].second].second - B[it->second[i].second].first) + 1;
                    weights.push_back(temp);
                    temp_weightB += temp;
                }
                //cout<<temp_weightA<< " + "<<temp_weightB<<endl;

                vector<float> Fracciones;
                vector<float> PosicionesIniciales(weights.size());
                float matrizMidtams[midCase][weights.size()];

                for(int i=0; i<weights.size();i++){
                    if(i==0){
                        PosicionesIniciales[i] = A[it->second[0].first].first;
                    }
                    else{
                        PosicionesIniciales[i] = PosicionesIniciales[i-1] + float((weights[i]*(temp_weightA)) / float(temp_weightB));
                    }
                    Fracciones.push_back(((weights[i] - float((weights[i]*(temp_weightA) )/ float(temp_weightB)))/(midCase+1)));
                }
                /*for(int i=0 ; i<weights.size();i++){
                    cout<<"("<<A[it->second[i].first].first<<";"<<A[it->second[i].first].second<<") - ("<<B[it->second[i].second].first<<";"<<B[it->second[i].second].second<<")"<<endl;


                }*/
                for(int i=0; i<midCase; i++){
                    for(int j=0;j<weights.size();j++){
                        if(i==0){
                            matrizMidtams[i][j] =   float((weights[j]*(temp_weightA)) / float(temp_weightB)) ;
                        }
                        else{
                            matrizMidtams[i][j] = matrizMidtams[i-1][j]+Fracciones[j];
                        }
                    }
                }
/*
                for(int i=0; i<midCase; i++){
                    for(int j=0;j<weights.size();j++){

                        cout<<matrizMidtams[i][j]<< " - ";

                    }
                    cout<<endl;
                }

                for(auto val : PosicionesIniciales){
                    cout<<val<< " ; ";
                }
*/
                for(int i=0; i<midCase; i++ ){
                    for(int j=0; j<weights.size();j++){
                        if(PosicionesIniciales[j] < B[it->second[j].second].first){
                            float temp = PosicionesIniciales[j] + float(B[it->second[j].second].first - PosicionesIniciales[j])*(i+1)/(midCase+1);
                            pasos[i]->push_back(make_pair(temp,temp+matrizMidtams[i][j]));
                            //cout<<temp<<" 0000000000 "<<temp+matrizMidtams[i][j]<<endl;

                            //cout<<"( "<<temp<<" - "<<temp+matrizMidtams[i][j]<<" )"<<endl;
                        }
                        else{
                            float temp = PosicionesIniciales[j] - float(PosicionesIniciales[j] - B[it->second[j].second].first )*(i+1)/(midCase+1);
                            pasos[i]->push_back(make_pair(temp,temp+matrizMidtams[i][j]));
                            //cout<<"( "<<temp<<" - "<<temp+matrizMidtams[i][j]<<" )"<<endl;
                            //cout<<temp<<" 0000000000 "<<temp+matrizMidtams[i][j]<<endl;

                        }
                    }
                }

            } else {

                cout<<"ENTRO EN AGRUPAACIOOON"<<endl;
                temp_weightB = (B[it->second[0].second].second-B[it->second[0].second].first)+1;
                for(int i = 0; i < it->second.size(); i++) { // AGRUPAMIENTO
                    int temp = (A[it->second[i].first].second-A[it->second[i].first].first)+1;
                    weights.push_back(temp);
                    temp_weightA += temp;
                }
                //cout<<temp_weightA<<";;;;;;;;;"<<temp_weightB<<endl;
                vector<float> Fracciones;
                vector<float> PosicionesIniciales(weights.size());
                float matrizMidtams[midCase][weights.size()];

                for(int i=0; i<weights.size();i++){
                    if(i==0){
                        PosicionesIniciales[i] = B[it->second[0].second].first;
                        //cout<<"0009999    "<<PosicionesIniciales[0]<<endl;
                    }
                    else{
                        PosicionesIniciales[i] = PosicionesIniciales[i-1] + (float(weights[i-1]*(temp_weightB)) / float(temp_weightA));
                    }

                    Fracciones.push_back((float((weights[i]*(temp_weightB) / float(temp_weightA)) - weights[i])/(midCase+1)));

                }

               /* for(auto val : PosicionesIniciales){
                    cout<<val<< " ; ";
                }
                cout<<endl;
*/
                for(int i=0; i<midCase; i++){
                    for(int j=0;j<weights.size();j++){
                        if(i==0){
                            matrizMidtams[i][j] =  weights[j]  ;
                        }
                        else{
                            matrizMidtams[i][j] = matrizMidtams[i-1][j]+Fracciones[j];
                        }
                    }
                }
/*
                for(int i=0; i<midCase; i++){
                    for(int j=0;j<weights.size();j++){
                        cout<<matrizMidtams[i][j]<< " - ";
                    }
                    cout<<endl;
                }
*/

                for(int i = 0; i < midCase; i++) {
                    //newtuple.push_back(make_pair(12,23));
                    for(int j=0;j<weights.size();j++){
                        if(A[it->second[j].first].first > PosicionesIniciales[j]){
                            float temp = A[it->second[j].first].first - (float(A[it->second[j].first].first - PosicionesIniciales[j]))*(i+1)/(midCase+1);
                            pasos[i]->push_back(make_pair(temp,temp+matrizMidtams[i][j]));
                            //cout<<temp<<" 0000000000 "<<temp+matrizMidtams[i][j]<<endl;

                        }
                        else {
                            float temp = A[it->second[j].first].first +
                                         float( PosicionesIniciales[j] - A[it->second[j].first].first) * (i + 1) /
                                         (midCase + 1);
                            pasos[i]->push_back(make_pair(temp, temp + matrizMidtams[i][j]));
                        }

                    }
                }
            }
        }

        int j=0;
        for (auto val : pasos){
            cout<<"VECTOR NUMERO "<<j<<endl;

            for(auto par : *val){

                cout<<"("<<par.first<<";"<<par.second<<") - ";
                cout<<"Este se murio el vector"<<endl;
                vector<int> temp(par.second-par.first+1,1);
                cout<<"Este se murio el copy"<<endl;

                copy(temp.begin(), temp.end(), midVectors[j].begin()+par.first);
            }
            cout<<"\n-----------"<<endl;
            j++;
        }
        cout<<"Este es el j -> "<<j<<endl;
		/*
        for(int i = 0; i < midVectors.size(); i++) {
            for(int j = 0; j < midVectors[i].size(); j++) {
                cout << midVectors[i][j] << " ";
            }
            cout << "\n";
        }


        cout<<"antes de l for"<<endl;
        */
        for (int k = 0; k < midCase; k++) {
            fstream fs;

            string nombre = "TEST/"+ to_string(k) + "_dinamico.txt";

            fs.open(nombre, ios::out | ios::app);
            for(const auto&e : midVectors[k]) fs << e << " ";
            fs << "\n";
            fs.close();
        }
        cout<<"despues del for"<<endl;


    }
};

