// Gal Yehezkel, ID: 315786228

#include "timeseries.h"
// fills map<string, vector<float>> table from the csv file.
TimeSeries::TimeSeries(const char* CSVfileName) {
    ifstream myFile(CSVfileName);
    string line, colname;
    float val;
    if(myFile.good()) {
        getline(myFile, line);
        stringstream ss(line);
        while(getline(ss,colname, ',')) {
            table.insert(pair<string, vector<float>>(colname, vector<float> {}));
            names.push_back(colname);
        }
    }
    while(getline(myFile, line)) {
        stringstream ss(line);
        int colIndex = 0;
        while(ss >> val) {
            table.at(names.at(colIndex)).push_back(val);
            if(ss.peek() == ',') 
                ss.ignore();
            colIndex++;
        }
    }
    myFile.close();
}
vector<string> TimeSeries::getNames() const {
    return names;
}
vector<float> TimeSeries::getCol(string name) const {
    return table.at(name);
}
int TimeSeries::getNumOfRows() const {
    return getCol(names[0]).size();
}
float TimeSeries::valueOfJinI(int row, string name) const {
    vector<float> temp = getCol(name);
    return temp[row];
}

