

#include <iostream>
#include <sstream>

#include "tid_client.h"
#include "sdl_tid_client.h"
#include "tid_exceptions.h"

#include <tobiid/IDSerializerRapid.hpp>


using namespace std;
using namespace TiD;

int main()
{
  TiDClientBase* client = 0;

  try
  {
    client = new TiDClient;

    std::vector<IDMessage> messages;

    IDMessage message1(IDMessage::FamilyBiosig, 781);
    IDMessage message2(IDMessage::FamilyBiosig, 782);
    IDMessage message3(IDMessage::FamilyBiosig, 783);
//    message1.SetDescription("feedback");
//    message2.SetDescription("feedback");
//    message3.SetDescription("feedback");
    message1.SetBlockIdx(98);
    message2.SetBlockIdx(99);
    message3.SetBlockIdx(100);
    message1.absolute.Tic();
    message2.absolute.Tic();
    message2.absolute.Tic();

    IDMessage message4(IDMessage::FamilyBiosig, 901);
    IDMessage message5(IDMessage::FamilyBiosig, 902);
    IDMessage message6(IDMessage::FamilyBiosig, 903);
//    message4.SetDescription("classifier");
//    message5.SetDescription("classifier");
//    message6.SetDescription("classifier");
    message4.SetBlockIdx(98);
    message5.SetBlockIdx(99);
    message6.SetBlockIdx(100);
    message4.absolute.Tic();
    message5.absolute.Tic();
    message6.absolute.Tic();

    messages.push_back(message1);
    messages.push_back(message2);
    messages.push_back(message3);
    messages.push_back(message4);
    messages.push_back(message5);
    messages.push_back(message6);


    IDMessage startRecordingMsg(IDMessage::FamilyCustom, 1);
    startRecordingMsg.SetFamilyType(IDMessage::TxtFamilyCustom);
    startRecordingMsg.SetDescription("StartRecording");

    IDMessage stopRecordingMsg(IDMessage::FamilyCustom, 1);
    stopRecordingMsg.SetFamilyType(IDMessage::TxtFamilyCustom);
    stopRecordingMsg.SetDescription("StopRecording");


    client->connect(std::string("127.0.0.1"), 9100);
    client->startReceiving(false);

    string str("a");
    cout << endl << ">>";

    unsigned int msg_count = 0;
    std::string buffer;
    //while(cin >> str)
    while(str != "q")
    {
      if(str == "q" || str == "quit" || str == "exit")
        break;
      else if(str == "s" || str == "send")
      {

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
