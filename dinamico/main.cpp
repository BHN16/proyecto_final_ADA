#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include "Matches.h"
using namespace std;
typedef vector<pair<int,int>> Tuples;
typedef pair<double,Tuples> Pesos;
typedef vector<Pesos> OptimalMatches;
typedef vector<Tuples> Blocks;
typedef map<int, Tuples> Helper;

Pesos ** M = nullptr;
Helper mapo;
Tuples A = {};
Tuples B = {};
double mu = 0;
vector<double> weight_A;
vector<double> weight_B;
vector<double> accumulated_weights_A;
vector<double> accumulated_weights_B;

vector<vector<int>> matrix1;
vector<vector<int>> matrix2;
OptimalMatches matches;
Blocks blocks_A;
Blocks blocks_B;

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//                                                                                                                   //
//FUNCIONES QUE FUERON MEJORADAS O YA NO SON NECESARIAS                                                              //
//                                                                                                                   //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

/*
void dynamicHelper(int i, int j, Pesos& value) {
    Tuples init = {M[i-1][j-1].second.begin(), M[i-1][j-1].second.end()};
    init.push_back(make_pair(i,j));
    init.push_back(make_pair(i-1, j-1));
    value = make_pair(M[i-1][j-1].first+getWeight({make_pair(i,j)}), init);
    for(int k = i-1; k > 0; k--) {
        Tuples aux;
        double weight = generateTuplesA(aux, k, i, j);
        aux.push_back(make_pair(k-1, j-1));
        Pesos compare = make_pair(weight + M[k-1][j-1].first, aux);
        if(compare.first<value.first) {
            value.second.clear();
            value = compare;
        } else {
            compare.second.clear();
        }
    }
    for(int k = j-1; k > 0; k--) {
        Tuples aux;
        double weight = generateTuplesB(aux, i, k, j);
        aux.push_back(make_pair(i-1, k-1));
        Pesos compare = make_pair(weight+M[i-1][k-1].first, aux);
        if(compare.first<value.first) {
            value.second.clear();
            value = compare;
        } else {
            compare.second.clear();
        }
    }
}
*/
/*
double getWeight(Tuples calculate) {
    double weight = 0;
    float current_a = weight_A[calculate[0].first];
    float current_b = weight_B[calculate[0].second];
    if(calculate.size()==1) { return current_a/current_b; }
    for(int i = 0; i < calculate.size()-1; i++) {
        if(i == calculate.size()-2) {
            if(calculate[i].first == calculate[i+1].first) {
                current_b += weight_B[calculate[i+1].second];
                weight += current_a/current_b;
            } else if(calculate[i].second == calculate[i+1].second) {
                current_a += weight_A[calculate[i+1].first];
                weight += current_a/current_b;
            } else {
                weight += current_a/current_b;
                weight += weight_A[calculate[i+1].first]/weight_B[calculate[i+1].second];
            }
        } else if(calculate[i].second == calculate[i+1].second) {
            current_a += weight_A[calculate[i+1].first];
        } else if(calculate[i].first == calculate[i+1].first) {
            current_b += weight_B[calculate[i+1].second];
        } else {
            weight += current_a/current_b;
            current_a = weight_A[calculate[i+1].first];
            current_b = weight_B[calculate[i+1].second];
        }
    }
    return weight;
}
*/
/*
void baseCase() {
    Tuples init = {make_pair(0,0)};
    M[0][0] = make_pair(getWeight(init), init);
    for(int i = 1; i < weight_A.size(); i++) {
        Tuples temp;
        temp.insert(temp.end(), M[i-1][0].second.begin(), M[i-1][0].second.end());
        temp.push_back(make_pair(i,0));
        M[i][0] = make_pair(getWeight(temp), temp);
    }
    for(int j = 1; j < weight_B.size(); j++) {
        Tuples temp;
        temp.insert(temp.end(), M[0][j-1].second.begin(), M[0][j-1].second.end());
        temp.push_back(make_pair(0,j));
        M[0][j] = make_pair(getWeight(temp), temp);
    }
    generateList();
}
*/
/*
void generateList() {
    M[0][0].second.emplace_back(-1, -1);
    for(int i = 1; i < weight_A.size(); i++) {
        M[i][0].second.emplace_back(-1, -1);
    }
    for(int j = 1; j < weight_B.size(); j++) {
        M[0][j].second.emplace_back(-1, -1);
    }
}
*/

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//                                                                                                                   //
//SOBRECARGA DE OPERADORES                                                                                           //
//                                                                                                                   //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
/*
ostream& operator<<(ostream& out, vector<pair<int,int>> s) {
    for(auto & i : s) {
        out << " (" << i.first << ", " << i.second << ") ";
    }
    out << "\n";
    return out;
}

ostream& operator<<(ostream& out, Pesos s) {
    out << "Peso : " << s.first << " OptimalMatches: " << s.second;
    return out;
}

ostream& operator<<(ostream& out, pair<int,int> s) {
    out << s.first << " " << s.second << " ";
    return out;
}

ostream& operator<<(ostream& out, const vector<double>& weights) {
    for(auto& it : weights)
        out << it << " ";
    out << "\n";
    return out;
}

Tuples operator+(Tuples prev, Tuples next) {
    next.insert(next.end(), prev.begin(), prev.end());
    return next;
}

Pesos operator+(const Pesos& prev, const Pesos& next) {
    Tuples a = prev.second+next.second;
    return make_pair(prev.first+next.first, a);
}
*/
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//                                                                                                                   //
// FUNCIONES ÚTILES                                                                                                  //
//                                                                                                                   //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

