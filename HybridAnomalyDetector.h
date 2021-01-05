// Gal Yehezkel, ID: 315786228

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
	void detectCheck(vector<AnomalyReport>* anomalyReport, Point p, correlatedFeatures cfi, int j);
	void check(const TimeSeries& ts, int sizeOfVector, vector<string> names, string corralationWith, float corrlation, int i, float c);


};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
