
#!/bin/bash

mex -I../src -Iinclude  matlab_tid_client.cpp \
../qmake/lib/amd64/libtid.a \
  ../build_amd64/src/tobiid/.libs/libtobiid.a \
        ../build_amd64/src/tobicore/.libs/libtobicore.a \
    -lboost_system -lSDL
