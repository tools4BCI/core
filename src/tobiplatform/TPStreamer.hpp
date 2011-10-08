/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
        
    TPStreamer.hpp/.cpp is part of libcnbicore
*/

#ifndef TPSTREAMER_HPP
#define TPSTREAMER_HPP

#include "TPMutex.hpp"
#include <string>

typedef unsigned int TPStreamerDirection;

/*! \brief Data streams handler
 *
 * This class is designed to allow an easy handling of data streams.
 * It allows to verify whether a certain message is present and to extract it
 * automatically. 
 */
class TPStreamer {
	public:
		TPStreamer(void);
		virtual ~TPStreamer(void);

		virtual void Append(std::string buffer);
		virtual void Append(const char* buffer, size_t bsize);
		virtual bool Extract(std::string* buffer, std::string hdr, 
				std::string trl, TPStreamerDirection direction = TPStreamer::Forward);
		virtual bool Has(std::string hdr, std::string trl, 
				TPStreamerDirection direction = TPStreamer::Forward);
		virtual int Count(std::string hdr);
		virtual void Dump(void);
		virtual int Size(void);
		virtual void Clear(void);
	private:
		virtual bool ImplHas(std::string hdr, std::string trl, 
				TPStreamerDirection direction);

	private:
		std::string _stream;
		TPMutex _mtxstream;

	public:
		static const TPStreamerDirection Forward = 0;
		static const TPStreamerDirection Reverse = 1;
};

#endif