void showMatrix() {
    for(int i = 0; i < weight_A.size(); i++) {
        for(int j = 0; j < weight_B.size(); j++) {
            cout << M[i][j] << " ";
        }
        cout << "\n";
    }
}

void readMatrix1(string name) {
    fstream file(name);
    string line;
    string number;
    while(getline(file, line)) {
        vector<int> row;
        for(char i : line) {
            if(i != ' ') {
                number.push_back(i);
            } else {
                row.push_back(stoi(number));
                number.clear();
            }
        }
        matrix1.push_back(row);
    }
    file.close();
}

void readMatrix2(string name) {
    fstream file(name);
    string line;
    string number;
    while(getline(file, line)) {
        vector<int> row;
        for(char i : line) {
            if(i != ' ') {
                number.push_back(i);
            } else {
                row.push_back(stoi(number));
                number.clear();
            }
        }
        matrix2.push_back(row);
    }
    file.close();
}

void generateTuplesA(Tuples& sequential, int i, int to_i, int j) {
    for(int k = i; k <= to_i; k++) {
        sequential.push_back(make_pair(k, j));
    }
}

void generateTuplesB(Tuples& sequential, int i, int j, int to_j) {
    for(int k = j; k <= to_j; k++) {
        sequential.push_back(make_pair(i, k));
    }
}

Tuples restoreTuples(Tuples restore) {
    Tuples response;
    if(restore[0].first == restore[0].second) {
        generateTuplesB(response, restore[0].first, restore[1].first, restore[1].second);
    } else {
        generateTuplesA(response, restore[0].first, restore[0].second, restore[1].first);
    }
    return response;
}

Tuples restoreMatches(Tuples restore, int index) {
    Tuples response = restoreTuples(restore);
    mapo[index] = response;
    if(restore[restore.size()-1].first == -1) {
        return response;
    }
    auto sum = restoreMatches(M[restore[restore.size()-1].first][restore[restore.size()-1].second].second, index+1);
    return response + sum;
}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//                                                                                                                   //
// FUNCIONES PARA MATRIZ Y VALORES                                                                                   //
//                                                                                                                   //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

