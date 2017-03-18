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

#include "tid_message_parser_1_0.h"
//#include "tid_exceptions.h"

#include <assert.h>
#include <thread>

#include <iostream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/current_function.hpp>

#include <tobiid/IDMessage.hpp>
#include <tobiid/IDSerializerRapid.hpp>
#include <tobicore/TCException.hpp>

#include "../tid_exceptions.h"
#include "tid_message_tags_1_0.h"

using std::string;
using std::pair;
using boost::algorithm::trim_copy;

namespace TiD
{

//-----------------------------------------------------------------------------

TiDMessageParser10::TiDMessageParser10()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif


  // FIXXXXXME
  // Bad hack to get around varying performance of tobiid
  serializer_ = new IDSerializerRapid();

  xml_string_ = new std::string;

  xml_string_->reserve(1024);

  //for(unsigned int n = 0; n < 10; n++)
  //{
  //  IDMessage msg;
  //  serializer_->SetMessage(&msg);
  //  std::string str = "<tobiid version=\"0.0.2.1\" description=\"7Qfa4c51JsIJi0U6pUPLRQgWxRGmA97RIapdgMNll65AHCGV0f\" frame=\"64\" family=\"biosig\" event=\"6000\" timestamp=\"1330691458,821096\" reference=\"1330691458,821096\"/>";
  //  serializer_->Deserialize(&str);
  //}
}

//-----------------------------------------------------------------------------

TiDMessageParser10::~TiDMessageParser10()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(serializer_)
    delete serializer_;

  if(xml_string_)
    delete xml_string_;

}

//-----------------------------------------------------------------------------

void TiDMessageParser10::parseMessage (IDMessage* msg, InputStream* stream)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  xml_string_->clear();

  try
  {
    stream->readUntil(TiDMessageTags10::XML_END_STRING, xml_string_);
    serializer_->SetMessage(msg);
    serializer_->Deserialize(xml_string_);

    #ifdef DEBUG
      std::cout << *xml_string_ << std::endl;
    #endif
  }
  catch(TCException& e)
  {
    std::cerr << "TCException caught@ " << BOOST_CURRENT_FUNCTION <<
                 " -- " << e.GetCaller() << "/" << e.GetInfo() <<  std::endl;
    std::cerr << "  --> xml string: "  << *xml_string_ <<  std::endl<< std::flush;
    throw;
  }
  catch(TiDLostConnection& e)
  {
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION;
    std::cout << " -- " <<  "TiDLostConnection caught: " << e.what() << std::endl;
  #endif
    throw;
  }
  catch(...)
  {
    std::cerr << "Exception caught@ " << BOOST_CURRENT_FUNCTION <<  std::endl;
    std::cerr << "  --> xml string: "  << *xml_string_ <<  std::endl<< std::flush;
    throw;
  }

}

//-----------------------------------------------------------------------------

//  xml_string_ = "<tobiid version=\"0.0.2.1\" description=\"7Qfa4c51JsIJi0U6pUPLRQgWxRGmA97RIapdgMNll65AHCGV0f\" frame=\"64\" family=\"biosig\" event=\"6000\" timestamp=\"1330691458,821096\" reference=\"1330691458,821096\"/>";

}
