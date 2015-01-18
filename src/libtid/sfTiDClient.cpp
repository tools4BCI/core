// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// It is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this file.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2011 Christian Breitwieser


#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfTiDClient

#include "simstruc.h"

#include <iostream>
#include <utility>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "sdl_tid_client.h"
#include "tid_exceptions.h"
#include "tobiid/IDSerializerRapid.hpp"

using boost::algorithm::to_lower;

enum
{
  TID_SERVER_IP_PARAM = 0,
  TID_SERVER_PORT_PARAM,
  BIOSIG_EV_CODES_TXT_PARAM,
  NUM_FCN_PARAMETERS	// must be last
};

enum
{
  EVENT_IN_PORT = 0,
  BLOCK_NR_IN_PORT,
  TIMESTAMP_IN_PORT,
  NUM_INPUT_PORTS     // must be last
};

enum
{
  NUM_OUTPUT_PORTS = 0 // must be last
};

enum
{
  WP_TID_CLIENT = 0,
  WP_TID_MESSAGE,
  WP_TID_DUMMY_MESSAGES,
  WP_TID_SERIALIZER,
  WP_EVENTCODES_MAP,
  NUM_WORK_POINTERS	// must be last
};

//---------------------------------------------------------------------------------------

void checkFcnParameters(SimStruct *S);
void parse_eventcodes(std::ifstream& ifs, std::map<unsigned int, std::string>* eventcodes_map);

//---------------------------------------------------------------------------------------

struct hexint
{
  operator unsigned int()
  {
    return value;
  }
  unsigned int value;
};

//---------------------------------------------------------------------------------------

inline std::istream &operator>>(std::istream &in, hexint &rhs)
{
  std::ios::fmtflags flags=in.flags();

  in.unsetf(std::ios::oct);
  in.unsetf(std::ios::dec);
  in.unsetf(std::ios::hex);
  unsigned int temp=0;
  in >> temp;
  in.setf(flags);
  if(in.bad())
    throw boost::bad_lexical_cast();

  rhs.value=temp;
  return in;
}

//---------------------------------------------------------------------------------------

static void mdlInitializeSizes(SimStruct *S)
{
  #ifdef MEXDEBUG
    mexPrintf("Initialising sfTiDClient ...\n");
  #endif

  ssSetNumSFcnParams(S,  NUM_FCN_PARAMETERS);
  if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S))
    return;
  checkFcnParameters(S);


  ssSetNumPWork(S, NUM_WORK_POINTERS);

  if(!ssSetNumInputPorts(S, NUM_INPUT_PORTS))
    return;
  if(!ssSetNumOutputPorts(S, NUM_OUTPUT_PORTS))
    return;

  ssSetInputPortWidth(S, EVENT_IN_PORT, DYNAMICALLY_SIZED);
  ssSetInputPortSampleTime(S, EVENT_IN_PORT, INHERITED_SAMPLE_TIME);
  ssSetInputPortDirectFeedThrough(S, EVENT_IN_PORT, 1);
  ssSetInputPortDataType( S, EVENT_IN_PORT, DYNAMICALLY_TYPED );
  ssSetInputPortRequiredContiguous(S, EVENT_IN_PORT, true);

  ssSetInputPortWidth(S, BLOCK_NR_IN_PORT, 1);
  ssSetInputPortSampleTime(S, BLOCK_NR_IN_PORT, INHERITED_SAMPLE_TIME);
  ssSetInputPortDirectFeedThrough(S, BLOCK_NR_IN_PORT, 1);
  ssSetInputPortDataType( S, BLOCK_NR_IN_PORT, DYNAMICALLY_TYPED );
  ssSetInputPortRequiredContiguous(S, BLOCK_NR_IN_PORT, true);

  ssSetInputPortWidth(S, TIMESTAMP_IN_PORT, 1);
  ssSetInputPortSampleTime(S, TIMESTAMP_IN_PORT, INHERITED_SAMPLE_TIME);
  ssSetInputPortDirectFeedThrough(S, TIMESTAMP_IN_PORT, 1);
  ssSetInputPortDataType( S, TIMESTAMP_IN_PORT, DYNAMICALLY_TYPED );
  ssSetInputPortRequiredContiguous(S, TIMESTAMP_IN_PORT, true);

  ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);
  ssSetNumSampleTimes( S, -1 );

  #ifdef MEXDEBUG
    mexPrintf(" ... sfTiDClient done\n");
  #endif
}

