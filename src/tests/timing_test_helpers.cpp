
#include "timing_test_helpers.h"
#include <boost/bind.hpp>
#include<boost/asio/placeholders.hpp>

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
