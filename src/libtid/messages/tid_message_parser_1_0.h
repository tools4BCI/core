#ifndef TID_MESSAGE_PARSER_1_0_H
#define TID_MESSAGE_PARSER_1_0_H

#include "tid_message_parser.h"
#include <tobiid/IDSerializerRapid.hpp>

namespace TiD
{

class TiDMessageParser10 : public TiDMessageParser
{
public:
    TiDMessageParser10();
    virtual ~TiDMessageParser10 ();
    virtual IDMessage parseMessage (InputStream& stream) const;


//    virtual void  getMessages (std::vector<TiDMessage>& messages);

private:
    static unsigned const               MAX_LINE_LENGTH_;

    IDSerializerRapid*                  serializer_;

//    std::string readVersion (InputStream& stream) const;
//    std::pair<std::string, std::string> readCommandAndParameter (InputStream& stream) const;
//    std::string readContent (InputStream& stream) const;
//    std::pair<std::string, std::string> getPair (std::string const& str) const;

//    bool                                             running_;
//    std::vector<TiDMessage>                          messages_;
};

}

#endif // TID_MESSAGE_PARSER_1_0_H
