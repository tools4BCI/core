/*/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    It is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
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
     * \arg message IDMessage to enqueue
     * \arg updatefidx Tic the frame timestamp if true
     */
    virtual void Enqueue(const IDMessage message, bool updatefidx = false);


    /*! \brief Dequeues an IDMessage, removing it from the internal queue
     *
     * \arg message Reference to an already allocated IDMessage
     * \arg type Matching rule based on IDFtype
     * \arg event Matching rule based on IDevent
     * \arg direction Matching rule based on direction
     */
    virtual bool Dequeue(IDMessage* message = NULL,
        const IDFtype type = IDMessage::FamilyUndef,
        const IDevent event = IDMessage::EventNull,
        const int direction = IDAsClient::BlockAll);

    /*! \brief Queue size getter
     *
     * \return The number of queued IDMessage
     */
    virtual unsigned int Size(void) const;

    /*! \brief Queue cleaner
     *
     * \return The number of deleted IDMessage
     */
    virtual unsigned int Clear(void);

    //! \brief Queue cleaner
    virtual void Dump(void) const;

  public:
    /*! \brief Dequeue search direction (in time)
     *
     * Indentifies the messages generated and sent by BCI modules located
     * before, after or at the same position of the current module.
     * For further details:
     * <a href="http://files.mtvl.org/projects/tobicore/doc/libtobiid.pdf">TOBI iD for dummies</a>.
     */
    enum {
      /*! Messages from modules at the same position in the processing
       * pipeline of the current module
       */
      BlockThis = 0,
      /*! Messages from modules at the same position in the processing
       * pipeline of the current module
       */
      BlockPrev,
      /*! Messages from modules at the same position in the processing
       * pipeline of the current module
       */
      BlockNext,
      //! Messages from all modules
      BlockAll
    };
  private:
    //! \brief IDMessage queue
    std::vector<IDMessage> _queue;
};

#endif
