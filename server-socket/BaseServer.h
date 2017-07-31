/*
 * BaseServer.h
 *
 *  Created on: 2017-7-27
 *      Author: caros
 */

#ifndef BASESERVER_H_
#define BASESERVER_H_
#include "../openssl-1.1.0f/include/openssl/ssl.h"
#include "../openssl-1.1.0f/include/openssl/err.h"
#include "../openssl-1.1.0f/include/openssl/pem.h"
class BaseServer {
public:
	BaseServer();
	virtual ~BaseServer();
	void warp();
	bool loop();

private:
	int _maxlisten;
	int _port;
	bool init();
	bool loadconfig(SSL_CTX* ctx);
	void servlet(SSL* ssl);
	int listener(int port);
};

#endif /* BASESERVER_H_ */
