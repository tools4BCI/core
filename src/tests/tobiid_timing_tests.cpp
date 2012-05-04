
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

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <boost/chrono.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include "UnitTest++/UnitTest++.h"
#include "tobiid/IDMessage.hpp"
#include "tobiid/IDSerializerRapid.hpp"
#include "statistics.h"
#include "timing_test_helpers.h"


extern unsigned int NR_TID_MESSAGES;
extern unsigned int STATISTICS_WINDOW_SIZE;

using namespace std;

//-------------------------------------------------------------------------------------------------

TEST(tobiidSerializeTiming)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(200));
  std::cout << "Running tobiid serialization timing test" << std::endl;
  #ifdef SKIP_TOBIID_SERIALIZE_TEST
    std::cout << "  --> skipping !!" << std::endl;
    return;
  #endif

  srand (time(NULL));
  std::vector<IDMessage> messages;
  messages.reserve(NR_TID_MESSAGES);
  std::vector<unsigned int> description_str_lengths;
  description_str_lengths.push_back(5);
  description_str_lengths.push_back(10);
  description_str_lengths.push_back(20);
  description_str_lengths.push_back(50);
  description_str_lengths.push_back(100);

  std::fstream file_stream;
  file_stream.precision(8);
  std::fstream summary_file_stream;
  summary_file_stream.precision(12);
  std::string filename;

  IDSerializerRapid serializer;
  tobiss::Statistics  stat(true, STATISTICS_WINDOW_SIZE );

  filename = "tobiid_serialize-" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +"-reps_summary.txt";
  summary_file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);
  summary_file_stream << "All values are in microseconds:" << std::endl << std::endl;

  for(unsigned int k = 0; k < description_str_lengths.size(); k++)
  {
    std::cout << "  ... iteration " << k+1 << " from " << description_str_lengths.size() << std::endl;

    std::string str;
    str.resize(description_str_lengths[k]);
    messages.clear();

    for(unsigned int n = 0; n < NR_TID_MESSAGES; n++)
    {
      IDMessage msg(IDMessage::FamilyBiosig, rand() % 10000 + 1);
      TiDHelpers::rand_alnum_str(description_str_lengths[k], str);
      msg.SetDescription( str.c_str() );
      msg.SetBlockIdx(n+1);
      msg.absolute.Tic();
      msg.relative.Tic();
      messages.push_back(msg);
    }

    std::string xml_str;
    xml_str.reserve(2048);
    boost::chrono::high_resolution_clock::time_point  start_time(boost::chrono::high_resolution_clock::now() );
    boost::chrono::duration<double, boost::micro>  diff;
    stat.reset();
    serializer.SetMessage(&(messages[0]) );
    serializer.Serialize(&xml_str);

    filename = "tobiid_serialize_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
        + "nr_reps_" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +".csv";
    file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);
    boost::this_thread::sleep(boost::posix_time::milliseconds(2));

    for(unsigned int n = 0; n < messages.size(); n++)
    {
      start_time = boost::chrono::high_resolution_clock::now();
      serializer.SetMessage(&(messages[n]) );
      serializer.Serialize(&xml_str);
      diff = boost::chrono::high_resolution_clock::now() - start_time;
      stat.update(diff.count());
      TiDHelpers::updateFileStream(file_stream, stat);
    }
    file_stream.unget();
    file_stream << " ";
    file_stream.close();

    filename = "tobiid_serialize_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
        + "nr_reps_" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +".raw.csv";
    file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);

    stat.printSampleValues(file_stream);

    file_stream.unget();
    file_stream << " ";
    file_stream.close();

    filename = "tobiid_serialize_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
        + "nr_reps_" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +".raw.csv";
    file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);

    stat.printSampleValues(file_stream);

    file_stream.unget();
    file_stream << " ";
    file_stream.close();

    summary_file_stream << "Desc-len: "<< boost::lexical_cast<std::string>(description_str_lengths[k]) << std::endl << std::endl;
    stat.printAll(summary_file_stream);
    summary_file_stream << std::endl << std::endl;
  }

  std::cout << endl << endl;
}

//-------------------------------------------------------------------------------------------------

