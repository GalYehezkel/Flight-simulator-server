
// Gal Yehezkel, ID: 315786228

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};
class socketIO:public DefaultIO{
private:
int ClientId;
public:
	socketIO(int Clientid):DefaultIO() {
		ClientId = Clientid;
	}
	virtual string read(){
		string clientInput="";
		char c=0;
		::read(ClientId,&c,sizeof(char));
		while(c!='\n'){				
			clientInput+=c;
			::read(ClientId,&c,sizeof(char));
		}
		return clientInput;
	}
	virtual void write(string text) {
		send(ClientId, &text, text.size(), 0);
	}
	virtual void write(float f){
		send(ClientId, &f, sizeof(f), 0);
	}
	virtual void read(float* f){
		char buffer[1024];
		int n = recv(ClientId, buffer,100,0);
		stringstream s(buffer);
		s >> *f;
	}
};
// you may add here helper classes
class Arg{
public:
	HybridAnomalyDetector ad;
	vector<AnomalyReport> ar;
	Arg(HybridAnomalyDetector hy, vector<AnomalyReport> har):ad(hy),ar(har){}
	Arg(){}
};
class Range{
public:
	float start, end;
	Range(float start, float end):start(start),end(end){}
	Range(){}
};

// you may edit this class
class Command{
	DefaultIO* dio;
	string description;
	Arg* arguments;
	friend class UploadTs;
	friend class AlgoSet;
	friend class DetectAnom;
	friend class DisplayResult;
	friend class UploadAnom;
	friend class Exit;

public:
	Command(DefaultIO* dio):dio(dio){}
	Command(DefaultIO* dio, Arg* arguments):dio(dio),arguments(arguments){}
	virtual void execute()=0;
	virtual ~Command(){}
	string getDesc() { return description;}
};

// implement here your command classes
class UploadTs:public Command {
	public:
	UploadTs(DefaultIO* dio, Arg* arguments):Command(dio) {
		Command::arguments = arguments;
		description = "upload a time series csv file";
	}
	void execute() {
		string done = "done";
		dio->write("Please upload your local train CSV file.\n");
		std::ofstream trainFile;
		trainFile.open("anomalyTrain.csv");
		string line = dio->read();
		while(line.compare(done) != 0) { //if not equall.
			trainFile << line + "\n";
			line = dio->read();
		}
		trainFile.close();
		dio->write("Upload complete.\n");
		dio->write("Please upload your local test CSV file.\n");
		std::ofstream testFile;
		testFile.open("anomalyTest.csv");
		line = dio->read();
		while(line.compare(done) != 0) { //if not equall.
			testFile << line + "\n";
			line = dio->read();
		}
		testFile.close();
		dio->write("Upload complete.\n");
	}
};
class AlgoSet:public Command {
	public:
	AlgoSet(DefaultIO* dio, Arg* arguments):Command(dio) {
		description = "algorithm settings";
		Command::arguments = arguments;
	}
	void execute() {
		float temp = -1;
		dio->write("The current correlation threshold is ");
		dio->write(arguments->ad.getThreshold());
		dio->write("\n");
		dio->write("Type a new threshold\n");
		dio->read(&temp);
		while(temp > 1 || temp < 0) {
			dio->write("please choose a value between 0 and 1.\n");
			dio->read(&temp);
		}
		arguments->ad.setThreshold(temp);
	}
};
class DetectAnom:public Command {
	public:
	DetectAnom(DefaultIO* dio, Arg* arguments):Command(dio) {
		description = "detect anomalies";
		Command::arguments = arguments;
	}
	void execute() {
		TimeSeries tsTrain("anomalyTrain.csv");
		TimeSeries tsTest("anomalyTest.csv");
		arguments->ad.learnNormal(tsTrain);
		arguments->ar = arguments->ad.detect(tsTest);
		dio->write("anomaly detection complete.\n");
	}
};
class DisplayResult:public Command {
	public:
	DisplayResult(DefaultIO* dio, Arg* arguments):Command(dio) {
		description = "display results";
		Command::arguments = arguments;
	}
void execute() {		
		int size = arguments->ar.size();
		for(int i = 0; i < size; i++) {
			dio->write(arguments->ar[i].timeStep);
			string s = "         "  + arguments->ar[i].description + "\n";
			dio->write(s);
		}
		dio->write("Done.\n");
	}
};
class UploadAnom:public Command {
	public:
	UploadAnom(DefaultIO* dio, Arg* arguments):Command(dio) {
		description = "upload anomalies and analyze results";
		Command::arguments = arguments;
	}
	static bool comprator(const Range& a1, const Range& a2) {
		return (a1.start < a2.start);
	}
	void execute() {
		
		float TP = 0, FP = 0, P = 0, N = arguments->ad.getInputSize(), begin = 0, finish = 0;
		int index = 0;
		string description;
		int size = arguments->ar.size();
		vector<Range> r;
		Range temp;
		for(int i = 0; i < size; i++) {
			begin = arguments->ar[i].timeStep;
			finish = arguments->ar[i].timeStep;
			int j = i + 1;
			for(j = i + 1; j < size; j++) {
				if(arguments->ar[j].timeStep != (finish + 1)) {
					break;
				} else {
					finish++;
				}
			}
			i = j - 1;
			temp.start = begin;
			temp.end = finish;
			r.push_back(temp);
		}
		sort(r.begin(), r.end(), comprator);
		size = r.size();
		bool flag = false;
		dio->write("Please upload your local anomalies file.\n");
		float start, end;
		vector<Range> input;
		string line, a, b;
		char* x, y;
		string done = "done";
		while((line = dio->read()) != "done") {
			int d = 0;
			while(line[d] != ',') {
				d++;
			}
			a = line.substr(0, d);
			b = line.substr(d + 1, line.length() - d);
			temp.start = atoi(a.c_str());
			temp.end = atoi(b.c_str());
			input.push_back(temp);
		}
		sort(input.begin(), input.end(), comprator);
		int size2 = input.size();
		int k = 0;
		while(k != size2) {
			flag = false;
			start = input[k].start;
			end = input[k].end;
			while(r[index].start <= end && index < size) {
				if(r[index].end < start) { //false postivie.
					FP++;
					index++;
				} else if(r[index].start > end) { //out of range of line.
					//k++;
					//P++;
					break;
				} else if(((r[index].start < start && r[index].end >= start) 
				|| (r[index].start >= start && r[index].end <= end) 
				|| (r[index].start <= end && r[index].end > end))) { //bigger then zero.
					flag = true;
					index++;
				} else {
					index++;
				}
			}	
			if(flag) { //can add here FN.
				TP++;
			}
			N -= (end - start + 1);
			k++;
			P++;
		}
		FP += (size - index);
		dio->write("Upload complete.\n");
		float trueRate =  (TP / P) * 1000;
		trueRate = floor(trueRate);
		trueRate /= 1000;
		dio->write("True Positive Rate: ");
		dio->write(trueRate);
		dio->write("\n");
		float falseRate = (FP / N) * 1000;
		falseRate = floor(falseRate);
		falseRate /= 1000;
		dio->write("False Positive Rate: ");
		dio->write(falseRate);
		dio->write("\n");

	}
};
class Exit:public Command {
	public:
	Exit(DefaultIO* dio, Arg* arguments):Command(dio) {
		description = "exit";
		Command::arguments = arguments;
	}
	void execute() {
	//	dio->~DefaultIO();
	}
};


#endif /* COMMANDS_H_ */
