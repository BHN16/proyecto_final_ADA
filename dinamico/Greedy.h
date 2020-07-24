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
    Tuples* A;
    Tuples* B;
    vector<double>* weight_A;
    vector<double>* weight_B;
    vector<double>* accumulated_weights_A;
    vector<double>* accumulated_weights_B;
    Helper* mapo;
    int index = 0;
public:
    vector<Helper>* matches;
    Greedy() {
        this->A = nullptr;
        this->B = nullptr;
        this->weight_A = nullptr;
        this->weight_B = nullptr;
        this->accumulated_weights_A = nullptr;
        this->accumulated_weights_B = nullptr;
        this->mapo = new Helper ;
        this->matches = new vector<Helper>;
    }

    void greedySolution(Tuples* A, Tuples* B, vector<double>* weight_A, vector<double>* weight_B, vector<double>* accumulated_weights_A, vector<double>* accumulated_weights_B) {
        this->A = A;
        this->B = B;
        this->weight_A = weight_A;
        this->weight_B = weight_B;
        this->accumulated_weights_A = accumulated_weights_A;
        this->accumulated_weights_B = accumulated_weights_B;
        auto a = this->greedyMatching(0,0);
        this->optimal.push_back(a);
        this->weight_A = nullptr;
        this->weight_B = nullptr;
        this->accumulated_weights_A = nullptr;
        this->accumulated_weights_B = nullptr;
        Helper* b = new Helper();
        for(auto it = this->mapo->rbegin(); it != this->mapo->rend(); ++it) {
            (*b)[index] = it->second;
            index--;
        }
        delete mapo;
        mapo = b;
        b = nullptr;
        this->matches->push_back((*this->mapo));
        this->mapo = new Helper;
    }

    Pesos greedyMatching(int i, int j) {
        double weight = 0;
        Tuples t_matches;
        if(i == weight_A->size()-1 && j == weight_B->size()-1) {
            weight = (*weight_A)[i]/(*weight_B)[j];
            t_matches = {make_pair(i, j)};
            (*mapo)[index] = t_matches;
            return make_pair(weight, t_matches);
        } else if(i == weight_A->size()-1 && j != weight_B->size()-1) {
            do {
                weight += (*weight_B)[j];
                t_matches.push_back(make_pair(i, j));
                j++;
            } while(j != weight_B->size());
            (*mapo)[index] = t_matches;
            return make_pair((*weight_A)[i]/weight, t_matches);
        } else if(i != weight_A->size()-1 && j == weight_B->size()-1) {
            do {
                weight += (*weight_A)[i];
                t_matches.push_back(make_pair(i, j));
                i++;
            } while(i != weight_A->size());
            (*mapo)[index] = t_matches;
            return make_pair(weight/(*weight_B)[j], t_matches);
        }
        weight = (*weight_B)[j];
        t_matches.push_back(make_pair(i, j));
        while((*weight_A)[i]/weight > 1) {
            j++;
            t_matches.push_back(make_pair(i, j));
            weight += (*weight_B)[j];
            if(j == weight_B->size()-2)
                break;
        }
        (*mapo)[index] = t_matches;
        index++;
        return greedyMatching(i+1, j+1) + make_pair((*weight_A)[i]/weight, t_matches);
    }
};
#endif //DINAMICO_GREEDY_H
