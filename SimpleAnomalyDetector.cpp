// Gal Yehezkel, ID: 315786228
#include "SimpleAnomalyDetector.h"
#include <iostream>
#include <math.h>

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
			float tempcorrlation = abs(pearson(&ts.getCol(names[i])[0], &ts.getCol(names[j])[0], sizeOfVector));
			if(tempcorrlation > corrlation) {
				corrlation = tempcorrlation;
				corralationWith = names[j];
			}
		}
		if(corrlation > 0.9) {
			Point *pi[sizeOfVector];
			for(int k = 0; k < sizeOfVector; k++) {
				pi[k] = new Point(ts.getCol(names[i]).at(k),ts.getCol(corralationWith).at(k));
			}
			Line lin_reg = linear_reg(pi,sizeOfVector);
			float threshold = 0;
			for(int k = 0; k < sizeOfVector; k++) {
				float tempdev = abs(dev(*(pi[k]), lin_reg));
				if(tempdev > threshold) {
					threshold = tempdev;
				}
			}
			for(int k = 0; k < sizeOfVector; k++) {
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
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> anomalyReport;
	for(int i = 0; i < cf.size(); i++) {
		vector<float> f1 = ts.getCol(cf[i].feature1);
		vector<float> f2 = ts.getCol(cf[i].feature2);
		for(int j = 0; j < f1.size(); j++) {
			Point p(f1[j], f2[j]);
			if(dev(p, cf[i].lin_reg) > (cf[i].threshold * 1.15)) {
				string desc = cf[i].feature1 + '-' + cf[i].feature2;
				anomalyReport.push_back(AnomalyReport(desc, j + 1));
			}
		}

	}

	return anomalyReport;
}


