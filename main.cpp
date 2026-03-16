#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>

using namespace std;

int preductClass(const vector<double>& testData, const vector<vector<double>>& compData, const vector<int>& features) {
    
}

double leave_one_out_accuracy(vector<vector<double>> testData,vector<int> featureSet, int j) {
    int number_corr_class = 0;
    vector<int> features = featureSet;
    features.push_back(j);

    for(int i = 0; i < testData.size(); i++) {
        vector<vector<double>> compData;
        for(int j = 0; i < testData.size(); j++) {
            if(i != j) compData.push_back(testData.at(j));
        }

        int predictedClass = preductClass(testData.at(i), compData, features);
        if(predictedClass  == testData.at(i).at(0)) number_corr_class++;
    }

    return static_cast<double>(number_corr_class) / testData.size();
}

void forwardSearch(const vector<vector<double>>& testData) {

    vector<int> featureSet;

    for(int i = 1; i <= testData.at(0).size() - 1; i++) {
        cout << "On the " << i << " th level of the search tree\n";

        int feature_to_add_at_this_level = 0;
        double best_so_far_accuracy = 0;

        for(int j = 1; j <= testData.at(0).size() - 1; j++) {
            bool added = false;
            for(int k = 0; k < featureSet.size(); k++) {
                if(j == featureSet.at(k)) added = true;
            }
            if(!added) {
                cout << "--Considering adding the " << j << " feature\n";
                double accuracy = leave_one_out_accuracy(testData, featureSet, j);

                if(accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = accuracy;
                    feature_to_add_at_this_level = j;
                }
            }
            
        }
        featureSet.push_back(feature_to_add_at_this_level);

        cout << "On level " << i << " i added feature " << feature_to_add_at_this_level
        << " to current set\n";
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