#ifndef TID_MESSAGE_PARSER_H
#define TID_MESSAGE_PARSER_H

#include <vector>

//#include "tid_message.h"
#include "../input_stream.h"

class IDMessage;

namespace TiD
{

//-----------------------------------------------------------------------------

/// TiDMessageParser
/// base class for a TiDMessageParser

class TiDMessageParser
{
  public:
    virtual ~TiDMessageParser () {}

    //-------------------------------------------------------------------------
    /// @throw exceptions if error occurs
    virtual void parseMessage (IDMessage* msg, InputStream* stream) = 0;
    //    virtual void  getMessages (std::vector<TiDMessage>& messages) = 0;

};


}

#endif // TID_MESSAGE_PARSER_H
