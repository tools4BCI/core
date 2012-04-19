#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H

#include <string>

namespace TiD
{

//-----------------------------------------------------------------------------
class InputStream
{
  public:
    virtual ~InputStream () {}
    virtual void readUntil (std::string delimiter, std::string* str) = 0;
};

}

#endif // INPUT_STREAM_H
