#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

void forwardSearch(vector<vector<double>> testData) {

    vector<double> featureSet;
    int currentFeatures[0];

    for(int i = 1; i <= testData.at(0).size() - 1; i++) {
        cout << "On the " << i << " th level of the search tree\n";

        for(int j = 1; j <= testData.at(0).size() - 1; j++) {
            cout << "--Considering adding the " << j << " feature\n";
        }
    }
}

int main () {

    ifstream file("CS170_Small_DataSet__1.txt");

    vector<vector<double>> data;
    string row;

    if(!file.is_open()) {
        EXIT_FAILURE;
    }

    while(getline(file, row)) {
        vector <double> values;
        stringstream ss(row);
        double value;

        while(ss >> value) {
            values.push_back(value);
        }

        data.push_back(values);
    }

    file.close();

    forwardSearch(data);



    return 0;
}