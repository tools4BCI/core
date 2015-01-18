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

#ifndef TID_MESSAGE_TAGS_1_0_H
#define TID_MESSAGE_TAGS_1_0_H

#include <string>

namespace TiD
{

namespace TiDMessageTags10
{
  char const NEW_LINE_CHAR = 0x0A;
  std::string const XML_END_STRING("/>");

//    char const NEW_LINE_CHAR = 0x0A;
//    std::string const NEW_LINE (1, NEW_LINE_CHAR);
//    char const COMMAND_DELIMITER = ':';


//    std::string const VERSION = "1.0";
//    std::string const ID = "TiD";
//    std::string const SPACE = " ";
//    std::string const ID_AND_VERSION = ID + std::string (" ") + VERSION;

//    std::string const CONTENT_LENGTH = "Content-Length";

//    std::string const OK = "OK";
//    std::string const ERROR_STR = "Error";

//    std::string const EVENT = "Event";
//    std::string const MARKER = "Marker";

    // std::string const DESCRIPTION_REGISTRATION = "Description-Registration";
}

}

#endif // TID_MESSAGE_TAGS_1_0_H
