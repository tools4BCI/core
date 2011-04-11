/*
    Copyright (C) 2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobicore library

    The libtobicore library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTTCULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TCBLOCK_CPP 
#define TCBLOCK_CPP 

#include "TCBlock.hpp" 

TCBlock::TCBlock(void) {
	this->_blockidx = TCBlock::BlockIdxUnset;
}

int TCBlock::SetBlockIdx(int fidx) {
	this->_blockidx = fidx;
	return this->_blockidx;
}

int TCBlock::GetBlockIdx(void) const {
	return this->_blockidx;
}

int TCBlock::IncBlockIdx(void) {
	return ++this->_blockidx;
}

void TCBlock::UnsetBlockIdx(void) {
	this->_blockidx = TCBlock::BlockIdxUnset;
}

bool TCBlock::IsSetBlockIdx(void) const {
	return(this->_blockidx >= 0);
}

#endif