//---------------------------------------------------------------------------------------

static void mdlInitializeSampleTimes(SimStruct *S)
{}

//---------------------------------------------------------------------------------------

#define MDL_START
static void mdlStart(SimStruct *S)
{

  unsigned int port = mxGetScalar( ssGetSFcnParam( S, TID_SERVER_PORT_PARAM ) );

  TiD::SDLTiDClient* client = new TiD::SDLTiDClient;
  ssGetPWork(S)[WP_TID_CLIENT] = client;

  IDMessage* message = new IDMessage;
  ssGetPWork(S)[WP_TID_MESSAGE] = message;

  std::vector<IDMessage>* dummy_messages = new std::vector<IDMessage>;
  ssGetPWork(S)[WP_TID_DUMMY_MESSAGES] = dummy_messages;
  dummy_messages->reserve(20);

  IDSerializerRapid* tid_serializer = new IDSerializerRapid();
  ssGetPWork(S)[WP_TID_SERIALIZER] = tid_serializer;
  tid_serializer->SetMessage( message );

  std::string tid_server_ip(mxArrayToString( ssGetSFcnParam( S, TID_SERVER_IP_PARAM ) ));
  to_lower(tid_server_ip);

  std::string event_codes_path(mxArrayToString( ssGetSFcnParam( S, BIOSIG_EV_CODES_TXT_PARAM ) ));

  boost::filesystem::path p( event_codes_path );
  std::map<unsigned int, std::string>* eventcodes_map = new std::map<unsigned int, std::string>;
  ssGetPWork(S)[WP_EVENTCODES_MAP] = eventcodes_map;

  if(exists(p))
  {
    if(is_regular_file(p))
    {
      std::ifstream ifs ( event_codes_path.c_str() , std::ios_base::in );
      parse_eventcodes(ifs, eventcodes_map);
      ifs.close();
    }
    else
      throw(std::invalid_argument( p.string() + "exists, but is not a regular file!"));
  }
  else
    throw(std::invalid_argument( p.string() + "does not exist!"));

  client->connect( tid_server_ip, port);
  client->startReceiving();
}

//---------------------------------------------------------------------------------------

static void mdlOutputs(SimStruct *S, int_T tid)
{
  int width = ssGetInputPortWidth(S, EVENT_IN_PORT);
  if(!width)
    return;

  width = ssGetInputPortWidth(S, BLOCK_NR_IN_PORT);
  if(!width)
    return;

  width = ssGetInputPortWidth(S, TIMESTAMP_IN_PORT);
  if(!width)
    return;

  const real_T* events = ssGetInputPortRealSignal( S, EVENT_IN_PORT);
  const real_T* block_nr = ssGetInputPortRealSignal( S, BLOCK_NR_IN_PORT);
  const real_T* timestamp = ssGetInputPortRealSignal( S, TIMESTAMP_IN_PORT);

  TiD::SDLTiDClient* tid_client = static_cast<TiD::SDLTiDClient*>( ssGetPWork(S)[WP_TID_CLIENT] );
  std::vector<IDMessage>* dummy_msgs =
    static_cast<std::vector<IDMessage>*>( ssGetPWork(S)[WP_TID_DUMMY_MESSAGES] );

  if(events[0])
  {
    for(int n = 1; n <= events[0]; n++)
    {

      IDMessage* message = static_cast<IDMessage*>( ssGetPWork(S)[WP_TID_MESSAGE] );
      IDSerializerRapid* tid_serializer =
        static_cast<IDSerializerRapid*>( ssGetPWork(S)[WP_TID_SERIALIZER] );

      std::map<unsigned int, std::string>* eventcodes_map =
        static_cast<std::map<unsigned int, std::string>*>( ssGetPWork(S)[WP_EVENTCODES_MAP] );

      message->SetFamilyType(IDMessage::FamilyBiosig);
      message->SetEvent(events[n]);
      if(  ssGetInputPortConnected(S, BLOCK_NR_IN_PORT) )
        message->SetBlockIdx( boost::lexical_cast<int>( block_nr[0]+1 ) );
      else
        message->SetBlockIdx( IDMessage::BlockIdxUnset );

      if(  ssGetInputPortConnected(S, TIMESTAMP_IN_PORT) )
      {
        message->relative.timestamp.tv_sec  =
          floor(static_cast<boost::uint64_t>(timestamp[0])/1000000) ;
        message->relative.timestamp.tv_usec =
          static_cast<boost::uint64_t>(timestamp[0]) - message->relative.timestamp.tv_sec*1000000;
      }

      // mexPrintf(" Sent an event with block nr: %d\n", block_nr[0]+1 );

      std::map<unsigned int, std::string>::iterator it( eventcodes_map->find(events[n]) );
      if(it != eventcodes_map->end() )
        message->SetDescription( it->second );
      else
        message->SetDescription( "unknown" );

      std::string buffer;
      message->absolute.Tic();
      tid_serializer->Serialize(&buffer);
      tid_client->sendMessage( buffer );
      buffer.clear();
    }
  }
}

