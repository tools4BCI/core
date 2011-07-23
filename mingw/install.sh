#!/bin/sh
# 2010-03-21  Michele Tavella <michele.tavella@epfl.ch>

ROOT=$PWD
BIN_AUTOCONF=autoconf-2.68
BIN_AUTOMAKE=automake-1.11.1
BIN_LIBTOOL=libtool-2.4
BIN_MWRAP=mwrap-0.33

tar xvf $BIN_AUTOCONF.tar.gz
cd $BIN_AUTOCONF
mkdir build
cd build
../configure --prefix=/mingw/
make
make install
cd $ROOT

tar xvf $BIN_AUTOMAKE.tar.bz2
cd $BIN_AUTOMAKE
mkdir build
cd build
../configure --prefix=/mingw/
make
make install
cd $ROOT

tar xvf $BIN_LIBTOOL.tar.gz
cd $BIN_LIBTOOL
mkdir build
cd build
../configure --prefix=/mingw/
make
make install
cd $ROOT

tar xvf $BIN_MWRAP.tar.gz
cd $BIN_MWRAP
make
cp mwrap.exe /mingw/bin/
cd $ROOT
