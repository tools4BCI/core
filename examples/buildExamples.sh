#!/bin/bash

arch=amd64

g++ -Iinclude  tidSendingClient.cpp   libtid/lib/${arch}/libtid.a tobiid/lib/${arch}/libtobiid.a tobicore/lib/${arch}/libtobicore.a -l boost_system -l boost_thread -l pthread -l rt -o tidSendingClient
g++ -Iinclude  tidReceivingClient.cpp libtid/lib/${arch}/libtid.a tobiid/lib/${arch}/libtobiid.a tobicore/lib/${arch}/libtobicore.a -l boost_system -l boost_thread -l pthread -l rt -o tidReceivingClient

g++ -Iinclude  tidServer.cpp libtid/lib/${arch}/libtid.a tobiid/lib/${arch}/libtobiid.a tobicore/lib/${arch}/libtobicore.a -l boost_system -l boost_thread -l pthread -l rt -o tidServer
