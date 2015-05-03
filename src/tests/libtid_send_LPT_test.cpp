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


#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include "UnitTest++/UnitTest++.h"

#include "tobiid/IDMessage.hpp"

#include "tid_message_vector_builder.h"

#include "LPT_tid_server.h"
#include "LPT_tid_client.h"
#include "timing_values.h"

extern unsigned int NR_TID_MESSAGES;
extern unsigned int NR_CLIENTS;


//-------------------------------------------------------------------------------------------------

TEST(libTiDLPTTimingTest)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(200));
  std::cout << "Running libTiD LPT test" << std::endl;
  #ifdef SKIP_LIBTID_LPT_TEST
    std::cout << "  --> skipping !!" << std::endl;
    return;
  #endif


  #ifdef ACT_AS_TID_SERVER
  std::cout << "   --> as Server" << std::endl;

  try
  {
    TiD::LPTTiDServer test_server;
    test_server.reserveNrOfMsgs(NR_TID_MESSAGES);
    test_server.bind (9001);
    test_server.start();

    std::string str;
    std::cout << std::endl << ">>";

    while(cin >> str)
    {
      if(str == "q" || str == "quit" || str == "exit")
        break;
      else
        cout << "Command '" << str << "' not recognized!" << endl << ">>";
    }

    test_server.stop();
  }

  #else
  std::cout << "   --> as Client (this test is not intended to run automatically)" << std::endl << std::endl;

  std::cout << "   Waiting for start command ..." << std::endl;

  std::string str;
  std::cout << std::endl << ">>";
  while(std::cin >> str)
  {
    if(str == "s" || str == "start" || str == "run")
      break;
    else if(str == "q" || str == "quit" || str == "stop")
      return;
    else
      std::cout << "Command '" << str << "' not recognized!" << std::endl << ">>";
  }


  TiDMessageVectorBuilder msg_builder;

  std::vector<unsigned int> description_str_lengths;
  description_str_lengths.push_back(5);
  description_str_lengths.push_back(20);
  description_str_lengths.push_back(100);

  std::vector<unsigned int> nr_clients;
  if(NR_CLIENTS == 0)
  {
    nr_clients.push_back(0);
    nr_clients.push_back(10);
  }
  else
    nr_clients.push_back(NR_CLIENTS);

  std::vector<TiD::TiDClient*> clients_vec;

  try
  {

    TiD::LPTTiDClient send_client;

    for(unsigned int cl_ind = 0; cl_ind < nr_clients.size(); cl_ind++ )
    {
      send_client.connect("192.168.1.11",9001);
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      std::cout << "  ... iteration " << cl_ind+1 << " from " << nr_clients.size() << std::endl;
      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec.push_back(new TiD::TiDClient );
        clients_vec[n]->connect("192.168.1.11",9001);
        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
      }

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

      TiD::LPTTiDClient recv_client;
      recv_client.connect("192.168.1.11",9001);
      recv_client.startReceiving(0);
      boost::this_thread::sleep(boost::posix_time::milliseconds(100));


      for(unsigned int k= 0; k < description_str_lengths.size(); k++ )
      {
        std::cout << "    ... sub-iteration " << k+1 << " from " << description_str_lengths.size();
        std::cout << " (will take at least " << SLEEP_TIME_BETWEEN_MSGS_REMOTE * NR_TID_MESSAGES << ")" << std::endl;

        std::cout << std::endl << "Press 'c' to continue ..." << std::endl;

        while(std::cin >> str)
        {
          if(str == "c" || str == "continue" )
            break;
          else
            std::cout << "Command '" << str << "' not recognized!" << std::endl << ">>";
        }

        msg_builder.generateMsgsVector(NR_TID_MESSAGES, description_str_lengths[k]);
        std::vector<IDMessage>& msgs_vec = msg_builder.getMessagesVector();

        recv_client.clearMessages();
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));

        boost::posix_time::microsec no_msgs_available_sleep_time = boost::posix_time::microseconds(100);

        std::vector<IDMessage> recv_msgs;

        for(unsigned int n = 0; n < msgs_vec.size(); n++ )
        {
          send_client.sendLPTMessage( msgs_vec[n] );
          boost::this_thread::sleep(SLEEP_TIME_BETWEEN_MSGS_REMOTE);

          while( !recv_client.newMessagesAvailable())
            boost::this_thread::sleep(no_msgs_available_sleep_time);

          recv_client.getLastMessagesContexts(recv_msgs);
          if( recv_msgs.size() != 1 )
          {
            std::cerr << "   #####  ERROR -- Something went wrong, received more than 1 TiD msg! (";
            std::cerr << recv_msgs.size() << " msgs -- available: " << recv_client.newMessagesAvailable() << ") #####" << std::endl;
            n--;
          }
          recv_msgs.clear();
          recv_client.clearMessages();
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec[n]->disconnect();
        delete clients_vec[n];
      }
      clients_vec.clear();
      recv_client.stopReceiving();
      recv_client.disconnect();
      send_client.disconnect();

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    }

  }
  catch(std::exception& e)
  {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }

  std::cout << std::endl << std::endl;

  #endif

}
