#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>

using namespace std;

double leave_one_out_accuracy(const vector<vector<double>>& testData, const vector<int>& featureSet, int j) {
    int number_corr_class = 0;
    vector<int> features = featureSet;
    features.push_back(j);
    
    for(int i = 0; i < testData.size(); i++) {
        double near_neig_dist = INFINITY;
        int near_neig_label = 0;

        for(int k = 0; k < testData.size(); k++) {
            if(i == k) continue;
            double dist = 0.0;

            for(int m = 0; m < features.size(); m++) {
                int featureIndex = features.at(m);
                dist += ((testData[i][featureIndex] - testData[k][featureIndex]) * (testData[i][featureIndex] - testData[k][featureIndex]));

            }
            dist = sqrt(dist);

            if(dist < near_neig_dist) {
                near_neig_dist = dist;
                near_neig_label = testData.at(k).at(0);
            }

        }

        if(near_neig_label == testData.at(i).at(0)) number_corr_class++;
    }

    return (static_cast<double>(number_corr_class) / testData.size()) * 100;
}

bool containsFeature(const vector<int>& featureSet, int feature) {
    for (int i = 0; i < (int)featureSet.size(); i++) {
        if (featureSet[i] == feature) return true;
    }
    return false;
}

void printSet(const vector<int>& featureSet) {
    cout << "{";
    for (int i = featureSet.size() - 1; i >= 0; i++) {
        if (i > 0) cout << ",";
        cout << featureSet.at(i);
    }
    cout << "}";
}

double forwardSearch(const vector<vector<double>>& testData) {
    cout << "Beginning Search\n";
    double best_overall_accuracy = 0;
    vector<int> bestSet;
    vector<int> featureSet;
    double finalAccuracy = 0;
    int numFeatures = testData.at(0).size() - 1;
    for(int i = 1; i <= numFeatures; i++) {
        int feature_to_add_at_this_level = 0;
        double best_so_far_accuracy = 0;

        for(int j = 1; j <= numFeatures; j++) {
   
            if (containsFeature(featureSet, j)) continue;
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
        featureSet.push_back(feature_to_add_at_this_level);
        if(best_so_far_accuracy > best_overall_accuracy ) {
            best_overall_accuracy = best_so_far_accuracy;
            bestSet = featureSet;
        }
            cout << "Feature set {";
            for(int m = featureSet.size() - 1; m >= 0; m--) {
                cout << featureSet.at(m);
                if(m != 0) cout << ",";
            }
            cout << "} was best, accuracy is " << best_so_far_accuracy << "%\n";

        
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

double backwardElimination(const vector<vector<double>>& data){
    cout << "Beginning Elimination\n";
    vector<int> featureSet;
    int numFeatures = data[0].size() - 1;
    for(int i = 1; i <= numFeatures; i++) featureSet.push_back(i);
    vector<int> bestSet = featureSet;

    double bestOverallAccuracy = 0;
    double finalAccuracy = 0.0;

    while(featureSet.size() > 1) {
        int feature_to_remove = -1;
        double best_so_far_accuracy = 0.0;
        for(int i = 0; i < featureSet.size(); i++) {
            vector<int> newSet(featureSet.size() - 1);
            int index = 0;
            for(int p = 0; p < featureSet.size(); p++) {
                if(p != i) {
                    newSet[index++] = featureSet[p];
                }
            }
            int remoFeature = featureSet[i];

            double accuracy = leave_one_out_accuracy(data, newSet, newSet[0]);

            cout << "     Using feature (s) " <<"{"; 
            for (int l = 0; l < newSet.size(); l++) {
                cout << newSet[l];
                if(l < newSet.size() - 1) cout << ",";
            }
            cout << "} accuracy is " << accuracy << "%\n";
            if(accuracy > best_so_far_accuracy ) {
                best_so_far_accuracy = accuracy;
                feature_to_remove = remoFeature;
            }
        }

        for (int z = 0; z < featureSet.size(); z++) {
            if (featureSet[z] == feature_to_remove) {
                featureSet.erase(featureSet.begin() + z);
                break; 
            }
        }

        if(featureSet.size() >= 2) {
            cout << "Feature set {";
            for(int m = 0; m < featureSet.size(); m++) {
                cout << featureSet[m];
                if(m < featureSet.size() - 1) cout << ",";
            }
            cout << "} was best, accuracy is " << best_so_far_accuracy << "%\n";
        }
        
        finalAccuracy = best_so_far_accuracy;

        if(best_so_far_accuracy > bestOverallAccuracy) {
            bestOverallAccuracy = best_so_far_accuracy;
            bestSet = featureSet;
        }
    }
    cout << "Finished search!! The best feature subset is {";
    for(int n = 0; n < bestSet.size(); n++) {
        cout << bestSet[n];
        if(n != bestSet.size() - 1) cout << ",";
    }
    cout << "}, which has an accuracy of " << bestOverallAccuracy << "%\n";
    return finalAccuracy;
}

int main () {

    cout << "Welcome to Daniyal Bhat's Feature Selection Algorithm.";
    cout << "Type in the name of the file to test: ";
    string fileName;
    cin >> fileName;

    cout << "\nType the number of the algorithm you want to run.\n";
    cout << "     1) Forward Selection\n     2) Backward Elimination\n Enter the input ";
    int input;
    cin >> input;
    cout << endl;
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

    double accuracy = 0;

    file.close();
    cout << "This dataset has " << data.at(0).size() - 1 << " features "
        << "(not including the class attribute) with " << data.size() << " instances.\n";

    auto start = std::chrono::high_resolution_clock::now();
    if(input == 1) {
        accuracy = forwardSearch(data);
    } else {
        accuracy = backwardElimination(data);
    }
    
    auto stop = std::chrono::high_resolution_clock::now();

     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        cout << "Time taken is " << (duration.count() / 3600000000.0)  << " hours" << endl;

        cout << "Running nearest neighbor with all " << data.at(0).size() - 1 << " features, " 
            << "using leaving-one-out evaluation, I get an accuracy of "
            << accuracy << "%\n";

    return 0;
}