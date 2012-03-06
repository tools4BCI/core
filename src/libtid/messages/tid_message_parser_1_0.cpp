#include "tid_message_parser_1_0.h"
#include "tid_message_tags_1_0.h"
//#include "tid_exceptions.h"

#include <assert.h>

#include <iostream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/current_function.hpp>

#include <tobiid/IDMessage.hpp>

#ifdef TIMING_TEST
  #include <boost/chrono.hpp>
#endif

using std::string;
using std::pair;
using boost::algorithm::trim_copy;

namespace TiD
{

//-----------------------------------------------------------------------------

TiDMessageParser10::TiDMessageParser10()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  serializer_ = new IDSerializerRapid();
  xml_string_.reserve(2048);
}

//-----------------------------------------------------------------------------

TiDMessageParser10::~TiDMessageParser10()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(serializer_)
    delete serializer_;
}

//-----------------------------------------------------------------------------

void TiDMessageParser10::parseMessage (IDMessage* msg, InputStream* stream)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  xml_string_.clear();
  stream->readUntil(TiDMessageTags10::XML_END_STRING, xml_string_);

  try
  {
    serializer_->SetMessage(msg);
    serializer_->Deserialize(&xml_string_);
  }
  catch(...)
  {
    std::cerr << "Exception caught@ " << BOOST_CURRENT_FUNCTION <<  std::endl;
    std::cerr << "  ...  "  << xml_string_ <<  std::endl<< std::flush;
    throw;
  }

}

//-----------------------------------------------------------------------------

}
