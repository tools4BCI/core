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

#ifndef TPINTERFACE_CPP
#define TPINTERFACE_CPP

#include "TPInterface.hpp"

TPInterface::TPInterface(void) {
  this->_socket = NULL;
  this->_endpoint = NULL;
  this->_com = NULL;
}

TPInterface::~TPInterface(void) {
}

int TPInterface::Plug(const std::string &ip, const std::string& port, int mode) {
  switch(mode) {
    case TPInterface::AsServer:
      return this->ConfAsServer(ip, port);
      break;
    case TPInterface::AsClient:
      return this->ConfAsClient(ip, port);
      break;
  }
  return TPInterface::ErrorGeneric;
}

void TPInterface::Unplug(void) {
  if(this->_socket == NULL)
    return;

  this->_socket->Close();
  delete this->_socket;
  this->_socket = NULL;

  this->_com = NULL;

  if(this->_socket != NULL) {
    delete this->_endpoint;
    this->_endpoint = NULL;
  }
}


int TPInterface::ConfAsServer(const std::string &ip, const std::string& port) {
  if(this->_socket != NULL)
    return TPInterface::ErrorBound;

  bool status = true;
  this->_socket = new TPSocket(TPSocket::TCP);
  status &= this->_socket->Open(true);
  status &= this->_socket->Bind(ip, port);
  status &= this->_socket->Listen();
  if(status == false) {
    this->Unplug();
    return TPInterface::ErrorSocket;
  }

  this->_endpoint = new TPSocket(TPSocket::TCP);
  status = this->_socket->Accept(this->_endpoint);
  if(status == false) {
    this->Unplug();
    return TPInterface::ErrorEndpoint;
  }

  this->_com = this->_endpoint;
  return TPInterface::Successful;
}

int TPInterface::ConfAsClient(const std::string &ip, const std::string& port) {
  if(this->_socket != NULL)
    return TPInterface::ErrorBound;

  bool status = true;
  this->_socket = new TPSocket(TPSocket::TCP);
  status &= this->_socket->Open(false);
  status &= this->_socket->Connect(ip, port);
  if(status == false) {
    this->Unplug();
    return TPInterface::ErrorSocket;
  }

  this->_socket->IsConnected();
  if(this->_socket->IsConnected() == false) {
    this->Unplug();
    return TPInterface::ErrorSocket;
  }

  this->_com = this->_socket;
  return TPInterface::Successful;
}

bool TPInterface::IsPlugged(void) {
  if(this->_com == NULL)
    return false;
  return this->_com->IsConnected();
}

#endif
