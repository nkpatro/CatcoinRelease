#!/bin/bash

# Prompt for the password
read -s -p "Enter your sudo password: " PASSWORD
echo

# Function to run commands with sudo and password
run_with_sudo() {
    echo $PASSWORD | sudo -S "$@"
}

run_with_sudo apt --fix-broken install -y
run_with_sudo apt-get update -y
run_with_sudo apt install git -y
run_with_sudo apt install build-essential -y
run_with_sudo apt-get install qt5-default qtcreator -y
run_with_sudo apt install libdb5.3++-dev -y

wget -nc http://security.ubuntu.com/ubuntu/pool/main/o/openssl1.0/libssl1.0.0_1.0.2n-1ubuntu5.13_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/o/openssl1.0/libssl1.0-dev_1.0.2n-1ubuntu5.13_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/m/miniupnpc/libminiupnpc10_1.9.20140610-4ubuntu2_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/m/miniupnpc/libminiupnpc-dev_1.9.20140610-4ubuntu2_amd64.deb

wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/boost1.65.1_1.65.1+dfsg.orig.tar.bz2
wget -nc http://launchpadlibrarian.net/359703362/libboost1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://launchpadlibrarian.net/359703353/libboost1.65-all-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-system1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-serialization1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-serialization1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-thread1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-system1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-date-time1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-date-time1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-atomic1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-atomic1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-chrono1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-chrono1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-program-options1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-program-options1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-filesystem1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-filesystem1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost-thread1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
wget -nc http://security.ubuntu.com/ubuntu/pool/main/b/boost1.65.1/libboost1.65-tools-dev_1.65.1+dfsg-0ubuntu5_amd64.deb

mkdir source
cd source


run_with_sudo dpkg -i ../libssl1.0.0_1.0.2n-1ubuntu5.13_amd64.deb
run_with_sudo dpkg -i ../libssl1.0-dev_1.0.2n-1ubuntu5.13_amd64.deb
run_with_sudo dpkg -i ../libminiupnpc10_1.9.20140610-4ubuntu2_amd64.deb
run_with_sudo dpkg -i ../libminiupnpc-dev_1.9.20140610-4ubuntu2_amd64.deb

tar xvf ../boost1.65.1_1.65.1+dfsg.orig.tar.bz2
cd boost_1_65_1
./bootstrap.sh
run_with_sudo ./b2 install
# Exit boost
cd ..

run_with_sudo dpkg -i ../libboost1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost1.65-tools-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-system1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-system1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-chrono1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-chrono1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-thread1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-thread1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-filesystem1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-filesystem1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb

run_with_sudo dpkg -i ../libboost-atomic1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-atomic1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-serialization1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-serialization1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-program-options1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-program-options1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-date-time1.65.1_1.65.1+dfsg-0ubuntu5_amd64.deb
run_with_sudo dpkg -i ../libboost-date-time1.65-dev_1.65.1+dfsg-0ubuntu5_amd64.deb

git clone https://github.com/CatcoinOfficial/CatcoinRelease.git
mkdir catbuild
cd catbuild
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake ../CatcoinRelease/catcoin-qt.pro -r -spec linux-g++-64
make
./catcoin-qt