void startBlocks(vector<int> array_A, vector<int> array_B) {
    int start;
    int finish = 0;
    for (int i = 0; i < array_A.size(); i++) {
        if(array_A[i] == 1) {
            start = i;
            while(array_A[i] == 1 && i < array_A.size()) {
                finish = i;
                i++;
            }
            weight_A.push_back((finish-start)+1);
            if(!accumulated_weights_A.empty()) {
                accumulated_weights_A.push_back(((finish - start) + 1) + accumulated_weights_A[accumulated_weights_A.size() - 1]);
            } else {
                accumulated_weights_A.push_back((finish - start) + 1);
            }
            pair<int,int> block = {start, finish};
            A.push_back(block);
        }
    }
    for (int i = 0; i < array_B.size(); i++) {
        if(array_B[i] == 1) {
            start = i;
            while(array_B[i] == 1 && i < array_B.size()) {
                finish = i;
                i++;
            }
            weight_B.push_back((finish-start)+1);
            if(!accumulated_weights_B.empty()) {
                accumulated_weights_B.push_back(((finish - start) + 1) + accumulated_weights_B[accumulated_weights_B.size() - 1]);
            } else {
                accumulated_weights_B.push_back((finish - start) + 1);
            }
            pair<int,int> block = {start, finish};
            B.push_back(block);
        }
    }
    blocks_A.push_back(A);
    blocks_B.push_back(B);
    mu = accumulated_weights_A[weight_A.size()-1]/accumulated_weights_B[weight_B.size()-1];
}

void baseCase() {
    Tuples init = {make_pair(0,0)};
    init.push_back(make_pair(0,0));
    init.push_back(make_pair(-1,-1));
    M[0][0] = make_pair(accumulated_weights_A[0] / accumulated_weights_B[0], init);
    for(int i = 1; i < weight_A.size(); i++) {
        Tuples temp;
        temp.push_back(make_pair(0,i));
        temp.push_back(make_pair(0,0));
        temp.push_back(make_pair(-1, -1));
        M[i][0] = make_pair(accumulated_weights_A[i] / weight_B[0], temp);
    }
    for(int j = 1; j < weight_B.size(); j++) {
        Tuples temp;
        temp.push_back(make_pair(0,0));
        temp.push_back(make_pair(0,j));
        temp.push_back(make_pair(-1, -1));
        M[0][j] = make_pair(weight_A[0] / accumulated_weights_B[j], temp);
    }
}

void initializeMatrix() {
    M = new Pesos *[A.size()];
    for(int i = 0; i < A.size(); i++) {
        M[i] = new Pesos[B.size()]{};
    }
    baseCase();
}

void deleteMatrix() {
    for(int i = 0; i < weight_A.size(); i++) {
        delete[] M[i];
    }
    delete [] M;
    weight_A.clear();
    weight_B.clear();
    accumulated_weights_A.clear();
    accumulated_weights_B.clear();
    M = nullptr;
}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//                                                                                                                   //
// FUNCIONES PARA EL ALGORITMO DINAMICO                                                                              //
//                                                                                                                   //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

void dynamicHelper(int i, int j, Pesos& value) {
    Tuples init;
    init.push_back(make_pair(i,i));
    init.push_back(make_pair(j,j));
    init.push_back(make_pair(i-1, j-1));
    value = make_pair(M[i-1][j-1].first+(weight_A[i]/weight_B[j]), init);
    for(int k = i-1; k > 0; k--) {
        double weight = ((accumulated_weights_A[i] - accumulated_weights_A[k - 1]) / weight_B[j]) + M[k - 1][j - 1].first;
        if(weight<value.first) {
            value.second.clear();
            Tuples aux;
            aux.push_back(make_pair(k,i));
            aux.push_back(make_pair(j,j));
            aux.push_back(make_pair(k-1, j-1));
            Pesos compare = make_pair(weight, aux);
            value = compare;
        }
    }
    for(int k = j-1; k > 0; k--) {
        double weight = (weight_A[i]/(accumulated_weights_B[j] - accumulated_weights_B[k - 1])) + M[i - 1][k - 1].first;
        if(weight<value.first) {
            value.second.clear();
            Tuples aux;
            aux.push_back(make_pair(i,i));
            aux.push_back(make_pair(k,j));
            aux.push_back(make_pair(i-1, k-1));
            Pesos compare = make_pair(weight, aux);
            value = compare;
        }
    }
}

