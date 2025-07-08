#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

struct dataPoint
{
    vector<double> Features;
    string label;
};

vector<dataPoint> readCSV(string fileName)
{
    vector<dataPoint> data;
    ifstream file(fileName);
    string line;

    getline(file, line);

    while(getline(file,line))
    {
        vector<double> features;
        string label;

        stringstream ss (line);
        string value;

        while(getline(ss,value,','))
        {
            try
            {
                features.push_back(stod(value));
            }
            catch (...)
            {
                label = value;
            }
        }
        if(!features.empty() && !label.empty())
            data.push_back({features,label});
    }

    return data;
}

double euclideanDistance(vector<double> A, vector<double> B)
{
    double sum = 0.0;

    for(size_t i = 0 ; i < A.size() ; i++)
        sum += pow(A[i] - B[i],2);

    return sqrt(sum);
}

string predictLabel(vector<dataPoint> trainingData, vector<double> testPoint, int k)
{
    vector<pair<double, string>> distances;

    for(auto data : trainingData)
    {
        double dist = euclideanDistance(data.Features, testPoint);
        distances.push_back({dist,data.label});
    }

    sort(distances.begin(),distances.end());

    map<string, int> labelCount;
    for (size_t i = 0; i < k && i < distances.size(); ++i)
    {
        labelCount[distances[i].second]++;
    }
    string predictedLabel;
    int maxCount = 0;
    for (auto& pair : labelCount)
    {
        if (pair.second > maxCount)
        {
            maxCount = pair.second;
            predictedLabel = pair.first;
        }
    }

    return predictedLabel;


}


int main()
{
    vector<dataPoint> trainingData = readCSV("knn_clean_data.csv");

    int numFeatures = trainingData[0].Features.size();
    cout << "Enter values for a new test point (" << numFeatures << " features):" << endl;

    vector<double> testPoint(numFeatures);
    for (int i = 0; i < numFeatures; ++i)
    {
        cout << "Feature " << i + 1 << ": ";
        cin >> testPoint[i];
    }

    int K;
    cout << "Enter the number of neighbors (K): ";
    cin >> K;

    string result = predictLabel(trainingData, testPoint, K);
    cout << "Predicted label: " << result << endl;

    return 0;
}
