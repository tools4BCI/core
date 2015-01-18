
#include <boost/date_time/posix_time/posix_time.hpp>

namespace
{
	//boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS;
	boost::posix_time::microseconds SLEEP_TIME_BETWEEN_MSGS(500);
	boost::posix_time::microseconds SLEEP_TIME_BETWEEN_MSGS_REMOTE(250);
}
