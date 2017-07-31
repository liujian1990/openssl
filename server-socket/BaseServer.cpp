/*
 * BaseServer.cpp
 *
 *  Created on: 2017-7-27
 *      Author: caros
 */

#include "BaseServer.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>


BaseServer::BaseServer() {
	// TODO Auto-generated constructor stub
	_maxlisten=10;
	_port = 28443;

}

BaseServer::~BaseServer() {
	// TODO Auto-generated destructor stub
}

void BaseServer::warp(){

}

int BaseServer::listener(int port){
	int sd =  socket(PF_INET, SOCK_STREAM, 0);
	if ( sd == -1){
		perror("listener get sd");
	}
    struct sockaddr_in addr;
    memset(&addr,  0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0){
		perror("port"+port);
    }

   if (listen(sd,_maxlisten) != 0){
	   perror("listen in port " +port);
	   close(sd);
   }
	return sd;
};

bool  BaseServer::init(){
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
	return true;
};

bool  BaseServer::loadconfig( SSL_CTX* ctx ){

    std::string _privatekey_passwd = "1qaz2wsx!";
    char* pkey_passwd = (char*)_privatekey_passwd.data();
    const char ca_cert[] = "/ssl_cert/rootcachain.pem";
//    const char aduca_cert[] = "/ssl_cert/baidusigncert.pem";
    const char certfile[] = "/ssl_cert/BaiduADUdevelopercert.pem";
    const char keyfile[] = "/ssl_cert/BaiduADUdeveloper.key";

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
	return true;
};

bool  BaseServer::loop(){
	init();
    int srv_sd = listener(_port);
    std::cout << "Start Server in port:" << _port << std::endl;

    SSL* s = NULL;
    int remote_sd = 0;
    struct sockaddr_in remote_addr;
    unsigned int len =sizeof(remote_addr);
    SSL_CTX* srvctx = SSL_CTX_new(TLS_server_method());
    if (!loadconfig(srvctx)){
    	return false;
    };


   while(true){

	  remote_sd = accept(srv_sd, (struct sockaddr*)&remote_addr, &len);
      s = SSL_new(srvctx);

      SSL_set_fd(s, remote_sd);

      servlet(s);
      SSL_free(s);
      close(remote_sd);
      //remote_sd  and s close.
   }
   if(srv_sd >= 0){
	   close(srv_sd);
   }
   SSL_CTX_free(srvctx);
	return false;
};

void BaseServer::servlet(SSL* ssl){
    char buf[1024] = {};
	char ans[]="OK";
    if(SSL_accept(ssl) == 0){
		printf("No accept");
	}

    if(SSL_read(ssl, buf, sizeof(buf)) > 1){
        std::cout << "Get request." << buf <<std::endl;
        SSL_write(ssl,ans,sizeof(ans));
        SSL_write(ssl,buf,sizeof(buf));
    };

};