//---------------------------------------------------------------------------------------

static void mdlTerminate(SimStruct *S)
{
  TiD::SDLTiDClient* tid_client = static_cast<TiD::SDLTiDClient*>( ssGetPWork(S)[WP_TID_CLIENT] );
  if(tid_client)
  {
    tid_client->disconnect();
    delete tid_client;
  }

  std::vector<IDMessage>* dummy_msgs =
    static_cast<std::vector<IDMessage>*>( ssGetPWork(S)[WP_TID_DUMMY_MESSAGES] );
  if(dummy_msgs)
    delete dummy_msgs;

  IDMessage* message = static_cast<IDMessage*>( ssGetPWork(S)[WP_TID_MESSAGE] );
  if(message)
    delete message;

  IDSerializerRapid* tid_serializer =
    static_cast<IDSerializerRapid*>( ssGetPWork(S)[WP_TID_SERIALIZER] );
  if(tid_serializer)
    delete tid_serializer;

  std::map<unsigned int, std::string>* eventcodes_map =
    static_cast<std::map<unsigned int, std::string>*>( ssGetPWork(S)[WP_EVENTCODES_MAP] );
  if(eventcodes_map)
    delete eventcodes_map;
}

//---------------------------------------------------------------------------------------

void checkFcnParameters(SimStruct *S)
{
  if(mxIsEmpty(ssGetSFcnParam(S,TID_SERVER_PORT_PARAM))
    || !mxIsNumeric( ssGetSFcnParam(S,TID_SERVER_PORT_PARAM))
    || (mxGetM(ssGetSFcnParam(S,TID_SERVER_PORT_PARAM)) != 1)
    || (mxGetN(ssGetSFcnParam(S,TID_SERVER_PORT_PARAM)) != 1) )
  {
    ssSetErrorStatus(S,"Parameter 1 must be a numerical value representing the port of the TiD Server!");
    return;
  }

  if(mxIsEmpty(ssGetSFcnParam(S,BIOSIG_EV_CODES_TXT_PARAM))
    || !mxIsChar( ssGetSFcnParam(S,BIOSIG_EV_CODES_TXT_PARAM)))
  {
    ssSetErrorStatus(S,"Parameter 2 must be a string representing the IP of the TiD Server!");
    return;
  }

  if(mxIsEmpty(ssGetSFcnParam(S,BIOSIG_EV_CODES_TXT_PARAM))
    || !mxIsChar( ssGetSFcnParam(S,BIOSIG_EV_CODES_TXT_PARAM)))
  {
    ssSetErrorStatus(S,"Parameter 3 must be a string representing the path to the BioSig 'eventcodes.txt' file!");
    return;
  }
}

//---------------------------------------------------------------------------------------

void parse_eventcodes(std::ifstream& ifs, std::map<unsigned int, std::string>* eventcodes_map)
{
  std::string line;

  while (ifs.good())
  {
    getline(ifs, line);
    if(line.size() && line[0] != '#')
    {
      unsigned int code = 0;
      size_t pos1 = line.find_first_of('\t');
      try
      {
        code = boost::lexical_cast<hexint>(line.substr(0, pos1));
      }
      catch(boost::bad_lexical_cast&)
      {
        std::cerr << "Could not convert '" << line << "' into a hexadecimal number and a string";
        std::cerr << "-- Please check if a 'tab' was used after the hexadecimal number!" << std::endl;
      }
      eventcodes_map->insert( std::make_pair(code, line.substr(pos1+1)) );
    }
  }
}

//---------------------------------------------------------------------------------------

// Required S-function trailer
#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
