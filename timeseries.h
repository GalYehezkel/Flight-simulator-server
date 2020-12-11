// Gal Yehezkel, ID: 315786228

#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries{
map<string, vector<float>> table;
vector<string> names;
public:
	TimeSeries(const char* CSVfileName);
	vector<string> getNames() const;
	vector<float> getCol(string name) const;
	float valueOfJinI(int row, string name) const;
	int getNumOfRows() const;
};



#endif /* TIMESERIES_H_ */
