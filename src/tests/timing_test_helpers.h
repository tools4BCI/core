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
