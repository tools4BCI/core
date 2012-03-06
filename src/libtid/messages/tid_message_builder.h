#ifndef TID_MESSAGE_BUILDER_H
#define TID_MESSAGE_BUILDER_H

//#include "tid_message.h"

class IDMessage;

namespace TiD
{

//-----------------------------------------------------------------------------
class TiDMessageBuilder
{
public:
    virtual ~TiDMessageBuilder () {}

    virtual void buildTiDMessage (IDMessage& message, std::string& xml_string) = 0;
};


}

#endif // TID_MESSAGE_BUILDER_H
