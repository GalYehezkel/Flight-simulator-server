// Gal Yehezkel, ID: 315786228


#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>


struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	bool isSimple = true;
	Circle c;
	
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	vector<correlatedFeatures> cf;
	float thresholdP;
	int n;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	int getInputSize();
	virtual void check(const TimeSeries& ts, int sizeOfVector, vector<string> names, string corralationWith, float corrlation, int i, float c);
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	void pushToVector(correlatedFeatures insertCf);
	virtual void detectCheck(vector<AnomalyReport>* anomalyReport, Point p, correlatedFeatures cfi, int j);
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
	void setThreshold(float ths);
	float getThreshold();

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
