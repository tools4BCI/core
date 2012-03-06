/*
    This file is part of the TOBI SignalServer test routine.

    The TOBI SignalServer test routine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The TOBI SignalServer test routine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the TOBI SignalServer test routine. If not, see <http://www.gnu.org/licenses/>.

    Copyright 2010 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#include "UnitTest++/UnitTest++.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "timing_test_helpers.h"

unsigned int NR_TID_MESSAGES = 50;
unsigned int NR_CLIENTS = 0;

boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS(3);



//-------------------------------------------------------------------------------------------------

char TiDHelpers::rand_alnum()
{
  char c;
  while (!std::isalnum(c = static_cast<char>(rand())))
    ;
  return c;
}

//-------------------------------------------------------------------------------------------------

void TiDHelpers::rand_alnum_str (std::string::size_type sz, std::string& s)
{
  s.clear();
  generate_n (std::back_inserter(s), sz, rand_alnum);
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

int main( int argc, const char* argv[] )
{
  if(argc >1)
  {
    NR_TID_MESSAGES = boost::lexical_cast<unsigned int>(argv[1]);
  }

  if(argc > 2)
  {
    NR_CLIENTS = boost::lexical_cast<unsigned int>(argv[2]);
  }


    srand (time(NULL));
    std::cout << std::endl << "Build on " << __DATE__ << " at " << __TIME__ << std::endl;
    std::cout << "  ...  running " << NR_TID_MESSAGES << " messages on " << NR_CLIENTS << " clients." << std::endl<< std::endl;

    return UnitTest::RunAllTests();
}

