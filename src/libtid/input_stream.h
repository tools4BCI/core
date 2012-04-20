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
    virtual void readUntil (const std::string& delimiter, std::string* str) = 0;
//    virtual void readUntil (std::string* str) = 0;
//    virtual void setDelimiter(const std::string& del) = 0;
};

}

#endif // INPUT_STREAM_H
