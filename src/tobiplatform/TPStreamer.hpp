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

/*! \brief Utility class to handle data streams
 *
 * \ingroup tobiplatform
 *
 * This class is designed to allow an easy handling of the data streams received
 * from a TCP connection (but not only).
 * It allows to verify whether a certain message is present and to extract it
 * automatically. 
 */
class TPStreamer {
	public:
		//! \brief Constructor
		TPStreamer(void);

		//! \brief Destructor
		virtual ~TPStreamer(void);

		/*! \brief Appends a string to the streamer
		 *
		 * \arg buffer The string to append
		 */
		virtual void Append(std::string buffer);
		
		/*! \brief Appends a char array to the streamer
		 *
		 * \arg buffer The char array to append
		 */
		virtual void Append(const char* buffer, size_t bsize);
		
		/*! \brief Extracts a message from the streamer
		 *
		 * Searches for a message (given a direction, i.e. FIFO or LIFO)
		 * contained between an header and a trailer and extracts it.
		 *
		 * \arg buffer Extracted message
		 * \arg hdr Header (i.e. "<msg>")
		 * \arg trl Trailer (i.e. "</msg>")
		 * \arg direction FIFO or LIFO
		 * \returns True if message was first found and then extracted, false
		 * otherwise
		 */
		virtual bool Extract(std::string* buffer, std::string hdr, 
				std::string trl, TPStreamerDirection direction = TPStreamer::Forward);
		
		/*! \brief Searches the streamer for a message
		 *
		 * Searches for a message (given a direction, i.e. FIFO or LIFO)
		 * contained between an header and a trailer and extracts it.
		 *
		 * \arg hdr Header (i.e. "<msg>")
		 * \arg trl Trailer (i.e. "</msg>")
		 * \arg direction FIFO or LIFO
		 * \returns True if message was found, false otherwise
		 */
		virtual bool Has(std::string hdr, std::string trl, 
				TPStreamerDirection direction = TPStreamer::Forward);
		
		/*! \brief Count the number of messages given one header
		 *
		 * \arg hdr The header that identifies the messages
		 * \return Number of messages
		 */
		virtual int Count(std::string hdr);
		
		/*! \brief Prints contents
		 */
		virtual void Dump(void);
		
		/*! \brief Returns size in bytes
		 * \return Bytes
		 */
		virtual int Size(void);
		
		/*! \brief Clears the streamer
		 */
		virtual void Clear(void);
	private:
		/*! \brief Non thread-safe implementation of a stream search
		 *
		 * To be used when TPStreamer::_mtxstream is already locked.
		 *
		 * \arg hdr Header (i.e. "<msg>")
		 * \arg trl Trailer (i.e. "</msg>")
		 * \arg direction FIFO or LIFO
		 * \returns True if message was found, false otherwise
		 */
		virtual bool ImplHas(std::string hdr, std::string trl, 
				TPStreamerDirection direction);

	private:
		//! \brief Holds the appended data
		std::string _stream;
		
		//! \brief Protects _stream
		TPMutex _mtxstream;

	public:
		//! \brief Search forward (FIFO)
		static const TPStreamerDirection Forward = 0;
		
		//! \brief Search reverse (LIFO)
		static const TPStreamerDirection Reverse = 1;
};

#endif
