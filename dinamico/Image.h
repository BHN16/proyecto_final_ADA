#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <utility>

using namespace std;

typedef vector<pair<int,int>> Tuples;
typedef vector<Tuples> Blocks;

class Image{
public:

    vector<vector<int>> matrix;
    vector<vector<double>> weights;
    vector<vector<double>> accumulated_weights;
    Blocks blocks;

    Image()= default;

    void readFile(const string& file) {
        fstream image(file);
        string line;
        while(getline(image, line)) {
            vector<int> row;
            Tuples row_block;
            vector<double> auxiliary_weight;
            vector<double> auxiliary_accumulated_weight;
            int start = 0;
            int finish = 0;
            for(int i = 0; i < line.size(); i+=2) {
                if(line[i] == '1') {
                    start = i>>1;
                    finish = i>>1;
                    row.push_back(1);
                    while(i < line.size()) {
                        if(line[i] == '0') { break; }
                        row.push_back(1);
                        finish = i>>1;
                        i+=2;
                    }
                    row_block.push_back(make_pair(start,finish));
                    auxiliary_weight.push_back(finish-start+1);
                    if(auxiliary_accumulated_weight.empty()) {
                        auxiliary_accumulated_weight.push_back(finish-start+1);
                    } else {
                        auxiliary_accumulated_weight.push_back((finish-start+1)+auxiliary_accumulated_weight.back());
                    }
                } else {
                    row.push_back(0);
                }
            }
            this->weights.push_back(auxiliary_weight);
            this->matrix.push_back(row);
            this->blocks.push_back(row_block);
            this->accumulated_weights.push_back(auxiliary_accumulated_weight);
            row.clear();
            row_block.clear();
            auxiliary_weight.clear();
            auxiliary_accumulated_weight.clear();
        }
        cin.sync();
        image.close();
        this->showBlocks();
    }

    void showBlocks() {
        for(auto& it : blocks) {
            for(auto&it2 : it) {
                cout << "(" << it2.first << ", " << it2.second << ") ";
            }
            cout << "\n";
        }
    }

    void showWeights() {
        for(auto&it : weights) {
            for(auto&it2: it) {
                cout << "(" << it2 << "), ";
            }
            cout << "\n";
        }
    }

    void showAccumulatedWeights() {
        for(auto&it : accumulated_weights) {
            for(auto&it2: it) {
                cout << "(" << it2 << "), ";
            }
            cout << "\n";
        }
    }

    void showMatrix() {
        for(const auto& it : matrix) {
            for(auto it2 : it) {
                cout << it2 << " ";
            }
            cout << "\n";
        }
    }
};

