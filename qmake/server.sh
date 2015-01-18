#!/bin/bash

CMD="tid-server"

#if [ ! -f bin/${CMD} ]; then
#  echo "** ERROR - bin/${CMD} does not exist."
#  exit 1
#fi

PLATFORM=$(uname -m)

if [ "$PLATFORM" == "x86_64" ]
then
  if [ ! -f bin/amd64/${CMD} ]; then
    echo "** ERROR - bin/${CMD} does not exist."
    exit 1
  fi
  LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib/amd64:$(pwd)/../build_amd64/src/tobicore/.libs::$(pwd)/../build_amd64/src/tobiid/.libs
else
  LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib/x86:$(pwd)/../build_x86/src/tobicore/.libs::$(pwd)/../build_x86/src/tobiid/.libs
  if [ ! -f bin/x86/${CMD} ]; then
    echo "** ERROR - bin/${CMD} does not exist."
    exit 1
  fi

fi

export PATH LD_LIBRARY_PATH
if [ "$PLATFORM" == "x86_64" ]
then

# exec ./bin/amd64/${CMD}  $1 $2 $3
# exec valgrind  --leak-check=full --show-reachable=yes ./bin/amd64/${CMD}  $1 $2 $3
 exec valgrind  ./bin/amd64/${CMD}  $1 $2 $3
else
 exec ./bin/x86/${CMD}  $1 $2 $3
fi
