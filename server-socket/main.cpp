/*
 * mian.cpp
 *
 *  Created on: 2017-7-27
 *      Author: caros
 */

#include <openssl/ssl.h>
#include "BaseServer.h"

int main() {

	BaseServer testsrv;
	testsrv.loop();
}



