/*
 * BaseClient.h
 *
 *  Created on: 2017-7-30
 *      Author: caros
 */

#ifndef BASECLIENT_H_
#define BASECLIENT_H_

#include "../openssl-1.1.0f/include/openssl/ssl.h"
#include "../openssl-1.1.0f/include/openssl/err.h"

class BaseClient {
public:
	BaseClient();
	virtual ~BaseClient();
	bool request(char* buf,int length);
private:
	bool init();
	bool loadconfig(SSL_CTX* ctx);
	int doconnected(const char* hostname, int port);
};

#endif /* BASECLIENT_H_ */
