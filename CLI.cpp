#include "CLI.h"

CLI::CLI(DefaultIO* dio){
    CLI::dio = dio;
    v[0] = new UploadTs(dio, &arguments);
    v[1] = new AlgoSet(dio, &arguments);
    v[2] = new DetectAnom(dio, &arguments);
    v[3] = new DisplayResult(dio, &arguments);
    v[4] = new UploadAnom(dio, &arguments);
    v[5] = new Exit(dio, &arguments);

}

void CLI::start(){
    int opp = 1;
    while(opp != 6) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for(int i = 1; i <= 6; i++) {
            dio->write(i);
            dio->write("." + v[i - 1]->getDesc() + "\n");
        }
        string option = dio->read();
        stringstream ss(option);
        ss >> opp;
        v[opp -1]->execute();
        std::cout << opp;
    }
}

CLI::~CLI() {
    for(int i = 0; i < 6; i++) {
        delete(v[i]);
    }
}

