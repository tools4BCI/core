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

    TPStreamer.hpp/.cpp is part of libcnbicore
*/

#ifndef TPSTREAMER_CPP
#define TPSTREAMER_CPP

#include "TPStreamer.hpp"
#include <iostream>

TPStreamer::TPStreamer(void) {
}

TPStreamer::~TPStreamer(void) {
}

void TPStreamer::Append(std::string buffer) {
  this->_mtxstream.Lock();
  this->_stream.append(buffer);
  this->_mtxstream.Release();
}

void TPStreamer::Append(const char* buffer, size_t bsize) {
  this->_mtxstream.Lock();
  this->_stream.append(buffer, bsize);
  this->_mtxstream.Release();
}

bool TPStreamer::Extract(std::string *buffer, std::string hdr, std::string trl,
    TPStreamerDirection direction) {
  this->_mtxstream.Lock();

  if(this->_stream.empty()) {
    this->_mtxstream.Release();
    return false;
  }

  if(this->ImplHas(hdr, trl, direction) == false) {
    this->_mtxstream.Release();
    return false;
  }

  buffer->clear();

  std::string::size_type p_hdr, p_trl, delta;

  if(direction == TPStreamer::Forward) {
    p_hdr = this->_stream.find(hdr);
    p_trl = this->_stream.find(trl, p_hdr);
  } else {
    p_hdr = this->_stream.rfind(hdr);
    p_trl = this->_stream.rfind(trl, p_hdr);
  }
  delta = trl.size();

  if(p_hdr == std::string::npos || p_trl == std::string::npos) {
    this->_mtxstream.Release();
    return false;
  }

  if(p_hdr >= p_trl) {
    this->_mtxstream.Release();
    return false;
  }

  /* 2010-03-26  Matteo Lostuzzo <matteo.lostuzzo@epfl.ch>
   * 2010-03-26  Michele Tavella <michele.tavella@epfl.ch>
   * Extreme bug was blasted with Lostuzzo.
   */
  *buffer = this->_stream.substr(p_hdr, p_trl - p_hdr + delta);
  this->_stream.erase(p_hdr, p_trl - p_hdr + delta);
  this->_mtxstream.Release();

  return true;
}

bool TPStreamer::Has(std::string hdr, std::string trl,
    TPStreamerDirection direction) {
  bool result = false;
  this->_mtxstream.Lock();
  result = this->ImplHas(hdr, trl, direction);
  this->_mtxstream.Release();
  return result;
}

int TPStreamer::Count(std::string hdr) {
  int count = 0;

  this->_mtxstream.Lock();
  if(!this->_stream.empty()) {
    std::string::size_type pos(0);

    while (pos != std::string::npos) {
      pos = this->_stream.find(hdr, pos);
      if(pos != std::string::npos) {
        ++count;
        pos += 3;
      }
    }
  }
  this->_mtxstream.Release();

  return count;
}

void TPStreamer::Dump(void) {
  this->_mtxstream.Lock();
  std::cout << "[TPStreamer::Dump] " << this->_stream  << std::endl;
  this->_mtxstream.Release();
}

int TPStreamer::Size(void) {
  int size = 0;
  this->_mtxstream.Lock();
  size = this->_stream.size();
  this->_mtxstream.Release();

  return size;
}

void TPStreamer::Clear(void) {
  this->_mtxstream.Lock();
  this->_stream.clear();
  this->_mtxstream.Release();
}

bool TPStreamer::ImplHas(std::string hdr, std::string trl,
    TPStreamerDirection direction) {

  if(this->_stream.empty())
    return false;

  std::string::size_type p_hdr, p_trl;

  if(direction == TPStreamer::Forward) {
    p_hdr = this->_stream.find(hdr);
    p_trl = this->_stream.find(trl, p_hdr);
  } else {
    p_hdr = this->_stream.rfind(hdr);
    p_trl = this->_stream.rfind(trl, p_hdr);
  }

  if(p_hdr == std::string::npos || p_trl == std::string::npos)
    return false;

  if(p_hdr >= p_trl)
    return false;

  return true;
}

#endif
