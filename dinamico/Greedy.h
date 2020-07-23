#ifndef DINAMICO_GREEDY_H
#define DINAMICO_GREEDY_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

typedef vector<pair<int,int>> Tuples;
typedef pair<double,Tuples> Pesos;
typedef vector<Pesos> OptimalMatches;

class Greedy {
private:
    OptimalMatches optimal;
    vector<double> weight_A;
    vector<double> weight_B;
    vector<double> accumulated_weights_A;
    vector<double> accumulated_weights_B;
    Tuples A = {};
    Tuples B = {};
public:

};

#endif //DINAMICO_GREEDY_H
