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

#ifndef TID_MESSAGE_H
#define TID_MESSAGE_H

#include <string>

namespace TiD
{

//-----------------------------------------------------------------------------
class TiDMessage
{
public:
    TiDMessage (std::string const& version, std::string const& type,
                std::string const& parameters, std::string const& content)
      : version_ (version), type_ (type), parameters_ (parameters),
        content_ (content)
      {}

    virtual ~TiDMessage () {}

    std::string getVersion () const {return version_;}
    std::string getType () const {return type_;}
    std::string getParameters () const {return parameters_;}
    std::string getContent () const {return content_;}

private:
    std::string version_;
    std::string type_;
    std::string parameters_;
    std::string content_;
};

}

#endif // TID_MESSAGE_H
