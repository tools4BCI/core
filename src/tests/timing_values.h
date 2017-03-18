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

#include <boost/date_time/posix_time/posix_time.hpp>

namespace
{
  //boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS;
  boost::posix_time::microseconds SLEEP_TIME_BETWEEN_MSGS(500);
  boost::posix_time::microseconds SLEEP_TIME_BETWEEN_MSGS_REMOTE(2000);
}
