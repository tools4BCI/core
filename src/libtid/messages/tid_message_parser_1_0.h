#ifndef TID_MESSAGE_PARSER_1_0_H
#define TID_MESSAGE_PARSER_1_0_H

#include "tid_message_parser.h"

#include <boost/thread/mutex.hpp>

class IDSerializerRapid;

namespace TiD
{

class TiDMessageParser10 : public TiDMessageParser
{
public:
    TiDMessageParser10();
    virtual ~TiDMessageParser10 ();
    virtual void parseMessage (IDMessage* msg, InputStream* stream);

private:
    static unsigned const               MAX_LINE_LENGTH_;

    IDSerializerRapid*                  serializer_;
    std::string*                         xml_string_;

    boost::mutex                        mutex_;
};

}

#endif // TID_MESSAGE_PARSER_1_0_H
