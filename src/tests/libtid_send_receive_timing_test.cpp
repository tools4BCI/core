
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

#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include "UnitTest++/UnitTest++.h"

#include "tobiid/IDMessage.hpp"

#include "tid_message_vector_builder.h"
#include "statistics.h"
#include "timing_test_helpers.h"
#include "timing_values.h"

#include "timed_tid_server.h"
#include "timed_tid_client.h"

extern unsigned int NR_TID_MESSAGES;
extern unsigned int STATISTICS_WINDOW_SIZE;
extern unsigned int NR_CLIENTS;
//extern boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS;
//extern boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS_REMOTE;

using std::fstream;

//-------------------------------------------------------------------------------------------------

TEST(libTiDLocalHostSendReceiveTimingTest)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(200));
  std::cout << "Running libTiD localhost send and receive timing test" << std::endl;
  #ifdef SKIP_LIBTID_LOCALHOST_SEND_RECEIVE_TEST
    std::cout << "  --> skipping !!" << std::endl;
    return;
  #endif

  TiDMessageVectorBuilder msg_builder;
  tobiss::Statistics  stat(true, STATISTICS_WINDOW_SIZE );
  std::fstream file_stream;
  file_stream.precision(8);
  std::fstream summary_file_stream;
  summary_file_stream.precision(12);
  std::string filename;

  std::vector<unsigned int> description_str_lengths;
  //  description_str_lengths.push_back(5);
  //  description_str_lengths.push_back(20);
  description_str_lengths.push_back(100);

  std::vector<unsigned int> nr_clients;
  if(NR_CLIENTS == 0)
  {
    // 2 clients are always present --> send and recv client
    // nr_clients.push_back(0);
    // nr_clients.push_back(3);
    nr_clients.push_back(8);
    //nr_clients.push_back(50);
  }
  else
    nr_clients.push_back(NR_CLIENTS);

  std::vector<TiD::TiDClient*> clients_vec;

  try
  {
    filename = "libtid_localhost_send_and_receive_timing-" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +"-reps_summary.txt";
    summary_file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);
    summary_file_stream << "All values are in microseconds:" << std::endl << std::endl;

    for(unsigned int cl_ind = 0; cl_ind < nr_clients.size(); cl_ind++ )
    {
      TiD::TiDServer test_server;
      test_server.bind (9001);
      test_server.reserveNrOfMsgs(1000000);
      test_server.start();

      TiD::TimedTiDClient send_client;
      send_client.reserveNrOfMsgs(1000000);
      boost::this_thread::sleep(boost::posix_time::milliseconds(20));
      send_client.connect("127.0.0.1",9001);

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      std::cout << "  ... iteration " << cl_ind+1 << " from " << nr_clients.size() << std::endl;
      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec.push_back(new TiD::TiDClient );
        clients_vec[n]->reserveNrOfMsgs(1000000);
        clients_vec[n]->connect("127.0.0.1",9001);
        clients_vec[n]->startReceiving(0);
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

      TiD::TimedTiDClient recv_client;
      recv_client.reserveNrOfMsgs(1000000);
      recv_client.connect("127.0.0.1",9001);
      recv_client.startReceiving(0);
      boost::this_thread::sleep(boost::posix_time::milliseconds(100));

      for(unsigned int k= 0; k < description_str_lengths.size(); k++ )
      {
        std::cout << "    ... sub-iteration " << k+1 << " from " << description_str_lengths.size();
        std::cout << " (will take at least " << SLEEP_TIME_BETWEEN_MSGS * NR_TID_MESSAGES << ")" << std::endl;

        msg_builder.generateMsgsVector(NR_TID_MESSAGES, description_str_lengths[k]);  // << Hardcoded value
        std::vector<IDMessage>& msgs_vec = msg_builder.getMessagesVector();

        stat.reset();
        recv_client.clearRecvTimingValues();
        recv_client.clearMessages();
        boost::this_thread::sleep(boost::posix_time::milliseconds(500));


        filename = "libtid_localhost_send_and_receive_timing_nr_clients_"
            + boost::lexical_cast<std::string>(nr_clients[cl_ind])
            + "_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
            + "nr_reps_" + boost::lexical_cast<std::string>(msgs_vec.size()) +".csv";
        file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);


        boost::chrono::high_resolution_clock::time_point  start_time;
        boost::chrono::duration<double, boost::micro>     recv_diff;

        boost::posix_time::microsec no_msgs_available_sleep_time = boost::posix_time::microseconds(10);

        for(unsigned int n = 0; n < msgs_vec.size(); n++ )
        {
          send_client.sendTimedMessage( msgs_vec[n],start_time);


          boost::this_thread::sleep(SLEEP_TIME_BETWEEN_MSGS);


          while( !recv_client.newReceiveDiffsAvailable())
            boost::this_thread::sleep(no_msgs_available_sleep_time);

          if(recv_client.getRecvTimePoints().size() != 1)
          {
            std::cerr << "   #####  ERROR localhost test -- Something went wrong, received more than 1 TiD msg! (";
            std::cerr << recv_client.getRecvTimePoints().size() << " msgs -- newMessagesAvailable: " << recv_client.newMessagesAvailable() << ") #####" << std::endl;
            recv_client.clearRecvTimingValues();
            recv_client.clearMessages();
            n--;
          }
          else
          {
            recv_diff = recv_client.getRecvTimePoints()[0] - start_time;
            stat.update(recv_diff.count());
            TiDHelpers::updateFileStream(file_stream, stat);
          }
          recv_client.clearRecvTimingValues();
          recv_client.clearMessages();
          for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
            clients_vec[n]->clearMessages();


        }
        test_server.clearMessages();
        file_stream.unget();
        file_stream << " ";
        file_stream.close();

        filename = "libtid_localhost_send_and_receive_timing_nr_clients_"
            + boost::lexical_cast<std::string>(nr_clients[cl_ind])
            + "_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
            + "nr_reps_" + boost::lexical_cast<std::string>(msgs_vec.size()) +".raw.csv";
        file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);

        stat.printSampleValues(file_stream);

        file_stream.unget();
        file_stream << " ";
        file_stream.close();

        summary_file_stream << "Desc-len: "<< boost::lexical_cast<std::string>(description_str_lengths[k]);
        summary_file_stream << ", Nr Clients: " << nr_clients[cl_ind] << std::endl << std::endl;
        stat.printAll(summary_file_stream);
        summary_file_stream << std::endl << std::endl;


        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec[n]->stopReceiving();
        clients_vec[n]->disconnect();
        delete clients_vec[n];
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }
      clients_vec.clear();
      recv_client.stopReceiving();
      recv_client.disconnect();
      send_client.disconnect();

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      test_server.stop();

      boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }
  }
  catch(std::exception& e)
  {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }

  summary_file_stream.close();
  std::cout << std::endl << std::endl;

  return;
}

