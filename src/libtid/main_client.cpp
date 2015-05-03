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

#include <iostream>
#include <sstream>

#include "tid_client.h"
#include "tid_exceptions.h"
#include <boost/chrono.hpp>

#include <tobiid/IDSerializerRapid.hpp>

using namespace std;
using namespace TiD;

int main(int argc, const char* argv[])
{
  TiDClient* client = 0;

  std::string srv_addr("127.0.0.1");
  int srv_port = 9500;

  string queue ="";

  if(argc == 1)
  {
    cout << "Using default server " << srv_addr << ":" << srv_port << endl;
  }
  else if(argc == 2)
  {
    queue = argv[1];
    cout << "Using SHM " << queue << endl;
  }
  else if(argc == 3)
  {
    srv_addr = argv[1];
    stringstream conv(argv[2]);
    conv >> srv_port;
    cout << "Using server " << srv_addr << ":" << srv_port << endl;
  }

  try
  {
    client = new TiDClient;

    std::vector<IDMessage> messages;

    IDMessage message1(IDMessage::TxtFamilyBiosig, 781);
    message1.SetSource("S1");
    IDMessage message1_end(IDMessage::TxtFamilyBiosig, 781 + 0x8000);
    message1_end.SetSource("S1");

    IDMessage message2(IDMessage::TxtFamilyBiosig, 782);
    IDMessage message2_end(IDMessage::TxtFamilyBiosig, 782+ 0x8000);

    IDMessage message3(IDMessage::TxtFamilyBiosig, 783);
    message3.SetSource("S3");
    IDMessage message3_end(IDMessage::TxtFamilyBiosig, 783+ 0x8000);
    message3_end.SetSource("S3");

//    message1.SetDescription("feedback");
//    message2.SetDescription("feedback");
//    message3.SetDescription("feedback");
//    message1.SetBlockIdx(98);
//    message2.SetBlockIdx(99);
//    message3.SetBlockIdx(100);
    message1.absolute.Tic();
    message2.absolute.Tic();
    message3.absolute.Tic();
    message1_end.absolute.Tic();
    message2_end.absolute.Tic();
    message3_end.absolute.Tic();

    IDMessage message4(IDMessage::TxtFamilyBiosig, 901);
    IDMessage message5(IDMessage::TxtFamilyBiosig, 902);
    IDMessage message6(IDMessage::TxtFamilyBiosig, 903);
    IDMessage message4_end(IDMessage::TxtFamilyBiosig, 901 + 0x8000);
    IDMessage message5_end(IDMessage::TxtFamilyBiosig, 902 + 0x8000);
    IDMessage message6_end(IDMessage::TxtFamilyBiosig, 903 + 0x8000);
//    message4.SetDescription("classifier");
//    message5.SetDescription("classifier");
//    message6.SetDescription("classifier");
//    message4.SetBlockIdx(98);
//    message5.SetBlockIdx(99);
//    message6.SetBlockIdx(100);
    message4.absolute.Tic();
    message5.absolute.Tic();
    message6.absolute.Tic();
    message4.absolute.Tic();
    message5.absolute.Tic();
    message6.absolute.Tic();

    messages.push_back(message1);
    messages.push_back(message1_end);
    messages.push_back(message2);
    messages.push_back(message2_end);
    messages.push_back(message3);
    messages.push_back(message3_end);
    messages.push_back(message4);
    messages.push_back(message4_end);
    messages.push_back(message5);
    messages.push_back(message5_end);
    messages.push_back(message6);
    messages.push_back(message6_end);


    IDMessage left(IDMessage::TxtFamilyBiosig, 912);
    bool left_off = 0;
    IDMessage right(IDMessage::TxtFamilyBiosig, 913);
    bool right_off = 0;


    IDMessage startRecordingMsg(IDMessage::TxtFamilyCustom, 1);
    startRecordingMsg.SetFamily(IDMessage::TxtFamilyCustom);
    startRecordingMsg.SetDescription("StartRecording");

    IDMessage stopRecordingMsg(IDMessage::TxtFamilyCustom, 1);
    stopRecordingMsg.SetFamily(IDMessage::TxtFamilyCustom);
    stopRecordingMsg.SetDescription("StopRecording");


    if(queue.empty())
    {
      client->connect(srv_addr, srv_port);
      client->startReceiving(false);
    }
    else
    {
      client->createSHMMessageQueue(queue);
      client->startReceivingFromSHM();
    }

    string str("a");
    cout << endl << ">>";

    unsigned int msg_count = 0;
    std::string buffer;
    while(cin >> str)
//    while(str != "q")
    {
      if(str == "q" || str == "quit" || str == "exit")
        break;
      else if(str == "s" || str == "send")
      {
        messages[msg_count].absolute.Tic();

        TCTimestamp stamp(messages[msg_count].absolute);
        std::cout << stamp.timestamp.tv_sec << std::endl;
        std::cout << stamp.timestamp.tv_usec << std::endl;

        client->sendMessage( messages.at(msg_count) );
        if(++msg_count == messages.size())
          msg_count = 0;

        cout << ">> " << endl;
      }
      else if(str == "recv" )
      {
        std::vector<IDMessage> msgs;
        client->getLastMessagesContexts(msgs);
        cout << "Received messages: " << msgs.size() << endl;

        IDMessage recv_message;
        IDSerializerRapid recv_serializer;
        for(unsigned int n = 0; n < msgs.size(); n++)
        {
          cout << "Msg Nr: " << n  << endl;
          recv_serializer.SetMessage(&msgs[n]);
          msgs[n].Dump();
        }

        cout << ">> " << endl;
      }
      else if(str == "a" )  // auto send 100 msgs
      {
        for(unsigned int j = 0; j < 100; j++)
        {
          for(unsigned int m = 0; m < 100; m++)
          {
            messages[msg_count].absolute.Tic();

            client->sendMessage( messages[msg_count] );
            if(++msg_count == messages.size())
              msg_count = 0;
            #ifdef WIN32
              Sleep(100);
            #else
              usleep(100000);
            #endif
          }

          std::vector<IDMessage> msgs;
          client->getLastMessagesContexts(msgs);
          cout << "Received messages: " << msgs.size() << endl;

          // IDMessage recv_message;
          //          IDSerializerRapid recv_serializer;
          for(unsigned int n = 0; n < msgs.size(); n++)
          {
            cout << "Msg Nr: " << n  << endl;
            //            recv_serializer.SetMessage(&msgs[n]);
            msgs[n].Dump();
          }
          //          #ifdef WIN32
          //            Sleep(100);
          //          #else
          //            usleep(100000);
          //          #endif
        }
        cout << ">> " << endl;
      }
      else if(str == "start")
      {
        startRecordingMsg.absolute.Tic();
        startRecordingMsg.Dump();
        client->sendMessage( startRecordingMsg );
        cout << ">> " << endl;
      }
      else if(str == "l")
      {
        left.absolute.Tic();
        left.Dump();
        client->sendMessage( left );
        if(!left_off)
        {
          left.SetEvent( left.GetEvent() + 0x8000 );
          left_off = true;
        }
        else
        {
          left.SetEvent( left.GetEvent() - 0x8000 );
          left_off = false;
        }
        cout << ">> " << endl;
      }

      else if(str == "r")
      {
        right.absolute.Tic();
        right.Dump();
        client->sendMessage( right );
        if(!right_off)
        {
          right.SetEvent( right.GetEvent() + 0x8000 );
          right_off = true;
        }
        else
        {
          right.SetEvent( right.GetEvent() - 0x8000 );
          right_off = false;
        }
        cout << ">> " << endl;
      }

      else if(str == "stop")
      {
        stopRecordingMsg.absolute.Tic();
        stopRecordingMsg.Dump();
        client->sendMessage( stopRecordingMsg );
        cout << ">> " << endl;
      }
      else
        cout << "Command '" << str << "' not recognized!" << endl << ">>";
    }

    if(queue.empty())
    {
      client->stopReceiving();
    }
    else
    {
      client->stopReceivingFromSHM();
      client->closeMsgQueue();
    }

  }
//  catch(TiDException& e)
//  {
//    std::cerr << "TiDException caugth: " << e.what() << std::endl;
//  }
  catch(std::exception& e)
  {
    std::cerr << "Exception caugth: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cerr << "Unknown exception caugth!" << std::endl;
  }

    delete client;

  return 0;
}
