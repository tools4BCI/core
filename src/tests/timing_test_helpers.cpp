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

#include "timing_test_helpers.h"
#include <boost/bind.hpp>
#include<boost/asio/placeholders.hpp>

//-------------------------------------------------------------------------------------------------

//char TiDHelpers::rand_alnum()
//{
//  char c;
//  while (!std::isalnum(c = static_cast<char>(rand())))
//    ;
//  return c;
//}

//-------------------------------------------------------------------------------------------------

void TiDHelpers::rand_alnum_str (std::string::size_type sz, std::string& s)
{
  s.clear();
  s.resize(sz);
  //generate_n (std::back_inserter(s), sz, rand_alnum);

      static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (unsigned int i = 0; i < sz; ++i)
    {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
}

//-------------------------------------------------------------------------------------------------

void TiDHelpers::updateFileStream(std::ostream& file_stream, tobiss::Statistics& stat)
{
  file_stream << stat.get_mean() << ",";

  file_stream << stat.get_adaptive_mean() << ",";
  file_stream << stat.get_adaptive_var() << ",";

  file_stream << stat.get_window_mean() << ",";
  file_stream << stat.get_window_median() << ",";
  file_stream << stat.get_window_min() << ",";
  file_stream << stat.get_window_max() << ",";
  file_stream << stat.get_window_var() << ",";
}

//-------------------------------------------------------------------------------------------------

void TiDHelpers::printElapsedTime(const boost::system::error_code& /*e*/,
                                  boost::asio::deadline_timer* timer, boost::posix_time::ptime ref_time)

{
  boost::posix_time::time_duration td = boost::posix_time::second_clock::local_time() - ref_time ;
  std::cout.precision(0);
  std::cout << td.minutes() << "."  << std::noshowpoint << td.seconds()/6.0 << "min, " << std::flush;

  timer->expires_at(timer->expires_at() + boost::posix_time::seconds(60));
  timer->async_wait(boost::bind(printElapsedTime,
        boost::asio::placeholders::error, timer, ref_time));

}

//-------------------------------------------------------------------------------------------------
