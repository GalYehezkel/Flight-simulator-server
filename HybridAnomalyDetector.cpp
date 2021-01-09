// Gal Yehezkel, ID: 315786228
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}
void HybridAnomalyDetector::detectCheck(vector<AnomalyReport>* anomalyReport, Point p, correlatedFeatures cfi, int j) {
	if(cfi.isSimple) { //if simple cf
		SimpleAnomalyDetector::detectCheck(anomalyReport, p, cfi, j);
	} else { //not simple, with circle.
		if(!isPointInCircle(&p, cfi.c)) { //if point not in circle.
			string desc = cfi.feature1 + '-' + cfi.feature2;
			anomalyReport->push_back(AnomalyReport(desc, j + 1));
		}
	}

}
void HybridAnomalyDetector::check(const TimeSeries& ts, int sizeOfVector, vector<string> names, string corralationWith, float corrlation, int i, float c) {
		//simple check.
		SimpleAnomalyDetector::check(ts, sizeOfVector, names,  corralationWith, corrlation, i, c);
		if(corrlation > 0.5 && corrlation < c) { //if should look for mincircle.
			Point *pi[sizeOfVector];
			for(int k = 0; k < sizeOfVector; k++) {
				pi[k] = new Point(ts.getCol(names[i]).at(k),ts.getCol(corralationWith).at(k));
			}
			correlatedFeatures tempcf;
			tempcf.feature1 = names[i];
			tempcf.feature2 = corralationWith;
			tempcf.corrlation = corrlation;
			tempcf.c = findMinCircle(pi, sizeOfVector);
			tempcf.c.radius *= 1.1;
			tempcf.isSimple = false;
			pushToVector(tempcf);
			for(int k = 0; k < sizeOfVector; k++) {
				delete(pi[k]);
			}	
		}
}
