//
// This file is part of libGDF.
//
// libGDF is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// libGDF is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with libGDF.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2011 Christian Breitwieser

#include "include/InstanceManager.h"
#include "include/Commands.h"
#include "include/mxStructAccess.h"

#include "../src/libtid/sdl_tid_client.h"
#include "../src/tobiid/IDMessage.hpp"


#include <mex.h>

using namespace std;

mxArray* createMxIDStruct(IDMessage* msg);
mxArray* createMxIDStruct(std::vector<IDMessage>& msg_vector);

void fillIDMessage(IDMessage& msg, const mxArray* mxIDStruct, unsigned int index);
void setmxIDStructFields(mxArray* mxIDStruct, IDMessage* msg, unsigned int index);

// ===================================================================================================
//      Object Interface to mex
// ===================================================================================================

class CmexObject
{
  public:
    CmexObject( );

    ~CmexObject( );

    void execute( size_t nlhs, mxArray *plhs[], size_t nrhs, const mxArray *prhs[] );

    static CmexObject &getInstance( ) { return instance; }

    InstanceManager< TiD::SDLTiDClient > clients;
    CommandManager commands;

  private:
    static CmexObject instance;
};

// ===================================================================================================
//      Commands
// ===================================================================================================

class CMD_connect : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_disconnect : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_startReceiving : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_stopReceiving : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_isReceiving : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_isConnected : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_sendMessage : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_asyncSendMessage : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_newMessagesAvailable : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_getLastMessagesContexts : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_clearMessages : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };
class CMD_wait4NewTiDMessage : public Command { void execute( mxArray *plhs[], const mxArray *prhs[] ); };

// ===================================================================================================
//      mexFunction
// ===================================================================================================

CmexObject CmexObject::instance;

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
  CmexObject::getInstance( ).execute( nlhs, plhs, nrhs, prhs );
}

// ===================================================================================================
//      CmexObject Definition
// ===================================================================================================

CmexObject::CmexObject( )
{
  commands.registerCommand( "Connect",            new CMD_connect( ), 1, 2 );
  commands.registerCommand( "Disconnect",         new CMD_disconnect( ), 0, 1 );
  commands.registerCommand( "StartReceiving",     new CMD_startReceiving( ), 0, 1 );
  commands.registerCommand( "StopReceiving",      new CMD_stopReceiving( ), 0, 1 );
  commands.registerCommand( "IsConnected",        new CMD_isConnected( ), 1, 1 );
  commands.registerCommand( "IsReceiving",        new CMD_isReceiving( ), 1, 1 );
  commands.registerCommand( "SendMessage",        new CMD_sendMessage( ), 0, 2 );
  commands.registerCommand( "AsyncSendMessage",   new CMD_asyncSendMessage( ), 0, 2 );
  commands.registerCommand( "ClearMessages",      new CMD_clearMessages( ), 0, 1 );
  commands.registerCommand( "Wait4NewTiDMessage", new CMD_wait4NewTiDMessage, 1, 1 );
  commands.registerCommand( "NewMessagesAvailable",     new CMD_newMessagesAvailable( ), 1, 1 );
  commands.registerCommand( "GetLastMessagesContexts",  new CMD_getLastMessagesContexts( ), 1, 1 );
}

//-------------------------------------------------------------------------------------------------

CmexObject::~CmexObject( )
{
}

//-------------------------------------------------------------------------------------------------

void CmexObject::execute( size_t nlhs, mxArray *plhs[], size_t nrhs, const mxArray *prhs[] )
{
  if( nrhs < 1 )
    throw std::invalid_argument( "Expecting at least one argument." );

  if( !mxIsChar(prhs[0]) )
    throw std::invalid_argument( "First argument must be a command (string)." );

  char *command;
  command = new char[32];
  if( mxGetString( prhs[0], command, 32 ) != 0 )
    throw std::invalid_argument( "Unknown problem with command string." );

  commands.execute( command, nlhs, plhs, nrhs-1, prhs+1 );
  delete[] command;
}

// ===================================================================================================
//      Command Definitions
// ===================================================================================================

void CMD_connect::execute( mxArray *plhs[], const mxArray *prhs[] )
{
 size_t handle = CmexObject::getInstance().clients.newInstance( );

 TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );
 std::string ip = mx::getString( prhs[0] );
 unsigned int port = mx::getNumeric<unsigned int>( prhs[1] );

 client->connect(ip, port);


 plhs[0] = mxCreateNumericMatrix( 1, 1, mxUINT64_CLASS, mxREAL );
 *reinterpret_cast<size_t*>(mxGetData( plhs[0] )) = handle;
}

//-----------------------------------------------------------------------------

void CMD_disconnect::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  printf("%d", handle);
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );
  client->disconnect();
}

//-----------------------------------------------------------------------------

void CMD_startReceiving::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );
  client->startReceiving();
}

//-----------------------------------------------------------------------------

void CMD_stopReceiving::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );
  client->stopReceiving();
}

//-----------------------------------------------------------------------------

