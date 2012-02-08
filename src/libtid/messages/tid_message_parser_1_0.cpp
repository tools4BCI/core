#include "tid_message_parser_1_0.h"
#include "tid_message_tags_1_0.h"
//#include "tid_exceptions.h"

#include <sstream>
#include <iostream>
#include <boost/algorithm/string/trim.hpp>

#include <tobiid/IDMessage.hpp>

using std::string;
using std::pair;
using boost::algorithm::trim_copy;

namespace TiD
{

//-----------------------------------------------------------------------------

TiDMessageParser10::TiDMessageParser10()
{
  #ifdef DEBUG
    std::cout << "TiDMessageParser10::Constructor " << std::endl;
  #endif

  serializer_ = new IDSerializerRapid();
}

//-----------------------------------------------------------------------------

TiDMessageParser10::~TiDMessageParser10()
{
  #ifdef DEBUG
    std::cout << "TiDMessageParser10::~TiDMessageParser10 " << std::endl;
  #endif

  //  running_ = 0;
  if(serializer_)
    delete serializer_;
}

//-----------------------------------------------------------------------------

IDMessage TiDMessageParser10::parseMessage (InputStream& stream) const
{
  #ifdef DEBUG
    std::cout << "TiDMessageParser10::parseMessage" << std::endl;
  #endif

  std::string xml_string = stream.readUntil(TiDMessageTags10::XML_END_STRING);

  IDMessage msg;
  serializer_->SetMessage(&msg);
  serializer_->Deserialize(&xml_string);

  return msg;
}

//-----------------------------------------------------------------------------

//string TiDMessageParser10::readVersion (InputStream& stream) const
//{
//  string id_and_version = trim_copy (stream.readLine ( TiDMessageTags10::NEW_LINE_CHAR ));

//  if (id_and_version != TiDMessageTags10::ID_AND_VERSION)
//    throw TiDException(string (
//                         "TiDMessageParser10::readVersion Wrong Message ID and Version. Required \"")
//                       + TiDMessageTags10::ID_AND_VERSION + "\" but was \"" + id_and_version + "\".");

//  return TiDMessageTags10::VERSION;
//}

////-----------------------------------------------------------------------------
//pair<string, string> TiDMessageParser10::readCommandAndParameter (InputStream& stream) const
//{
//  string line = trim_copy (stream.readLine ( TiDMessageTags10::NEW_LINE_CHAR ));
//  return getPair (line);
//}

////-----------------------------------------------------------------------------
//string TiDMessageParser10::readContent (InputStream& stream) const
//{
//  string line = trim_copy (stream.readLine ( TiDMessageTags10::NEW_LINE_CHAR ));
//  if (line.size () == 0)
//    return "";

//  pair<string, string> content_length_pair = getPair (line);
//  if (content_length_pair.first != TiDMessageTags10::CONTENT_LENGTH)
//    throw TiDException (string (
//                          "TiDMessageParser10::readContent: Invalid field. Expected \"")
//                        + TiDMessageTags10::CONTENT_LENGTH + "\" but was \""
//                        + content_length_pair.first + "\"");

//  std::istringstream isstr (content_length_pair.second);
//  size_t length = 0;
//  isstr >> length;
//  if (isstr.fail ())
//    throw TiDException (string(
//                          "TiDMessageParser10::readContent: Error while converting \"")
//                        + content_length_pair.second + "\" into a number.");

//  string empty_line = stream.readLine (TiDMessageTags10::NEW_LINE_CHAR );
//  if (empty_line.size ())
//    throw TiDException (string ("TiDMessageParser10::readContent: Expecting an empty line before content starts."));

//  string content = stream.readString (length);

//  std::cout << "Length: " << length << " -- Content: " << content  << " (" << content.size() << ")" << std::endl;

////  if (content.size () != length)
////    throw TiDException (string ("TiDMessageParser10::readContent: Missing bytes!"));

//  if (content.size () < length)
//    throw TiDException (string ("TiDMessageParser10::readContent: Missing bytes!"));

//  return content;
//}


////-----------------------------------------------------------------------------
//pair<string, string> TiDMessageParser10::getPair (std::string const& str) const
//{
//  size_t delimiter_index = 0;
//  while ((str[delimiter_index] != TiDMessageTags10::COMMAND_DELIMITER) &&
//         delimiter_index < str.size ())
//    delimiter_index++;

//  string tag = str.substr (0, delimiter_index);
//  string value;
//  if (delimiter_index < str.size () - 1)
//    value = trim_copy (str.substr (delimiter_index + 1, str.size () + 1 - delimiter_index));

//  return pair<string, string> (tag, value);
//}


}
