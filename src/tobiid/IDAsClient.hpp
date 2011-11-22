/*/*
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
*/

#ifndef IDASCLIENT_HPP 
#define IDASCLIENT_HPP 

#include "IDTypes.hpp"
#include "IDMessage.hpp"
#include "IDSerializer.hpp"
#include <tobicore/TCException.hpp>
#include <tobicore/TCBlock.hpp>
#include <vector>

/*! \brief Utility class to handle iD messages
 * 
 * \ingroup tobiid
 *
 * This class can be useful to:
 * - handle large amounts of received IDMessage objects searching them by
 * IDFtype, IDevent and direction
 * - gather a deeper understanding on how IDMessage classes can be handled
 */
class IDAsClient : public TCBlock {
	public:
		//! \brief Constructor
		IDAsClient(void);

		//! \brief Destructor
		virtual ~IDAsClient(void);

		/*! \brief Add an IDMessage to the internal queue
		 *  
		 * \arg message
	 	 * \arg updatefidx
		 */
		virtual void Add(const IDMessage message, bool updatefidx = false);
		
		virtual bool Get(IDMessage* message = NULL,  
				const IDFtype type = IDMessage::FamilyUndef,
				const IDevent event = IDMessage::EventNull, 
				const int direction = IDAsClient::BlockAll);
		
		virtual unsigned int Size(void) const;
		
		virtual unsigned int Clear(void);
		
		virtual void Dump(void) const;

	public:
		enum { 
			BlockThis = 0, 
			BlockPrev,
			BlockNext,
			BlockAll 
		};
	private:
		std::vector<IDMessage> _queue;
	protected:
};

#endif