TEST(tobiidDeSerializeTiming)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(200));
  std::cout << "Running tobiid deserialization timing test" << std::endl;
  #ifdef SKIP_TOBIID_DESERIALIZE_TEST
    std::cout << "  --> skipping !!" << std::endl;
    return;
  #endif

  srand (time(NULL));
  std::string xml_str_to_serialize;
  xml_str_to_serialize.reserve(2048);
  std::string str_1("<tobiid version=\"0.0.2.1\" description=\"");
  std::string str_2("\" frame=\"");
  std::string str_3("\" family=\"biosig\" event=\"");
  std::string str_4("\" timestamp=\"");
  std::string str_5("\" reference=\"");
  std::string str_6("\"/>");
  std::string tmp_str;

  std::vector<unsigned int> description_str_lengths;
  description_str_lengths.push_back(5);
  description_str_lengths.push_back(10);
  description_str_lengths.push_back(20);
  description_str_lengths.push_back(50);
  description_str_lengths.push_back(100);

  std::fstream file_stream;
  file_stream.precision(8);
  std::fstream summary_file_stream;
  summary_file_stream.precision(12);
  std::string filename;

  tobiss::Statistics  stat(true, STATISTICS_WINDOW_SIZE );
  IDMessage tid_msg;
  IDMessage recv_message;
  IDSerializerRapid recv_serializer;

  boost::chrono::high_resolution_clock::time_point  start_time(boost::chrono::high_resolution_clock::now() );
  boost::chrono::duration<double, boost::micro>  diff;

  filename = "tobiid_deserialize-" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +"-reps_summary.txt";
  summary_file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);
  summary_file_stream << "All values are in microseconds:" << std::endl << std::endl;

  for(unsigned int k = 0; k < description_str_lengths.size(); k++)
  {
    std::cout << "  ... iteration " << k+1 << " from " << description_str_lengths.size() << std::endl;
    stat.reset();

    filename = "tobiid_deserialize_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
        + "nr_reps_" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +".csv";
    file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);
    boost::this_thread::sleep(boost::posix_time::milliseconds(2));

    for(unsigned int n = 0; n < NR_TID_MESSAGES; n++)
    {
      xml_str_to_serialize.clear();
      xml_str_to_serialize += str_1;

      TiDHelpers::rand_alnum_str(description_str_lengths[k], tmp_str);
      xml_str_to_serialize += tmp_str;

      xml_str_to_serialize += str_2;
      xml_str_to_serialize += boost::lexical_cast<std::string>(n);

      xml_str_to_serialize += str_3;
      xml_str_to_serialize += boost::lexical_cast<std::string>(rand() % 10000 + 1);

      tmp_str.clear();
      tid_msg.absolute.Tic();
      tid_msg.absolute.Get(&tmp_str);
      xml_str_to_serialize += str_4;
      xml_str_to_serialize += tmp_str;

      tmp_str.clear();
      tid_msg.relative.Tic();
      tid_msg.relative.Get(&tmp_str);
      xml_str_to_serialize += str_5;
      xml_str_to_serialize += tmp_str;

      xml_str_to_serialize += str_6;

      // std::cout << xml_str_to_serialize << std::endl << std::flush;

      start_time = boost::chrono::high_resolution_clock::now();
      recv_serializer.SetMessage(&recv_message);
      recv_serializer.Deserialize(&xml_str_to_serialize);
      diff = boost::chrono::high_resolution_clock::now() - start_time;
      stat.update(diff.count());

      TiDHelpers::updateFileStream(file_stream, stat);
    }
    file_stream.unget();
    file_stream << " ";
    file_stream.close();

    filename = "tobiid_deserialize_desc_len_" + boost::lexical_cast<std::string>(description_str_lengths[k])
        + "nr_reps_" + boost::lexical_cast<std::string>(NR_TID_MESSAGES) +".raw.csv";
    file_stream.open(filename.c_str(), fstream::in | fstream::out | fstream::trunc);

    stat.printSampleValues(file_stream);

    file_stream.unget();
    file_stream << " ";
    file_stream.close();

    summary_file_stream << "Desc-len: "<< boost::lexical_cast<std::string>(description_str_lengths[k]) << std::endl << std::endl;
    stat.printAll(summary_file_stream);
    summary_file_stream << std::endl << std::endl;
  }

  std::cout << endl << endl;
}

