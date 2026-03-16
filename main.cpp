#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>

using namespace std;

double compDist(const vector<double>& testData, const vector<double>& compDataPoint, const vector<int>& features) {
    double dist = 0;
    for(int i = 0; i < features.size(); i++) {
        dist += pow((testData.at(features.at(i)) - compDataPoint.at(features.at(i))), 2);
    }

    return sqrt(dist);
}

int predictClass(const vector<double>& testData, const vector<vector<double>>& compData, const vector<int>& features) {
    double near_neig_dist = INFINITY;
    int near_neig_label = 0;

    for(int i = 0; i < compData.size(); i++) {
        double distance = compDist(testData, compData.at(i), features);
        if(distance < near_neig_dist) {
            near_neig_dist = distance;
            near_neig_label = compData.at(i).at(0);
        }
    }
    return near_neig_label;
}

double leave_one_out_accuracy(const vector<vector<double>>& testData, const vector<int>& featureSet, int j) {
    int number_corr_class = 0;
    vector<int> features = featureSet;
    features.push_back(j);
    for(int i = 0; i < testData.size(); i++) {
        vector<vector<double>> compData;
        for(int j = 0; j < testData.size(); j++) {
            if(i != j) compData.push_back(testData.at(j));
        }

        int predictedClass = predictClass(testData.at(i), compData, features);
        if(predictedClass  == testData.at(i).at(0)) number_corr_class++;
    }

    return (static_cast<double>(number_corr_class) / testData.size()) * 100;
}

double forwardSearch(const vector<vector<double>>& testData) {
    cout << "Beginning Search\n";
    double best_overall_accuracy = 0;
    vector<int> bestSet;
    vector<int> featureSet;
    double finalAccuracy = 0;

    for(int i = 1; i <= testData.at(0).size() - 1; i++) {
        int feature_to_add_at_this_level = 0;
        double best_so_far_accuracy = 0;

        for(int j = 1; j <= testData.at(0).size() - 1; j++) {
            bool added = false;
            for(int k = 0; k < featureSet.size(); k++) {
                if(j == featureSet.at(k)) added = true;
            }
            if(!added) {
                double accuracy = leave_one_out_accuracy(testData, featureSet, j);
                cout << "     Using feature (s) " <<"{"; 
                cout << j;
                if(featureSet.size() != 0) cout << ",";
                for (int l = featureSet.size() - 1; l >= 0; l--) {
                    cout << featureSet.at(l);
                    if(l != 0) cout << ",";
                }
                cout << "} accuracy is " << accuracy << "%\n";
                if(accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = accuracy;
                    feature_to_add_at_this_level = j;
                }
            }
            
        }
        featureSet.push_back(feature_to_add_at_this_level);
        if(best_so_far_accuracy > best_overall_accuracy ) {
            best_overall_accuracy = best_so_far_accuracy;
            bestSet = featureSet;
        }
        if(i != testData.at(0).size()) {
            cout << "Feature set {";
            for(int m = featureSet.size() - 1; m >= 0; m--) {
                cout << featureSet.at(m);
                if(m != 0) cout << ",";
            }
            cout << "} was best, accuracy is " << best_so_far_accuracy << "%\n";
        }
        
        
        finalAccuracy = best_so_far_accuracy;
    }
    
    cout << "Finished search!! The best feature subset is {";
    for(int n = bestSet.size() - 1; n >= 0; n--) {
        cout << bestSet.at(n);
        if(n != 0) cout << ",";
    }
    cout << "}, which has an accuracy of " << best_overall_accuracy << "%\n";
    return finalAccuracy;
}

int main () {

    cout << "Welcome to Daniyal Bhat's Feature Selection Algorithm.";
    cout << "Type in the name of the file to test: ";
    string fileName;
    cin >> fileName;

    cout << "\nType the number of the algorithm you want to run.\n";
    cout << "     1) Forward Selection\n     2) Backward Elimination\n";

    ifstream file(fileName);

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
    cout << "This dataset has " << data.at(0).size() - 1 << " features "
        << "(not including the class attribute) with " << data.size() << " instances.\n";

    auto start = std::chrono::high_resolution_clock::now();
    double accuracy = forwardSearch(data);
    auto stop = std::chrono::high_resolution_clock::now();

     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        cout << "Time taken is " << (duration.count() / 1000000.0)  << " seconds" << endl;

        cout << "Running nearest neighbor with all 4 features, " 
            << "using leaving-one-out evaluation, I get an accuracy of "
            << accuracy << "%\n";

    return 0;
}