Pesos dynamicSolution(int size_a, int size_b) {
    initializeMatrix();
    for(int i = 1; i <= size_a; i++) {
        for(int j = 1; j <= size_b; j++) {
            Pesos calculate;
            dynamicHelper(i, j, calculate);
            M[i][j] = calculate;
        }
    }
    auto optimal_solution = make_pair(M[size_a][size_b].first, restoreMatches(M[size_a][size_b].second, 0));
    deleteMatrix();
    return optimal_solution;
}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//                                                                                                                   //
// FUNCIONES PARA EL ALGORITMO DINAMICO CON PROMEDIOS                                                                //
//                                                                                                                   //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

void baseCaseAverage() {
    Tuples init = {make_pair(0,0)};
    init.push_back(make_pair(0,0));
    init.push_back(make_pair(-1,-1));
    M[0][0] = make_pair(abs(mu - (accumulated_weights_A[0]/accumulated_weights_B[0])), init);
    for(int i = 1; i < weight_A.size(); i++) {
        Tuples temp;
        temp.push_back(make_pair(0,i));
        temp.push_back(make_pair(0,0));
        temp.push_back(make_pair(-1, -1));
        M[i][0] = make_pair(abs(mu - (accumulated_weights_A[i] / weight_B[0])), temp);
    }
    for(int j = 1; j < weight_B.size(); j++) {
        Tuples temp;
        temp.push_back(make_pair(0,0));
        temp.push_back(make_pair(0,j));
        temp.push_back(make_pair(-1, -1));
        M[0][j] = make_pair(abs(mu - (weight_A[0] / accumulated_weights_B[j])), temp);
    }
}

void initializeMatrixAverage() {
    M = new Pesos *[A.size()];
    for(int i = 0; i < A.size(); i++) {
        M[i] = new Pesos[B.size()]{};
    }
    baseCaseAverage();
}

void dynamicHelperAverage(int i, int j, Pesos& value) {
    Tuples init;
    init.push_back(make_pair(i,i));
    init.push_back(make_pair(j,j));
    init.push_back(make_pair(i-1, j-1));
    value = make_pair((M[i-1][j-1].first+abs(mu - (weight_A[i]/weight_B[j]))), init);
    for(int k = i-1; k > 0; k--) {
        double weight = abs(mu - ((accumulated_weights_A[i] - accumulated_weights_A[k - 1])/weight_B[j])) + M[k - 1][j - 1].first;
        if(weight<value.first) {
            value.second.clear();
            Tuples aux;
            aux.push_back(make_pair(k,i));
            aux.push_back(make_pair(j,j));
            aux.push_back(make_pair(k-1, j-1));
            Pesos compare = make_pair(weight, aux);
            value = compare;
        }
    }
    for(int k = j-1; k > 0; k--) {
        double weight = abs(mu - (weight_A[i]/(accumulated_weights_B[j] - accumulated_weights_B[k - 1]))) + M[i - 1][k - 1].first;
        if(weight<value.first) {
            value.second.clear();
            Tuples aux;
            aux.push_back(make_pair(i,i));
            aux.push_back(make_pair(k,j));
            aux.push_back(make_pair(i-1, k-1));
            Pesos compare = make_pair(weight, aux);
            value = compare;
        }
    }
}

