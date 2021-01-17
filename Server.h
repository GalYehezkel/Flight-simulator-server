// Gal Yehezkel, ID: 315786228

/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <thread> 
#include <pthread.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "commands.h"
#include "CLI.h"


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
		socketIO* s = new socketIO(clientID);
		CLI* c= new CLI(s);
		c->start();
		delete c;
		delete s;
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
	int fd;
	bool flag;
	sockaddr_in server;
	sockaddr_in client;
	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
