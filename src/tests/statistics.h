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

/**
* @file statistics.h
*
* @brief Class to collect various runtime statistics.
**/

#ifndef STATISTICS_H
#define STATISTICS_H

#include <boost/circular_buffer.hpp>
#include <list>
#include <iostream>

namespace tobiss
{

/**
* @class Statistics
* @brief Class to calculate various (univariate) runtime statistics.
*/
class Statistics
{
friend class StatisticInterface;

public:
  /**
    * @brief Constructor
    * @param[in] buffer_data bool Buffer the data to calculate statistics over the whole recording
    * @param[in] window_size size_t Window size for window-based statistics
    */
  Statistics( bool buffer_data = false, size_t window_size = 1);

  /**
    * @brief Destructor
    */
  virtual ~Statistics( );

  /**
    * @brief Update statistics with a new sample
    * @param[in] x double New sample
    */
  void update( double x );

  /**
    * @brief Print statistics to specified stream (used to keep the former interface consistency).
    */
  //  void  print(std::ostream &out = std::cout, bool print_everything = false)
  //  {

  //  }

  /**
    * @brief Print all statistics to specified stream.
    */
  void printAll( std::ostream &out = std::cout );

  /**
    * @brief Print only window statistics to specified stream.
    */
  void printWindowStatistics( std::ostream &out = std::cout );

  /**
    * @brief Print only runtime statistics (mean, min, max, and adaptive statistics) to specified stream.
    */
  void printFastStatistics( std::ostream &out = std::cout );

  /**
    * @brief Print all buffered samples to specified stream with a given seperator.
    */
  void printSampleValues( std::ostream &out , char separator = ',');

  /**
    * @brief Set everything to initial values.
    */
  void reset( );

  /**
    * @brief Set the update coefficient.
    */
  void setUpdateCoefficient(double coeff);

  double get_mean( ) { return mean_; }

  /**
    * @brief Get the variance
    * @warning Could take a very long time in case of a high number of values in the buffer
    */
  double get_var();

  /**
    * @brief Get the median
    * @warning Could take a very long time in case of a high number of values in the buffer
    */
  double get_median();

  double get_min() { return min_; }
  double get_max() { return max_; }

  double get_adaptive_mean( ) { return adaptive_mean_; }
  double get_adaptive_var( ) { return adaptive_var_; }

  double get_window_mean( );

  /**
    * @brief Get the median of the last values within the given window
    * @warning Could take a very long time in case of a high window size
    */
  double get_window_median( );

  double get_window_min( );
  double get_window_max( );

  /**
    * @brief Get the variance of the last values within the given window
    * @warning Could take a very long time in case of a high window size
    */
  double get_window_var( );

protected:
  /**
    * @brief Returns iterators to a range of the sample buffer buffer
    * @param[in] buffersize size_t size of the buffer
    * @param[out] begin iterator start of the buffer section
    * @param[out] end iterator end of the buffer section
    */
  void getSamples( size_t buffersize, std::list<double>::iterator begin, std::list<double>::iterator end  );

private:
  template<typename T> inline static void delptr( T ptr ) { delete ptr; }
  /**
    * @brief Sort the samples in the window buffer.
    */
  void sort_win_buffer();

  /**
    * @brief Sort ALL recorded samples
    * @warning Could take a very long time in case of a high number of values in the buffer
    */
  void sort_sample_buffer();

private:
  boost::circular_buffer<double>      window_buffer_;
  boost::circular_buffer<double>      window_buffer_sorted_;
  std::list<double>                   sample_buffer_;
  double N;
  double update_coefficient_;

  double mean_;
  double var_;
  double min_;
  double max_;

  double adaptive_mean_;
  double adaptive_var_;

  bool   buffer_data_;
  size_t window_size_;
  bool   win_buf_is_sorted_;
  bool   sample_buf_is_sorted_;
};

}

#endif
