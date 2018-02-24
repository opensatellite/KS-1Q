To build KS-1Q ground control system

cd ../csp
./build_csp.sh
sudo ./install_csp.sh

cd ../KS1GCS
qmake
make

sudo apt install gnuradio
# version > 3.7.10 to support usrp b210

cd ../gr-kcsa-ks1q
mkdir build
cd build
cmake ..
make
sudo make install

To run KS-1Q GCS

python -u ccsds-halfduplex-tcpserver2.py
./KS1GCS

then click CONNECT button on KS1GCS GUI 