Pesos dynamicSolutionAverage(int size_a, int size_b) {
    initializeMatrixAverage();
    for(int i = 1; i <= size_a; i++) {
        for(int j = 1; j <= size_b; j++) {
            Pesos calculate;
            dynamicHelperAverage(i, j, calculate);
            M[i][j] = calculate;
        }
    }
    auto optimal_solution = make_pair(M[size_a][size_b].first, restoreMatches(M[size_a][size_b].second, 0));
    deleteMatrix();
    return optimal_solution;
}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//                                                                                                                   //
// FUNCIONES PARA ALGORITMO VORAZ                                                                                    //
//                                                                                                                   //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

Pesos greedyMatching(int i, int j) {
    double weight = 0;
    Tuples t_matches;
    if(i == weight_A.size()-1 && j == weight_B.size()-1) {
        weight = weight_A[i]/weight_B[j];
        t_matches = {make_pair(i, j)};
        return make_pair(weight, t_matches);
    } else if(i == weight_A.size()-1 && j != weight_B.size()-1) {
        do {
            weight += weight_B[j];
            t_matches.push_back(make_pair(i, j));
            j++;
        } while(j != weight_B.size());
        return make_pair(weight_A[i]/weight, t_matches);
    } else if(i != weight_A.size()-1 && j == weight_B.size()-1) {
        do {
            weight += weight_A[i];
            t_matches.push_back(make_pair(i, j));
            i++;
        } while(i != weight_A.size());
        return make_pair(weight/weight_B[j], t_matches);
    }
    weight = weight_B[j];
    t_matches.push_back(make_pair(i, j));
    while(weight_A[i]/weight > 1) {
        j++;
        t_matches.push_back(make_pair(i, j));
        weight += weight_B[j];
        if(j == weight_B.size()-2)
            break;
    }
    return greedyMatching(i+1, j+1) + make_pair(weight_A[i]/weight, t_matches);
}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
//                                                                                                                   //
// FUNCIÓN PRINCIPAL                                                                                                 //
//                                                                                                                   //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

void processImages(string name1, string name2) {
    readMatrix1(name1);
    readMatrix2(name2);
    for(int i = 0; i < matrix1.size(); i++) {
        startBlocks(matrix1[i], matrix2[i]);
        matches.push_back(dynamicSolution(weight_A.size()-1, weight_B.size()-1));
    }
}
//Pesos = (0,1),(0,2)...
//A = {(0, 2), (4, 4)...}
//B = {(), (), (), }
// TODO : mapa para traer las agrupaciones
// 0 -> 0, 1, 2, 3, 4, 5, 6
// 1 -> 7
// 2 -> 7
// A[it.first] << " " << B[it.second] SEGUN EL FOR PARA ACCEDER A LOS BLOQUES REALES EN EL ARREGLO
/*
void generateMap(Pesos solution) {

    for(int i = 0; i < solution.second.size()-1; i++) {
        if(solution.second[i].first == solution.second[i+1].first) {
            mapo[]
        }
    }
}*/

void transformationProcess(Pesos solution) {
    int midCase = 60;// future constant for multiple mid matrices
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

int main() {
    /*
    vector<int> array_A = {1,1,1,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1};
    vector<int> array_B = {0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0};
    vector<int> array_A = {0,0,0,1,1,1,0,0,1,0,0,1,1,0,1};
    vector<int> array_B = {0,0,1,1,1,1,1,0,1,1,0,1,1,1,0,0,0,0,1};
    startBlocks(array_A,array_B);
    auto solution = dynamicSolution(weight_A.size()-1, weight_B.size()-1);
    cout << solution << endl;
    transformationProcess(solution);*/
    /*cout << greedyMatching(0, 0) << endl;
    cout << dynamicSolutionAverage(weight_A.size()-1, weight_B.size()-1) << endl;
    processImages("matrizDibujo1.txt", "matrizDibujo2.txt");
    Image test("prueba.txt");
    cout << "hola\n";
    test.readFile();
    test.showBlocks();*/
    Matches prueba("matrizDibujo2.txt", "matrizDibujo1.txt", "1");
	cout<<"HOLA QUE TAL"<<endl;
    return 0;
}

//clang++ main.cpp < input.txt
