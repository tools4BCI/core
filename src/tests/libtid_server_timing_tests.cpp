
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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/asio/basic_deadline_timer.hpp>
#include <boost/bind.hpp>


#include "UnitTest++/UnitTest++.h"

#include "tobiid/IDMessage.hpp"

#include "tid_message_vector_builder.h"
#include "statistics.h"
#include "timing_test_helpers.h"

#include "timed_tid_server.h"
#include "libtid/tid_client.h"

extern unsigned int NR_TID_MESSAGES;
extern unsigned int STATISTICS_WINDOW_SIZE;
extern unsigned int NR_CLIENTS;
extern boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS;


using std::fstream;

//-------------------------------------------------------------------------------------------------

TEST(libTiDServerDispatchTiming)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(200));
  std::cout << "Running libTiD server-dispatching timing test" << std::endl;
  #ifdef SKIP_LIBTID_SERVER_DISPATCH_TEST
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
//    nr_clients.push_back(1);
    nr_clients.push_back(5);
//    nr_clients.push_back(10);
    //    nr_clients.push_back(50);
  }
  else
    nr_clients.push_back(NR_CLIENTS);

  std::vector<TiD::TiDClient*> clients_vec;

  try
  {
    TiD::TimedTiDServer server;
    server.bind(9001);
    server.start();

    TiD::TiDClient send_client;

    filename = "libtid_server_dispatch-" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +"-reps_summary.txt";
    summary_file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);
    summary_file_stream << "All values are in microseconds:" << std::endl << std::endl;

    for(unsigned int cl_ind = 0; cl_ind < nr_clients.size(); cl_ind++ )
    {
      std::cout << "  ... iteration " << cl_ind+1 << " from " << nr_clients.size() << std::endl;
      send_client.connect("127.0.0.1",9001);
      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec.push_back(new TiD::TiDClient );
        clients_vec[n]->connect("127.0.0.1",9001);
        clients_vec[n]->startReceiving(0);
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }

      boost::this_thread::sleep(boost::posix_time::milliseconds(100));

      for(unsigned int k= 0; k < description_str_lengths.size(); k++ )
      {
        std::cout << "    ... sub-iteration " << k+1 << " from " << description_str_lengths.size();
        std::cout << " (will take at least " << SLEEP_TIME_BETWEEN_MSGS * NR_TID_MESSAGES << ")" << std::endl;


        msg_builder.generateMsgsVector(NR_TID_MESSAGES, description_str_lengths[k]);
        std::vector<IDMessage>& msgs_vec = msg_builder.getMessagesVector();


        stat.reset();
        server.clearConnectionDispatchTimings();
        server.clearMessages();
        server.reserveNrOfMsgs(NR_TID_MESSAGES/10);
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));

        filename = "libtid_server_dipatch_nr_clients_"
            + boost::lexical_cast<std::string>(nr_clients[cl_ind])
            + "_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
            + "nr_reps_" + boost::lexical_cast<std::string>(msgs_vec.size()) +".csv";
        file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);

        boost::asio::io_service io;
        boost::posix_time::ptime start_time = boost::posix_time::second_clock::local_time();

        boost::asio::deadline_timer timer(io, boost::posix_time::seconds(60));

        timer.async_wait( boost::bind(TiDHelpers::printElapsedTime, boost::asio::placeholders::error,
                                      &timer, start_time));

        boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
        std::cout << "        ";
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));

        for(unsigned int n = 0; n < msgs_vec.size(); n++ )
        {
          send_client.sendMessage(msgs_vec[n]);
          boost::this_thread::sleep(SLEEP_TIME_BETWEEN_MSGS);
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        io.stop();
        t.join();
        std::cout << std::endl;

        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        std::list< std::vector<double> > diff_list = server.getConnectionDispatchTimings();
        std::vector<double>& diffs = *diff_list.begin();

        for(unsigned int n = 0; n < diffs.size(); n++)
        {
          for(unsigned int m = 0; m < nr_clients[cl_ind]; m++)
            clients_vec[m]->clearMessages();

          server.clearMessages();
          stat.update( diffs[n] );
          TiDHelpers::updateFileStream(file_stream, stat);
        }
        file_stream.unget();
        file_stream << " ";
        file_stream.close();

        filename = "libtid_server_dipatch_nr_clients_"
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

        for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
          clients_vec[n]->clearMessages();
      }
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

      for(unsigned int n = 0; n < nr_clients[cl_ind]; n++)
      {
        clients_vec[n]->disconnect();
        delete clients_vec[n];
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }
      clients_vec.clear();
      send_client.disconnect();

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    server.stop();

  }
  catch(std::exception& e)
  {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }

  summary_file_stream.close();
  std::cout << std::endl << std::endl;
}

//-------------------------------------------------------------------------------------------------