void CMD_isReceiving::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );

  bool is_receiving = client->receiving();

  plhs[0] = mxCreateNumericMatrix( 1, 1, mxUINT8_CLASS, mxREAL);
  *reinterpret_cast<bool*>(mxGetData( plhs[0] )) = is_receiving;
}

//-----------------------------------------------------------------------------

void CMD_isConnected::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );

  bool is_connected = client->connected();

  plhs[0] = mxCreateNumericMatrix( 1, 1, mxUINT8_CLASS, mxREAL);
  *reinterpret_cast<bool*>(mxGetData( plhs[0] )) = is_connected;
}

//-----------------------------------------------------------------------------

void CMD_sendMessage::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );

  const mxArray* mxIDStruct = prhs[1];
  if(mxGetN(mxIDStruct) != 1)
    throw mx::Exception("Error -- Mesages vector has to be a row vector!");

  for(unsigned int n = 0; n < mxGetM(mxIDStruct); n++)
  {
    IDMessage msg;
    fillIDMessage(msg, mxIDStruct, n);
    client->sendMessage(msg);
  }
}

//-----------------------------------------------------------------------------

void CMD_asyncSendMessage::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );

  const mxArray* mxIDStruct = prhs[1];
  if(mxGetN(mxIDStruct) != 1)
    throw mx::Exception("Error -- Mesages vector has to be a row vector!");

  for(unsigned int n = 0; n < mxGetM(mxIDStruct); n++)
  {
    IDMessage msg;
    fillIDMessage(msg, mxIDStruct, n);
    client->asyncSendMessage(msg);
  }
}

//-----------------------------------------------------------------------------

void CMD_clearMessages::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );
  client->clearMessages();
}

//-----------------------------------------------------------------------------

void CMD_wait4NewTiDMessage::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );

  IDMessage msg = client->wait4NewTiDMessage();
  plhs[0] = createMxIDStruct(&msg);
}

//-----------------------------------------------------------------------------

void CMD_newMessagesAvailable::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );
  bool new_messages_available = client->newMessagesAvailable();

  plhs[0] = mxCreateNumericMatrix( 1, 1, mxUINT8_CLASS, mxREAL);
  *reinterpret_cast<bool*>(mxGetData( plhs[0] )) = new_messages_available;
}

//-----------------------------------------------------------------------------

void CMD_getLastMessagesContexts::execute( mxArray *plhs[], const mxArray *prhs[] )
{
  size_t handle = mx::getNumeric<size_t>( prhs[0] );
  TiD::SDLTiDClient *client = CmexObject::getInstance().clients.get( handle );

  std::vector<IDMessage> msgs;
  client->getLastMessagesContexts(msgs);
  plhs[0] = createMxIDStruct(msgs);
}

//-----------------------------------------------------------------------------

mxArray* createMxIDStruct(IDMessage* msg)
{
  mxArray* mxIDStruct = mxCreateStructMatrix( 1, 1, 0, NULL );
  mxAddField( mxIDStruct, "family" );
  mxAddField( mxIDStruct, "event" );
  mxAddField( mxIDStruct, "description" );
  mxAddField( mxIDStruct, "source" );
  mxAddField( mxIDStruct, "value" );
  setmxIDStructFields(mxIDStruct, msg, 1);
  return mxIDStruct;
}

//-----------------------------------------------------------------------------

mxArray* createMxIDStruct(std::vector<IDMessage>& msg_vector)
{
  mxArray* mxIDStruct = mxCreateStructMatrix( msg_vector.size(), 1, 0, NULL );
  mxAddField( mxIDStruct, "family" );
  mxAddField( mxIDStruct, "event" );
  mxAddField( mxIDStruct, "description" );
  mxAddField( mxIDStruct, "source" );
  mxAddField( mxIDStruct, "value" );

  for(unsigned int n= 0; n< msg_vector.size(); n++)
    setmxIDStructFields(mxIDStruct, &(msg_vector[n]), n+1);

  return mxIDStruct;
}

//-----------------------------------------------------------------------------

void setmxIDStructFields(mxArray* mxIDStruct, IDMessage* msg, unsigned int index)
{
  mx::setFieldString(mxIDStruct, msg->GetFamily(),      "family", index);
  mx::setFieldString(mxIDStruct, msg->GetSource(),      "source", index);
  mx::setFieldString(mxIDStruct, msg->GetDescription(), "description", index);

  mx::setFieldNumeric(mxIDStruct, boost::numeric_cast<int64_t>(msg->GetEvent()), "event", index);
  mx::setFieldNumeric(mxIDStruct, msg->GetValue(), "value", index);
}

//-----------------------------------------------------------------------------

void fillIDMessage(IDMessage& msg, const mxArray* mxIDStruct, unsigned int index)
{


  msg.SetEvent( mx::getFieldNumeric<int>(mxIDStruct, "event", index));
  msg.SetValue( mx::getFieldNumeric<float>(mxIDStruct, "value", index) );

  msg.SetFamily( mx::getFieldString(mxIDStruct, "family", index) );
  msg.SetSource( mx::getFieldString(mxIDStruct, "source", index) );
  msg.SetDescription( mx::getFieldString(mxIDStruct, "description", index) );
}

//-----------------------------------------------------------------------------

