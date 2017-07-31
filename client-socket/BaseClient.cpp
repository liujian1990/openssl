/*
 * BaseClient.cpp
 *
 *  Created on: 2017-7-30
 *      Author: caros
 */

#include "BaseClient.h"
#include "../comment/openssl_comment.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

BaseClient::BaseClient() {
	// TODO Auto-generated constructor stub

}

BaseClient::~BaseClient() {
	// TODO Auto-generated destructor stub
}

bool BaseClient::request(char* buf,int length){
	char rbuf[1024] = {};
	init();
	int remote_sd = doconnected(g_srvaddr,28443);
	if(remote_sd <0){
		return false;
	}

	SSL_CTX* clictx = SSL_CTX_new(TLS_client_method());
	if (clictx == NULL){
		std::cout << "Error in new ctx." << std::endl;
		return false;
	}

	SSL* ssl = SSL_new(clictx);
	if (ssl == NULL){
		return false;
	}

	SSL_set_fd(ssl, remote_sd);
	if((SSL_connect(ssl) == 0) ){
		std::cout << "Error in connect." << std::endl;
	}
	if(SSL_write(ssl, buf, length)){
		int bytes = SSL_read(ssl,rbuf,sizeof(buf));
		std::cout << "Recived:" << rbuf << "length" << bytes << std::endl;
	} else {
	    ERR_print_errors_fp(stderr);
	};
	SSL_shutdown(ssl);
	SSL_free(ssl);
    close(remote_sd);
    return false;
};

int  BaseClient::doconnected(const char* hostname, int port){
    struct hostent* host = NULL;
    struct sockaddr_in addr={};

    if ((host = gethostbyname(hostname)) == NULL) {
    	perror("No host name to connect");
    	return -1;
    }

    int sd = socket(PF_INET, SOCK_STREAM, 0);

    if( sd < 0){
    	perror("Error in get sd for host name");
    	return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);

    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        close(sd);
        perror("Connect fail address:" +addr.sin_addr.s_addr + port);
        return -1;
    }

    return sd;
}
bool BaseClient::init(){
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
	return true;
};

bool BaseClient::loadconfig(SSL_CTX* ctx){
//    char* pkey_passwd = static_cast<char*> ("1qaz2wsx");
	char pkey_passwd[] = "1qaz2wsx!";


    if (SSL_CTX_load_verify_locations(ctx, ca_cert, NULL) != 1) {
        ERR_print_errors_fp(stderr);
    };// add ca-cert in the ctx

    if (SSL_CTX_set_default_verify_paths(ctx) != 1) {
        ERR_print_errors_fp(stderr);
    };

    SSL_CTX_set_default_passwd_cb_userdata(ctx, pkey_passwd);

    if (SSL_CTX_use_certificate_file(ctx, certfile, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    };     //为SSL会话加载本应用的证书

    if (SSL_CTX_use_PrivateKey_file(ctx, keyfile, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    };//为SSL会话加载本应用的私钥

    if (!SSL_CTX_check_private_key(ctx)) {
        std::cout << "Private key does not match the public certificate" << "\n";
        return false;
    };
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
                         NULL); //NULL maybe cert
	return true;

};

