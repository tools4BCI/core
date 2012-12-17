

#include <iostream>
#include <sstream>

#include "sdl_tid_client.h"
#include "tid_exceptions.h"
#include <boost/chrono.hpp>

#include <tobiid/IDSerializerRapid.hpp>

using namespace std;
using namespace TiD;

int main(int argc, const char* argv[])
{
  TiDClientBase* client = 0;

  std::string srv_addr("127.0.0.1");
  int srv_port = 9500;

  if(argc == 1)
  {
    cout << "Using default server " << srv_addr << ":" << srv_port << endl;
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
    client = new SDLTiDClient;

    std::vector<IDMessage> messages;

    IDMessage message1(IDMessage::FamilyBiosig, 781);
    IDMessage message1_end(IDMessage::FamilyBiosig, 781 + 0x8000);
    IDMessage message2(IDMessage::FamilyBiosig, 782);
    IDMessage message2_end(IDMessage::FamilyBiosig, 782+ 0x8000);
    IDMessage message3(IDMessage::FamilyBiosig, 783);
    IDMessage message3_end(IDMessage::FamilyBiosig, 783+ 0x8000);
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

    IDMessage message4(IDMessage::FamilyBiosig, 901);
    IDMessage message5(IDMessage::FamilyBiosig, 902);
    IDMessage message6(IDMessage::FamilyBiosig, 903);
    IDMessage message4_end(IDMessage::FamilyBiosig, 901 + 0x8000);
    IDMessage message5_end(IDMessage::FamilyBiosig, 902 + 0x8000);
    IDMessage message6_end(IDMessage::FamilyBiosig, 903 + 0x8000);
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


    IDMessage startRecordingMsg(IDMessage::FamilyCustom, 1);
    startRecordingMsg.SetFamilyType(IDMessage::TxtFamilyCustom);
    startRecordingMsg.SetDescription("StartRecording");

    IDMessage stopRecordingMsg(IDMessage::FamilyCustom, 1);
    stopRecordingMsg.SetFamilyType(IDMessage::TxtFamilyCustom);
    stopRecordingMsg.SetDescription("StopRecording");


    client->connect(srv_addr, srv_port);
    client->startReceiving(false);

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
      else if(str == "r" )
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

    client->stopReceiving();
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
