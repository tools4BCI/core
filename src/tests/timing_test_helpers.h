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

#ifndef TIMING_TEST_HELPERS_H
#define TIMING_TEST_HELPERS_H

#include "statistics.h"
#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time.hpp>

namespace TiDHelpers
{

//-------------------------------------------------------------------------------------------------

char rand_alnum();
void rand_alnum_str (std::string::size_type sz, std::string& s);
void updateFileStream(std::ostream& file_stream, tobiss::Statistics& stat);

void printElapsedTime(const boost::system::error_code& /*e*/, boost::asio::deadline_timer* timer,
                      boost::posix_time::ptime ref_time);

//-------------------------------------------------------------------------------------------------

} //TiDHelpers


//-------------------------------------------------------------------------------------------------



#endif // TIMING_TEST_HELPERS_H
