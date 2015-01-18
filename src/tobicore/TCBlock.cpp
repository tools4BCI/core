/*
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
