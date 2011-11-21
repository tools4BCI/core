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
*/

#ifndef IDASCLIENT_CPP 
#define IDASCLIENT_CPP 

#include "IDAsClient.hpp" 

IDAsClient::IDAsClient(void) {
}

IDAsClient::~IDAsClient(void) {
}

void IDAsClient::Add(const IDMessage message, bool updatefidx) {
	this->_queue.push_back(message);
	if(updatefidx)
		TCBlock::SetBlockIdx(message.GetBlockIdx());
}

bool IDAsClient::Get(IDMessage* message,  const IDFtype type,
		const IDevent event, const int direction) {
	if(message == NULL)
		throw TCException("iD message needs to be allocated",
				__PRETTY_FUNCTION__);
	if(direction != IDAsClient::BlockAll && TCBlock::IsSetBlockIdx() == false)
		throw TCException("Block number must be set for searching Prev/Next",
				__PRETTY_FUNCTION__);
	
	if(this->Size() == 0)
		return false;

	int t_blockidx = TCBlock::BlockIdxUnset;
	IDFtype t_type;
	IDevent t_event;
	bool fmatch = false, tmatch = false, ematch = false;
	for(unsigned int i = 0; i < this->_queue.size(); i++) {
		t_blockidx = (this->_queue.at(i)).GetBlockIdx();
		t_type = (this->_queue.at(i)).GetFamilyType();
		t_event = (this->_queue.at(i)).GetEvent();

		// Match frame
		switch(direction) {
			case IDAsClient::BlockThis:
				fmatch = (t_blockidx == TCBlock::GetBlockIdx());
				break;
			case IDAsClient::BlockPrev:
				fmatch = (t_blockidx > TCBlock::GetBlockIdx());
				break;
			case IDAsClient::BlockNext:
				fmatch = (t_blockidx < TCBlock::GetBlockIdx());
				break;
			default:
			case IDAsClient::BlockAll:
				fmatch = true;
				break;
		}
		
		// Match type 
		switch(type) {
			case IDMessage::FamilyBiosig:
				tmatch = (type == t_type);
				break;
			default:
			case IDMessage::FamilyUndef:
				tmatch = true;
				break;
		}
		
		// Match event 
		switch(event) {
			case IDMessage::EventNull:
				ematch = true;
				break;
			default:
				ematch = (event == t_event);
				break;
		}

		if(tmatch && ematch && fmatch) {
			message->Copy(&(this->_queue.at(i)));
			this->_queue.erase(this->_queue.begin() + i);
			return true;
		}
	}

	return false;
}
		
unsigned int IDAsClient::Size(void) const {
	return this->_queue.size();
}

unsigned int IDAsClient::Clear(void) {
	this->_queue.clear();
	return this->Size();
}

void IDAsClient::Dump(void) const {
	for(unsigned int i = 0; i < this->_queue.size(); i++)
		(this->_queue.at(i)).Dump();
}

#endif
