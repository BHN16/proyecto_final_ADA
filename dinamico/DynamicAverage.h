#ifndef DINAMICO_DYNAMICAVERAGE_H
#define DINAMICO_DYNAMICAVERAGE_H

class DynamicAverage {
private:
    OptimalMatches optimal;
    Tuples* A;
    Tuples* B;
    vector<double>* weight_A;
    vector<double>* weight_B;
    vector<double>* accumulated_weights_A;
    vector<double>* accumulated_weights_B;
    Helper* mapo;
    Pesos ** M;
    double mu;
public:
    vector<Helper>* matches;
    DynamicAverage() {
        this->M = nullptr;
        this->A = nullptr;
        this->B = nullptr;
        this->weight_A = nullptr;
        this->weight_B = nullptr;
        this->accumulated_weights_A = nullptr;
        this->accumulated_weights_B = nullptr;
        this->mapo = new Helper ;
        this->matches = new vector<Helper>;
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
        (*mapo)[index] = response;
        if(restore[restore.size()-1].first == -1) {

            return response;
        }
        auto sum = restoreMatches(M[restore[restore.size()-1].first][restore[restore.size()-1].second].second, index+1);
        return response + sum;
    }

    void baseCase() {
        Tuples init = {make_pair(0,0)};
        init.push_back(make_pair(0,0));
        init.push_back(make_pair(-1,-1));
        M[0][0] = make_pair(abs(mu-((*this->accumulated_weights_A)[0]/(*this->accumulated_weights_B)[0])), init);
        for(int i = 1; i < this->weight_A->size(); i++) {
            Tuples temp;
            temp.push_back(make_pair(0,i));
            temp.push_back(make_pair(0,0));
            temp.push_back(make_pair(-1, -1));
            M[i][0] = make_pair(abs(mu-((*this->accumulated_weights_A)[i] / (*this->weight_B)[0])), temp);
        }
        for(int j = 1; j < weight_B->size(); j++) {
            Tuples temp;
            temp.push_back(make_pair(0,0));
            temp.push_back(make_pair(0,j));
            temp.push_back(make_pair(-1, -1));
            M[0][j] = make_pair(abs(mu-((*this->weight_A)[0] / (*this->accumulated_weights_B)[j])), temp);
        }
    }

    void initializeMatrix() {
        this->M = new Pesos *[A->size()];
        for(int i = 0; i < A->size(); i++) {
            M[i] = new Pesos[B->size()]{};
        }
        baseCase();
    }

    void deleteMatrix() {
        for(int i = 0; i < this->weight_A->size(); i++) {
            delete[] M[i];
        }
        delete [] M;
        this->weight_A = nullptr;
        this->weight_B = nullptr;
        this->accumulated_weights_A = nullptr;
        this->accumulated_weights_B = nullptr;
        M = nullptr;
    }

    void dynamicAverageHelper(int i, int j, Pesos& value) {
        Tuples init;
        init.push_back(make_pair(i,i));
        init.push_back(make_pair(j,j));
        init.push_back(make_pair(i-1, j-1));
        value = make_pair(M[i-1][j-1].first+abs(mu-(((*this->weight_A)[i]/(*this->weight_B)[j]))), init);
        for(int k = i-1; k > 0; k--) {
            double weight = abs(mu-(((*this->accumulated_weights_A)[i] - (*this->accumulated_weights_A)[k - 1]) / (*this->weight_B)[j])) + M[k - 1][j - 1].first;
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
            double weight = abs(mu-(((*this->weight_A)[i]/((*this->accumulated_weights_B)[j] - (*this->accumulated_weights_B)[k - 1])))) + M[i - 1][k - 1].first;
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

    Pesos dynamicAverageSolution(int size_a, int size_b, Tuples* A, Tuples* B, vector<double>* weight_A, vector<double>* weight_B, vector<double>* accumulated_weights_A, vector<double>* accumulated_weights_B) {
        this->A = A;
        this->B = B;
        this->weight_A = weight_A;
        this->weight_B = weight_B;
        this->accumulated_weights_A = accumulated_weights_A;
        this->accumulated_weights_B = accumulated_weights_B;
        this->mu = (this->accumulated_weights_A->back()/this->accumulated_weights_B->back());
        this->initializeMatrix();
        for(int i = 1; i <= size_a; i++) {
            for(int j = 1; j <= size_b; j++) {
                Pesos calculate;
                dynamicAverageHelper(i, j, calculate);
                M[i][j] = calculate;
            }
        }
        auto optimal_solution = make_pair(M[size_a][size_b].first, restoreMatches(M[size_a][size_b].second, 0));
        this->deleteMatrix();
        this->matches->push_back((*this->mapo));
        this->mapo = new Helper;
        return optimal_solution;
    }
};
#endif //DINAMICO_DYNAMICAVERAGE_H
