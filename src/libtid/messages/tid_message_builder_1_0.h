#ifndef TID_MESSAGE_BUILDER_1_0_H
#define TID_MESSAGE_BUILDER_1_0_H

#include "tid_message_tags_1_0.h"
#include "tid_message_builder.h"
#include "tid_exceptions.h"

#include <boost/lexical_cast.hpp>

#include <tobiid/IDSerializerRapid.hpp>

namespace TiD
{

//-----------------------------------------------------------------------------
class TiDMessageBuilder10 : public TiDMessageBuilder
{
  public:
    TiDMessageBuilder10()
    {
      serializer_ = new IDSerializerRapid();
    }

    virtual ~TiDMessageBuilder10()
    {
      if(serializer_)
        delete serializer_;
    }

    virtual std::string buildTiDMessage (IDMessage& message)
    {
      serializer_->SetMessage(&message);
      std::string msg_buffer;
      serializer_->Serialize(&msg_buffer);

      return( msg_buffer );
    }

  private:
    IDMessage                       msg_;
    IDSerializerRapid*               serializer_;
};

}

#endif // TID_MESSAGE_BUILDER_10_H

//        std::string str = TiDMessageTags10::ID + TiDMessageTags10::SPACE;
//        str += message.getVersion ();
//        str += TiDMessageTags10::NEW_LINE;

//        str += message.getType ();
//        if (message.getParameters().size())
//        {
//            str += TiDMessageTags10::COMMAND_DELIMITER;
//            str += message.getParameters ();
//        }
//        str += TiDMessageTags10::NEW_LINE;

//        if (message.getContent().size())
//        {
//            str += TiDMessageTags10::CONTENT_LENGTH;
//            str += TiDMessageTags10::COMMAND_DELIMITER;
//            str += boost::lexical_cast<std::string>(message.getContent().size());
//            str += TiDMessageTags10::NEW_LINE;
//        }

//        str += TiDMessageTags10::NEW_LINE;
//        str += message.getContent ();
//        return str;
