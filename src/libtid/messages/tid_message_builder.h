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

#ifndef TID_MESSAGE_BUILDER_H
#define TID_MESSAGE_BUILDER_H

//#include "tid_message.h"

class IDMessage;

namespace TiD
{

//-----------------------------------------------------------------------------
class TiDMessageBuilder
{
public:
    virtual ~TiDMessageBuilder () {}

    virtual void buildTiDMessage (IDMessage& message, std::string& xml_string) = 0;
};


}

#endif // TID_MESSAGE_BUILDER_H
