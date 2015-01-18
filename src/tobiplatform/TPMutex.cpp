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

    TPMutex.hpp/.cpp is part of libcnbicore
*/

#ifndef TPMUTEX_CPP
#define TPMUTEX_CPP

#include "TPMutex.hpp"

TPMutex::TPMutex(void) {
  pthread_mutex_init(&this->_mutex, NULL);
}

TPMutex::~TPMutex(void) {
  pthread_mutex_destroy(&this->_mutex);
}

void TPMutex::Lock(void) {
  pthread_mutex_lock(&this->_mutex);
}

void TPMutex::Release(void) {
  pthread_mutex_unlock(&this->_mutex);
}

bool TPMutex::TryLock(void) {
  if(pthread_mutex_trylock(&this->_mutex) == 0)
    return true;
  else
    return false;
}

#endif
