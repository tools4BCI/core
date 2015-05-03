/*
    This file is part of TOBI Interface D (TiD).

    TOBI Interface D (TiD) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TOBI Interface D (TiD) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TOBI Interface D (TiD).  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#ifndef TID_MESSAGE_BUILDER_1_0_H
#define TID_MESSAGE_BUILDER_1_0_H

#include "tid_message_tags_1_0.h"
#include "tid_message_builder.h"
#include "tid_exceptions.h"

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <thread>

#include <tobiid/IDSerializerRapid.hpp>
#include <tobicore/TCException.hpp>

namespace TiD
{

//-----------------------------------------------------------------------------
class TiDMessageBuilder10 : public TiDMessageBuilder
{
  public:
    TiDMessageBuilder10()
    {
      #ifdef DEBUG
        std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      serializer_ = new IDSerializerRapid();

      // FIXXXXXME
      // Bad hack to get around varying performance of tobiid

      IDMessage msg(IDMessage::TxtFamilyBiosig,10000);
      msg.SetDescription( "dhsfkjhfku84ewrkhfkjdhg7sfkhkshdfkjflaruihhdfskjhf74zkshgfksdhfsrfz7erfsdfkksdfh" );
      msg.SetBlockIdx(10000);
      msg.absolute.Tic();
      msg.relative.Tic();

      for(unsigned int n = 0; n < 10; n++)
      {
        std::string str;
        serializer_->SetMessage(&msg);
        serializer_->Serialize(&str);
      }

    }

    //-------------------------------------------

    virtual ~TiDMessageBuilder10()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
      if(serializer_)
        delete serializer_;
    }

    //-------------------------------------------

    virtual void buildTiDMessage (IDMessage& message, std::string& xml_string)
    {
      #ifdef DEBUG
        std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      try
      {
        serializer_->SetMessage(&message);
        serializer_->Serialize(&xml_string);
      }
      catch(TCException& e)
      {
        std::cerr << "TCException caught@ " << BOOST_CURRENT_FUNCTION <<
                 " -- " << e.GetCaller() << "/" << e.GetInfo() <<  std::endl;
        std::cerr << "  --> xml string: "  << xml_string <<  std::endl<< std::flush;
        throw;
      }
      catch(...)
      {
        std::cerr << "Uknown Exception caught@ " << BOOST_CURRENT_FUNCTION <<  std::endl;
        throw;
      }
    }

    //-------------------------------------------

  private:
    IDSerializerRapid*              serializer_;
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
