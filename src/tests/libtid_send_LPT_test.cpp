
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

#include "timing_test_helpers.h"

#include "LPT_tid_server.h"
#include "LPT_tid_client.h"

extern unsigned int NR_TID_MESSAGES;
extern unsigned int NR_CLIENTS;
extern boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS;

using std::fstream;

//-------------------------------------------------------------------------------------------------

TEST(libTiDSendReceiveTimingTest)
{
  #ifdef ACT_AS_TID_SERVER



  #else


  #endif

}
