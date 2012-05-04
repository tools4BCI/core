#!/bin/bash

CMD="tid-server"

if [ ! -f bin/${CMD} ]; then
  echo "** ERROR - bin/${CMD} does not exist."
  exit 1
fi

PLATFORM=$(uname -m)

if [ "$PLATFORM" == "x86_64" ]
then
  LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib/amd64:$(pwd)/../build_amd64/src/tobicore/.libs::$(pwd)/../build_amd64/src/tobiid/.libs
else
  LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib/x86:$(pwd)/../build_x86/src/tobicore/.libs::$(pwd)/../build_x86/src/tobiid/.libs
fi

export PATH LD_LIBRARY_PATH
 exec ./bin/${CMD}  $1 $2 $3
# exec valgrind  --leak-check=full --show-reachable=yes ./bin/${CMD}  $1 $2 $3
# exec valgrind  ./bin/${CMD}  $1 $2 $3

