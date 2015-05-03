/*
    This file is part of the libTiD test routine.

    The libTiD test routine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The libTiD test routine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the libTiD test routine. If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#include <string>
#include <sstream>
#include <iostream>

#include "UnitTest++/UnitTest++.h"

#include "tobiid/IDMessage.hpp"
#include "tobiid/IDSerializerRapid.hpp"
#include "tid_message_vector_builder.h"

using namespace std;

extern unsigned int NR_TID_MESSAGES;

//-------------------------------------------------------------------------------------------------

bool parseIstream(std::istream& stream, std::string& out_string, std::string& delimiter)
{

  if(stream.eof())
    return 0;

  std::getline(stream, out_string, delimiter[0]);
  if(stream.eof())
    return 0;
  out_string.push_back(delimiter[0]);

  char c = 0;
  for(unsigned int n = 1; n < delimiter.size(); )
  {
    stream.get(c);
    out_string.push_back(c);
    if(stream.eof())
      return 0;
    if(c == delimiter[n])
      n++;
    else
    {
      string tmp_string;
      std::getline(stream, tmp_string, delimiter[0]);
      out_string += tmp_string;
      out_string.push_back(delimiter[0]);
      if(stream.eof())
        return 0;
      n = 1;
    }
  }

   return 1;
}

//-------------------------------------------------------------------------------------------------

TEST(delimiterParsingTest)
{

  std::cout << "Running libTiD delimiter parsing test" << std::endl;
  #ifdef SKIP_STREAM_PARSING_TEST
    std::cout << "  --> skipping !!" << std::endl;
    return;
  #endif


  IDSerializerRapid serializer;
  IDSerializerRapid deserializer;
  IDMessage msg;
  deserializer.SetMessage(&msg);

  TiDMessageVectorBuilder msg_builder;
  msg_builder.generateMsgsVector(1000, NR_TID_MESSAGES);
  std::vector<IDMessage>& msgs_vec = msg_builder.getMessagesVector();

  string xml_str;
  string multi_xml_msg;
  for(unsigned int n = 0; n < msgs_vec.size(); n++)
  {
    serializer.SetMessage(&(msgs_vec[n]) );
    serializer.Serialize(&xml_str);
    multi_xml_msg.append(xml_str);
  }

  multi_xml_msg.resize(multi_xml_msg.size());


  //<tobiid version=\"0.0.2.1\" description=\"7Qfa4c51JsIJi0U6pUPLRQgWxRGmA97RIapdgMNll65AHCGV0f\" frame=\"64\" family=\"biosig\" event=\"6000\" timestamp=\"1330691458,821096\" reference=\"1330691458,821096\"/>
  //string multi_xml_msg = ("<tobiid version=\"0.0.2.1\" description=\"7Qfa4c51JsIJi0U6pUPLRQgWxRGmA97RIapdgMNll65AHCGV0f\" frame=\"64\" family=\"biosig\" event=\"6000\" timestamp=\"1330691458,821096\" reference=\"1330691458,821096\"/><tobiid version=\"0.0.2.1\" description=\"83DMO8w9F9yXs3jUJvqPemMB4bKawnSn7VDLu0i7Mh7vleXV6a\" frame=\"65\" family=\"biosig\" event=\"5009\" timestamp=\"1330691458,821109\" reference=\"1330691458,821110\"/><tobiid version=\"0.0.2.1\" description=\"9lxWeD9Br78dKxgQKqh4vdymzH4IPRjj3j9X6TSh2MyNfKnbq9\" frame=\"66\" family=\"biosig\" event=\"659");
  //  multi_xml_msg = ("1096\" reference=\"1330691458,821096\"/><tobiid version=\"0.0.2.1\" description=\"83DMO8w9F9yXs3jUJvqPemMB4bKawnSn7VDLu0i7Mh7vleXV6a\" frame=\"65\" family=\"biosig\" event=\"5009\" timestamp=\"1330691458,821109\" reference=\"1330691458,821110\"/><tobiid version=\"0.0.2.1\" description=\"9lxWeD9Br78dKxgQKqh4vdymzH4IPRjj3j9X6TSh2MyNfKnbq9\" frame=\"66\" family=\"biosig\" event=\"659");
  stringstream ss_stream(stringstream::in | stringstream::out);

  string out_str;
  string delimiter("</tid>");

  ss_stream << multi_xml_msg;

  //std::cout << multi_xml_msg << std::endl;

  bool run = 1;
  while(ss_stream.good() && run)
  {
    if( (run = parseIstream(ss_stream, out_str, delimiter)) )
      deserializer.Deserialize(&out_str);
    else
    {

    }
  }

  std::cout << " ... successfully" << std::endl << std::endl;

  return;
}
