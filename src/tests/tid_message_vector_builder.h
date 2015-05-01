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

#ifndef TID_MESSAGE_VECTOR_BUILDER_H
#define TID_MESSAGE_VECTOR_BUILDER_H

#include <time.h>
#include <vector>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include <algorithm>

#include "tobiid/IDMessage.hpp"
#include "timing_test_helpers.h"
#include <boost/bind.hpp>

class TiDMessageVectorBuilder
{
  public:
    TiDMessageVectorBuilder()
    {
      srand (time(NULL));
    }

    std::vector<IDMessage>& getMessagesVector()
    {
      return (msgs_vec_);
    }

    void generateMsgsVector(unsigned int nr_messages, std::string::size_type desc_length)
    {
      std::string str;
      str.resize(desc_length);
      msgs_vec_.clear();

      for(unsigned int n = 0; n < nr_messages; n++)
      {
        IDMessage msg(IDMessage::FamilyBiosig, rand() % 10000 + 1);
        TiDHelpers::rand_alnum_str(desc_length, str);
        msg.SetDescription( str.c_str() );
        msg.SetBlockIdx(n+1);
        msg.absolute.Tic();
        msg.relative.Tic();
        msgs_vec_.push_back(msg);
      }
    }


  private:
    //char rand_alnum()
    //{
    //  char c;
    //  while (!isalnum(c = static_cast<char>(rand())))
    //    ;
    //  return c;
    //}

    //void rand_alnum_str (std::string::size_type sz, std::string& s)
    //{
    //  s.clear();
    //  std::generate_n (std::back_inserter(s), sz, boost::bind(&TiDMessageVectorBuilder::rand_alnum, this));
    //}


  private:
    std::vector<IDMessage>    msgs_vec_;


};

#endif // TID_MESSAGE_VECTOR_BUILDER_H