//-------------------------------------------------------------------------------------------------

TEST(libTiDSharedMemLocalHostSendReceiveTimingTest)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(200));
  std::cout << "Running libTiD SHM localhost send and receive timing test" << std::endl;

  TiDMessageVectorBuilder msg_builder;
  tobiss::Statistics  stat(true, STATISTICS_WINDOW_SIZE );
  std::fstream file_stream;
  file_stream.precision(8);
  std::fstream summary_file_stream;
  summary_file_stream.precision(12);
  std::string filename;

  std::vector<unsigned int> description_str_lengths;
  description_str_lengths.push_back(100);

  std::vector<unsigned int> nr_clients;
  if(NR_CLIENTS == 0)
  {
    nr_clients.push_back(8);
  }
  else
    nr_clients.push_back(NR_CLIENTS);

  std::vector<TiD::TiDClient*> clients_vec;

  std::string queue_str = "qu_";

  try
  {
    filename = "libtid_SHM_localhost_send_and_receive_timing-" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +"-reps_summary.txt";
    summary_file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);
    summary_file_stream << "All values are in microseconds:" << std::endl << std::endl;

    for(unsigned int cl_ind = 0; cl_ind < nr_clients.size(); cl_ind++ )
    {
      TiD::TiDServer test_server;
      for(unsigned int n = 0; n < nr_clients[cl_ind] +2; n++)
      {
        std::string queue = queue_str + boost::lexical_cast<std::string>(n);
        test_server.addSHMMessageQueue(queue);
        test_server.startMessageQueueProcessing(queue);
      }

      boost::this_thread::sleep(boost::posix_time::milliseconds(200));

      TiD::TimedTiDClient send_client;
      boost::this_thread::sleep(boost::posix_time::milliseconds(20));

      std::string send_queue = queue_str + boost::lexical_cast<std::string>(nr_clients[cl_ind]);
      std::cout << "Conneting Send client at: " << send_queue << std::endl << std::flush;
      send_client.createSHMMessageQueue(send_queue);
      //send_client.startReceivingFromSHM();

      TiD::TimedTiDClient recv_client;
      std::string recv_queue = queue_str + boost::lexical_cast<std::string>(nr_clients[cl_ind] +1);
      std::cout << "Conneting Recv client at: " << recv_queue << std::endl << std::flush;
      recv_client.createSHMMessageQueue(recv_queue);
      recv_client.startReceivingFromSHM();

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      std::cout << "  ... iteration " << cl_ind+1 << " from " << nr_clients.size() << std::endl;
      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec.push_back(new TiD::TiDClient );
        std::string queue = queue_str + boost::lexical_cast<std::string>(n);
        std::cout << "Conneting client at: " << queue << std::endl << std::flush;
        clients_vec[n]->createSHMMessageQueue(queue);
        clients_vec[n]->startReceivingFromSHM();
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

      boost::this_thread::sleep(boost::posix_time::milliseconds(100));


      for(unsigned int k= 0; k < description_str_lengths.size(); k++ )
      {
        std::cout << "    ... sub-iteration " << k+1 << " from " << description_str_lengths.size();
        std::cout << " (will take at least " << SLEEP_TIME_BETWEEN_MSGS * NR_TID_MESSAGES << ")" << std::endl;

        msg_builder.generateMsgsVector(NR_TID_MESSAGES, description_str_lengths[k]);  // << Hardcoded value
        std::vector<IDMessage>& msgs_vec = msg_builder.getMessagesVector();

        stat.reset();
        recv_client.clearRecvTimingValues();
        recv_client.clearMessages();
        boost::this_thread::sleep(boost::posix_time::milliseconds(500));


        filename = "libtid_localhost_SHM_send_and_receive_timing_nr_clients_"
            + boost::lexical_cast<std::string>(nr_clients[cl_ind])
            + "_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
            + "nr_reps_" + boost::lexical_cast<std::string>(msgs_vec.size()) +".csv";
        file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);


        boost::chrono::high_resolution_clock::time_point  start_time;
        boost::chrono::duration<double, boost::micro>     recv_diff;

        boost::posix_time::microsec no_msgs_available_sleep_time = boost::posix_time::microseconds(10);

        for(unsigned int n = 0; n < msgs_vec.size(); n++ )
        {
          send_client.sendTimedMessage( msgs_vec[n],start_time);
          recv_client.waitForSHM();

          boost::this_thread::yield();
          //boost::this_thread::sleep(SLEEP_TIME_BETWEEN_MSGS);

          //while( !recv_client.newRecvTimePointsAvailable())
            //boost::this_thread::sleep(no_msgs_available_sleep_time);

          if(recv_client.getRecvTimePoints().size() != 1)
          {
            std::cerr << "   #####  ERROR localhost test -- Something went wrong, received more than 1 TiD msg! (";
            std::cerr << recv_client.getRecvTimePoints().size() << " msgs -- newMessagesAvailable: " << recv_client.newMessagesAvailable() << ") #####" << std::endl;
            recv_client.clearRecvTimingValues();
            recv_client.clearMessages();
            n--;
          }
          else
          {
            recv_diff = recv_client.getRecvTimePoints()[0] - start_time;
            stat.update(recv_diff.count());
            TiDHelpers::updateFileStream(file_stream, stat);
          }
          recv_client.clearRecvTimingValues();
          recv_client.clearMessages();
          send_client.clearMessages();
          for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
            clients_vec[n]->clearMessages();
        }
        test_server.clearMessages();
        file_stream.unget();
        file_stream << " ";
        file_stream.close();

        filename = "libtid_localhost_SHM_send_and_receive_timing_nr_clients_"
            + boost::lexical_cast<std::string>(nr_clients[cl_ind])
            + "_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
            + "nr_reps_" + boost::lexical_cast<std::string>(msgs_vec.size()) +".raw.csv";
        file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);

        stat.printSampleValues(file_stream);

        file_stream.unget();
        file_stream << " ";
        file_stream.close();

        summary_file_stream << "Desc-len: "<< boost::lexical_cast<std::string>(description_str_lengths[k]);
        summary_file_stream << ", Nr Clients: " << nr_clients[cl_ind] << std::endl << std::endl;
        stat.printAll(summary_file_stream);
        summary_file_stream << std::endl << std::endl;


        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec[n]->stopReceivingFromSHM();
        clients_vec[n]->closeMsgQueue();
        delete clients_vec[n];
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }
      clients_vec.clear();
      recv_client.stopReceivingFromSHM();
      recv_client.closeMsgQueue();
      send_client.closeMsgQueue();

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      test_server.stopMessageQueueProcessing();
      test_server.closeAllMsgQueues();

      boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }
  }
  catch(std::exception& e)
  {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }

  summary_file_stream.close();
  std::cout << std::endl << std::endl;

  return;
}

