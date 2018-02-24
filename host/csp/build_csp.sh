#!/bin/sh

cd libcsp
./waf configure --verbose \
      --install-csp                   \
      --enable-rdp                    \
      --enable-qos                    \
      --enable-promisc                \
      --enable-crc32                  \
      --enable-hmac                   \
      --enable-xtea                   \
      --enable-bindings               \
      --enable-dedup                  \
      --enable-if-kiss                \
      --enable-if-can				  \
      --enable-can-socketcan          \
      --with-driver-usart=linux  	  \
      --with-os=posix            	  \
      --with-bufalign=8
#      --enable-if-zmqhub              \
#      --enable-examples               \
#      --with-rdp-max-window=SIZE 	  \
#      --with-max-bind-port=PORT       \
#      --with-max-connections=COUNT    \
#      --with-conn-queue-length=SIZE   \
#      --with-router-queue-length=SIZE \
#      --with-padding=BYTES			  \
#      --with-loglevel=LEVEL           \
#      --with-rtable=TABLE             \
#      --with-connection-so=CSP_SO     \
./waf build

