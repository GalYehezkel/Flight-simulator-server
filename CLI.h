// Gal Yehezkel, ID: 315786228


#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	Arg arguments;
	DefaultIO* dio;
	Command *v[6];
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
