PATH=c:\mingw\bin;%PATH%
g++ -c -o ICClass.o ../../src/libtobiic/ICClass.cpp -DDLLEXPORT=
g++ -c -o ICClassifier.o ../../src/libtobiic/ICClassifier.cpp -DDLLEXPORT=
g++ -c -o ICMessage.o ../../src/libtobiic/ICMessage.cpp -DDLLEXPORT=
g++ -c -o ICMessageRapid.o ../../src/libtobiic/ICMessageRapid.cpp -DDLLEXPORT= -Ic:\dropbox\tobi\rapidxml
g++ -c -o ICTools.o ../../src/libtobiic/ICTools.cpp -DDLLEXPORT=
g++ -o libtobiic.dll -shared *.o
