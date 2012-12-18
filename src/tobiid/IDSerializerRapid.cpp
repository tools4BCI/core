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

#include "IDSerializerRapid.hpp"
#include "IDTypes.hpp"
#include <tobicore/TCException.hpp>
#include <tobicore/TCTools.hpp>
#include <tobicore/rapidxml.hpp>
#include <tobicore/rapidxml_print.hpp>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>

#ifdef __BORLANDC__
using namespace std;
#endif
using namespace rapidxml;

IDSerializerRapid::IDSerializerRapid(IDMessage* const message, const bool indent,
    const bool declaration)
  : IDSerializer(message) {
  this->_indent = indent;
  this->_declaration = declaration;
}

//-----------------------------------------------------------------------------

IDSerializerRapid::~IDSerializerRapid(void) {
}

//-----------------------------------------------------------------------------

std::string* IDSerializerRapid::Serialize(std::string* buffer) {
  if(buffer == NULL)
    return NULL;
  if(IDSerializer::message == NULL)
    throw TCException("iD message not set, cannot serialize");

  buffer->clear();

  // XML document and buffers
  xml_document<> doc;
  std::string xml_as_string;
  std::string xml_no_indent;

  // XML declaration
  if(this->_declaration) {
    xml_node<>* decl = doc.allocate_node(node_declaration);
    decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
    doc.append_node(decl);
  }

  char cacheFidx[16], cacheEvent[128], cacheValue[128];
  TCTools::itoa(IDSerializer::message->GetBlockIdx(), cacheFidx);
  TCTools::itoa(IDSerializer::message->GetEvent(), cacheEvent);
  TCTools::ftoa(IDSerializer::message->GetValue(), cacheValue);
  IDFvalue fvalue = IDSerializer::message->GetFamily();

  std::string timestamp, reference;
  IDSerializer::message->absolute.Get(&timestamp);
  IDSerializer::message->relative.Get(&reference);

  // Root node
  xml_node<>* root = doc.allocate_node(node_element, IDMESSAGE_ROOTNODE);
  root->append_attribute(doc.allocate_attribute(IDMESSAGE_VERSIONNODE,
        IDMESSAGE_VERSION));
  root->append_attribute(doc.allocate_attribute(IDMESSAGE_DESCRIPTIONNODE,
        IDSerializer::message->_description.c_str()));
  root->append_attribute(doc.allocate_attribute(IDMESSAGE_FRAMENODE_2,
        cacheFidx));
  root->append_attribute(doc.allocate_attribute(IDMESSAGE_FAMILYNODE,
        fvalue.c_str()));
  root->append_attribute(doc.allocate_attribute(IDMESSAGE_EVENTNODE,
        cacheEvent));
  root->append_attribute(doc.allocate_attribute(IDMESSAGE_VALUENODE,
        cacheValue));
  root->append_attribute(doc.allocate_attribute(IDMESSAGE_TIMESTAMPNODE_2,
        timestamp.c_str()));
  root->append_attribute(doc.allocate_attribute(IDMESSAGE_REFERENCENODE_2,
        reference.c_str()));
  if( message->GetSource().length() > 0 )
  {
    root->append_attribute(doc.allocate_attribute(IDMESSAGE_SOURCENODE,
            message->GetSource().c_str()));
  }
  doc.append_node(root);

  if(this->_indent)
    print(std::back_inserter(*buffer), doc);
  else
    print(std::back_inserter(*buffer), doc, print_no_indenting);

  return buffer;
}

//-----------------------------------------------------------------------------

