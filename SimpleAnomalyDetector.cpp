// Gal Yehezkel, ID: 315786228
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	vector<string> names = ts.getNames();
	for(int i = 0; i<names.size(); i++) {
		float corrlation = 0;
		string corralationWith;
		int sizeOfVector = ts.getCol(names[i]).size();
		for(int j = i + 1; j<names.size(); j++) {
			float tempcorrlation = pearson(&ts.getCol(names[i])[0], &ts.getCol(names[j])[0], sizeOfVector);
			if(tempcorrlation > corrlation) {
				corrlation = tempcorrlation;
				corralationWith = names[j];
			}
		}
		if(corrlation > 0.9) {
			Point *pi[sizeOfVector] = {};
			for(int k = 0; k < sizeOfVector; k++) {
				pi[k] = new Point(ts.getCol(names[i]).at(k),ts.getCol(corralationWith).at(k));
			}
			Line lin_reg = linear_reg(pi,sizeOfVector);
			float threshold = 0;
			for(int k = 0; k < sizeOfVector; k++) {
				float tempdev = dev(*pi[k], lin_reg);
				if(tempdev > threshold) {
					threshold = tempdev;
				}
				delete(pi[k]);
			}
			correlatedFeatures tempcf;
			tempcf.threshold = threshold;
			tempcf.feature1 = names[i];
			tempcf.feature2 = corralationWith;
			tempcf.lin_reg = lin_reg;
			tempcf.corrlation = corrlation;
			cf.push_back(tempcf);
		}
	}

}
correlatedFeatures* SimpleAnomalyDetector::findCorrolation(string feature) {
	for(int i = 0; i < cf.size(); i++) {
		if(cf[i].feature1 == feature || cf[i].feature2 == feature) {
			return &cf[i];
		}
	}
	return nullptr; //won't get here.
}
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> anomalyReport;
	vector<string> names = ts.getNames();
	int numOfRows = ts.getNumOfRows();
	for(int i = 0; i<numOfRows; i++) {
		for(int j = 0; j < names.size(); j++) {
			correlatedFeatures c = *findCorrolation(names[j]);
			string f1,f2;
			Point p(ts.valueOfJinI(i,c.feature1), ts.valueOfJinI(i,c.feature2));
			if(dev(p, c.lin_reg) > c.threshold * 1.2) {
				string desc = c.feature1 + '-' + c.feature2;
				anomalyReport.push_back(AnomalyReport(desc, ts.valueOfJinI(i, names[0])));
				break;
			}
		}	
	}
	return anomalyReport;
}


