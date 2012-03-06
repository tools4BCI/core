#ifndef TIMING_TEST_HELPERS_H
#define TIMING_TEST_HELPERS_H

#include "statistics.h"

namespace TiDHelpers
{

//-------------------------------------------------------------------------------------------------

char rand_alnum();
void rand_alnum_str (std::string::size_type sz, std::string& s);
void updateFileStream(std::ostream& file_stream, tobiss::Statistics& stat);

//-------------------------------------------------------------------------------------------------

} //TiDHelpers

#endif // TIMING_TEST_HELPERS_H