std::string* IDSerializerRapid::Deserialize(std::string* const buffer)
{
  if(buffer == NULL)
    throw TCException("iD buffer-pointer is NULL",
                                               #ifdef _WIN32
                                                       __FUNCSIG__
                                               #else
                                                       __PRETTY_FUNCTION__
                                               #endif
    );

  xml_document<> doc;
  std::string cache;
  std::vector<char> xml_copy(buffer->begin(), buffer->end());
    xml_copy.push_back('\0');
    doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

  xml_node<>* rootnode = doc.first_node(IDMESSAGE_ROOTNODE);
  if(rootnode == NULL)
    throw TCException("iD root node not found",
                                               #ifdef _WIN32
                                                       __FUNCSIG__
                                               #else
                                                       __PRETTY_FUNCTION__
                                               #endif
    );

  /* Check version */
  cache = rootnode->first_attribute(IDMESSAGE_VERSIONNODE)->value();
  if(cache.compare(IDMESSAGE_VERSION_SUPPORTED) == 0 )
  {
    // Get frame number
    cache.clear();
    cache = rootnode->first_attribute(IDMESSAGE_FRAMENODE)->value();
    IDSerializer::message->SetBlockIdx(atol(cache.c_str()));

    // Get timestamp
    cache.clear();
    cache = rootnode->first_attribute(IDMESSAGE_TIMESTAMPNODE)->value();
    IDSerializer::message->absolute.Set(cache);
    cache.clear();
    cache = rootnode->first_attribute(IDMESSAGE_REFERENCENODE)->value();
    IDSerializer::message->relative.Set(cache);

    cache = rootnode->first_attribute(IDMESSAGE_DESCRIPTIONNODE)->value();
    IDSerializer::message->SetDescription(cache);

    cache = rootnode->first_attribute(IDMESSAGE_FAMILYNODE)->value();
    if(cache.compare(IDTYPES_FAMILY_BIOSIG) == 0)
      IDSerializer::message->SetFamilyType(IDMessage::FamilyBiosig);
    else if(cache.compare(IDTYPES_FAMILY_CUSTOM) == 0)
      IDSerializer::message->SetFamilyType(IDMessage::FamilyCustom);
    else
      IDSerializer::message->SetFamilyType(IDMessage::FamilyUndef);

    cache.clear();
    cache = rootnode->first_attribute(IDMESSAGE_EVENTNODE)->value();
    IDSerializer::message->SetEvent(atoi(cache.c_str()));

    return buffer;
  }
  else if( cache.compare(IDMESSAGE_VERSION)  == 0 )
  {
    // Get frame number
    cache.clear();
    cache = rootnode->first_attribute(IDMESSAGE_FRAMENODE_2)->value();
    IDSerializer::message->SetBlockIdx(atol(cache.c_str()));

    // Get timestamp
    cache.clear();
    cache = rootnode->first_attribute(IDMESSAGE_TIMESTAMPNODE_2)->value();
    IDSerializer::message->absolute.Set(cache);
    cache.clear();
    cache = rootnode->first_attribute(IDMESSAGE_REFERENCENODE_2)->value();
    IDSerializer::message->relative.Set(cache);

    cache = rootnode->first_attribute(IDMESSAGE_DESCRIPTIONNODE)->value();
    IDSerializer::message->SetDescription(cache);

    cache = rootnode->first_attribute(IDMESSAGE_FAMILYNODE)->value();
    if(cache.compare(IDTYPES_FAMILY_BIOSIG) == 0)
      IDSerializer::message->SetFamilyType(IDMessage::FamilyBiosig);
    else if(cache.compare(IDTYPES_FAMILY_CUSTOM) == 0)
      IDSerializer::message->SetFamilyType(IDMessage::FamilyCustom);
    else
      IDSerializer::message->SetFamilyType(IDMessage::FamilyUndef);

    cache.clear();
    cache = rootnode->first_attribute(IDMESSAGE_EVENTNODE)->value();
    IDSerializer::message->SetEvent(atoi(cache.c_str()));

    cache.clear();
    cache = rootnode->first_attribute(IDMESSAGE_VALUENODE)->value();
    IDSerializer::message->SetValue(atof(cache.c_str()));

    if(rootnode->first_attribute(IDMESSAGE_SOURCENODE))
    {
      cache.clear();
      cache = rootnode->first_attribute(IDMESSAGE_SOURCENODE)->value();
      IDSerializer::message->SetSource(cache);
    }
    else
      IDSerializer::message->SetSource("");

    return buffer;
  }
  else
  {
    std::string info("iD version mismatch: ");
    info.append(IDMESSAGE_VERSION);
    info.append("/");
    info.append(cache);
    throw TCException(info,
                            #ifdef _WIN32
                                    __FUNCSIG__
                            #else
                                    __PRETTY_FUNCTION__
                            #endif
    );
  }


}

//-----------------------------------------------------------------------------
