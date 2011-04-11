%module pylibtobiic
%include "std_string.i"
%ignore Deserialize(std::string *);

#define DLLEXPORT

%{
#include "../../src/libtobiic/ICClass.hpp"
#include "../../src/libtobiic/ICClassifier.hpp"
#include "../../src/libtobiic/ICMessage.hpp"
#include "../../src/libtobiic/ICMessageRapid.hpp"
#include "../../src/libtobiic/ICTypes.hpp"

typedef int IValueType;
typedef float IValueData;
typedef int ILabelType;
typedef std::string ILabelData2;

%}

%include "../../src/libtobiic/ICClass.hpp"
%include "../../src/libtobiic/ICClassifier.hpp"
%include "../../src/libtobiic/ICMessage.hpp"
%include "../../src/libtobiic/ICMessageRapid.hpp"
%include "../../src/libtobiic/ICTypes.hpp"

typedef int IValueType;
typedef float IValueData;
typedef int ILabelType;
typedef std::string ILabelData2;

