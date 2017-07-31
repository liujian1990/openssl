/*
 * main.cpp
 *
 *  Created on: 2017-7-30
 *      Author: caros
 */
#include "BaseClient.h"

int main(){
	BaseClient* client = new BaseClient();
	char message[] = "What fuck!";
	if(client->request(message,sizeof(message))){
		printf("OK");
	};
	delete(client);
	return -1;
}



