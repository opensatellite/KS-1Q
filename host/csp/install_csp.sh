#!/bin/sh

cd libcsp
./waf install
cp -v -R include/csp /usr/local/include/
cp -v -R build/include/csp /usr/local/include/

