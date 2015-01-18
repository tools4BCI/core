/*
    This file is part of TOBI Interface D (TiD).

    TOBI Interface D (TiD) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TOBI Interface D (TiD) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TOBI Interface D (TiD).  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H

#include <string>

namespace TiD
{

//-----------------------------------------------------------------------------
class InputStream
{
  public:
    virtual ~InputStream () {}
    virtual void readUntil (const std::string& delimiter, std::string* str) = 0;
//    virtual void readUntil (std::string* str) = 0;
//    virtual void setDelimiter(const std::string& del) = 0;
};

}

#endif // INPUT_STREAM_H
