

#include <iostream>
#include <sstream>

#include "tid_client.h"
#include "tid_exceptions.h"

#include <tobiid/IDSerializerRapid.hpp>


using namespace std;
using namespace TiD;

int main()
{
  try
  {
    TiDClient client;

    std::vector<IDMessage> messages;

    IDMessage message1(IDMessage::FamilyBiosig, 781);
    IDMessage message2(IDMessage::FamilyBiosig, 782);
    IDMessage message3(IDMessage::FamilyBiosig, 783);
    message1.SetDescription("feedback");
    message2.SetDescription("feedback");
    message3.SetDescription("feedback");
    message1.SetBlockIdx(98);
    message2.SetBlockIdx(99);
    message3.SetBlockIdx(100);

    IDMessage message4(IDMessage::FamilyBiosig, 901);
    IDMessage message5(IDMessage::FamilyBiosig, 902);
    IDMessage message6(IDMessage::FamilyBiosig, 903);
    message4.SetDescription("classifier");
    message5.SetDescription("classifier");
    message6.SetDescription("classifier");
    message4.SetBlockIdx(98);
    message5.SetBlockIdx(99);
    message6.SetBlockIdx(100);

    messages.push_back(message1);
    messages.push_back(message2);
    messages.push_back(message3);
    messages.push_back(message4);
    messages.push_back(message5);
    messages.push_back(message6);


    client.connect(std::string("10.0.2.15"), 9001);
    client.startReceiving(true);

    string str;
    cout << endl << ">>";

    unsigned int msg_count = 0;
    std::string buffer;
    while(cin >> str)
    {
      if(str == "q" || str == "quit" || str == "exit")
        break;
      else if(str == "s" || str == "send")
      {
        IDSerializerRapid serializer( &messages.at(msg_count) );
        //        messages.at(msg_count).Dump();
        serializer.Serialize(&buffer);

        client.sendMessage( buffer );
        buffer.clear();
        if(++msg_count == messages.size())
          msg_count = 0;
      }
      else if(str == "r" )
      {
        std::vector<IDMessage> msgs;
        client.getLastMessagesContexts(msgs);
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
      else
        cout << "Command '" << str << "' not recognized!" << endl << ">>";
    }

    client.stopReceiving();
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

  return 0;
}
