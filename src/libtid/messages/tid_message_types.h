#ifndef TID_MESSAGE_TYPES_H
#define TID_MESSAGE_TYPES_H

#include "tid_message.h"
#include "tid_message_tags_1_0.h"

namespace TiD
{

//-----------------------------------------------------------------------------
class OkMessage : public TiDMessage
{
public:
    OkMessage (std::string const& version) : TiDMessage (version, "OK", "", "")
    {}
    virtual ~OkMessage () {}
};

//-----------------------------------------------------------------------------
class ErrorMessage : public TiDMessage
{
public:
    ErrorMessage (std::string const& version) : TiDMessage (version, "Error", "", "")
    {}
    virtual ~ErrorMessage () {}
};

//-----------------------------------------------------------------------------


}

#endif // TID_MESSAGE_TYPES_H