//-------------------------------------------------------------------------------------------------

TEST(libTiDRemoteSendReceiveTimingTest)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(200));
  std::cout << "Running libTiD remote send and receive timing test" << std::endl;
  #ifdef SKIP_LIBTID_REMOTE_SEND_RECEIVE_TEST
    std::cout << "  --> skipping !!" << std::endl;
    return;
  #endif

  TiDMessageVectorBuilder msg_builder;
  tobiss::Statistics  stat(true, STATISTICS_WINDOW_SIZE );
  std::fstream file_stream;
  file_stream.precision(8);
  std::fstream summary_file_stream;
  summary_file_stream.precision(12);
  std::string filename;

  std::vector<unsigned int> description_str_lengths;
  //  description_str_lengths.push_back(5);
  //  description_str_lengths.push_back(20);
  description_str_lengths.push_back(100);

  std::vector<unsigned int> nr_clients;
  if(NR_CLIENTS == 0)
  {
    // 2 clients are always present --> send and recv client
    // nr_clients.push_back(0);
    nr_clients.push_back(3);
    // nr_clients.push_back(8);
    // nr_clients.push_back(50);
  }
  else
    nr_clients.push_back(NR_CLIENTS);

  std::vector<TiD::TiDClient*> clients_vec;

  try
  {

    TiD::TimedTiDClient send_client;
    send_client.reserveNrOfMsgs(1000000);

    filename = "libtid_remote_send_and_receive_timing-" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +"-reps_summary.txt";
    summary_file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);
    summary_file_stream << "All values are in microseconds:" << std::endl << std::endl;

    for(unsigned int cl_ind = 0; cl_ind < nr_clients.size(); cl_ind++ )
    {
      send_client.connect("192.168.1.11",9001);
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      std::cout << "  ... iteration " << cl_ind+1 << " from " << nr_clients.size() << std::endl;
      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec.push_back(new TiD::TiDClient );
        clients_vec[n]->reserveNrOfMsgs(1000000);
        clients_vec[n]->connect("192.168.1.11",9001);
        clients_vec[n]->startReceiving(0);
        boost::this_thread::sleep(boost::posix_time::milliseconds(20));
      }

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

      TiD::TimedTiDClient recv_client;
      recv_client.reserveNrOfMsgs(1000000);
      recv_client.connect("192.168.1.11",9001);
      recv_client.startReceiving(0);
      boost::this_thread::sleep(boost::posix_time::milliseconds(100));

      for(unsigned int k= 0; k < description_str_lengths.size(); k++ )
      {
        std::cout << "    ... sub-iteration " << k+1 << " from " << description_str_lengths.size();
        std::cout << " (will take at least " << SLEEP_TIME_BETWEEN_MSGS_REMOTE * NR_TID_MESSAGES << ")" << std::endl;

        msg_builder.generateMsgsVector(NR_TID_MESSAGES, description_str_lengths[k]);  // << Hardcoded value
        std::vector<IDMessage>& msgs_vec = msg_builder.getMessagesVector();

        stat.reset();
        recv_client.clearRecvTimingValues();
        recv_client.clearMessages();
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));


        filename = "libtid_remote_send_and_receive_timing_nr_clients_"
            + boost::lexical_cast<std::string>(nr_clients[cl_ind])
            + "_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
            + "nr_reps_" + boost::lexical_cast<std::string>(msgs_vec.size()) +".csv";
        file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);


        boost::chrono::high_resolution_clock::time_point  start_time;
        boost::chrono::duration<double, boost::micro>     recv_diff;
        boost::posix_time::microsec no_msgs_available_sleep_time = boost::posix_time::microseconds(3);

        for(unsigned int n = 0; n < msgs_vec.size(); n++ )
        {
          send_client.sendTimedMessage( msgs_vec[n],start_time);
          boost::this_thread::sleep(SLEEP_TIME_BETWEEN_MSGS_REMOTE);


          while( !recv_client.newReceiveDiffsAvailable())
            boost::this_thread::sleep(no_msgs_available_sleep_time);

          if(recv_client.getRecvTimePoints().size() != 1)
          {
            std::cerr << "   #####  ERROR remote test -- Something went wrong, received more than 1 TiD msg! (";
            std::cerr << recv_client.getRecvTimePoints().size() << " msgs -- newMessagesAvailable: " << recv_client.newMessagesAvailable() << ") #####" << std::endl;
            recv_client.clearRecvTimingValues();
            recv_client.clearMessages();
            n--;
          }
          else
          {
            recv_diff = recv_client.getRecvTimePoints()[0] - start_time;
            stat.update(recv_diff.count());
            TiDHelpers::updateFileStream(file_stream, stat);
          }
          recv_client.clearRecvTimingValues();
          recv_client.clearMessages();
          for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
            clients_vec[n]->clearMessages();
        }
        file_stream.unget();
        file_stream << " ";
        file_stream.close();

        filename = "libtid_remote_send_and_receive_timing_nr_clients_"
            + boost::lexical_cast<std::string>(nr_clients[cl_ind])
            + "_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
            + "nr_reps_" + boost::lexical_cast<std::string>(msgs_vec.size()) +".raw.csv";
        file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);

        stat.printSampleValues(file_stream);

        file_stream.unget();
        file_stream << " ";
        file_stream.close();

        summary_file_stream << "Desc-len: "<< boost::lexical_cast<std::string>(description_str_lengths[k]);
        summary_file_stream << ", Nr Clients: " << nr_clients[cl_ind] << std::endl << std::endl;
        stat.printAll(summary_file_stream);
        summary_file_stream << std::endl << std::endl;

        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec[n]->stopReceiving();
        clients_vec[n]->disconnect();
        delete clients_vec[n];
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }
      clients_vec.clear();
      recv_client.stopReceiving();
      recv_client.disconnect();
      send_client.disconnect();

      boost::this_thread::sleep(boost::posix_time::milliseconds(50));
    }

    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
  }
  catch(std::exception& e)
  {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }

  summary_file_stream.close();
  std::cout << std::endl << std::endl;

  return;
}

//-------------------------------------------------------------------------------------------------
