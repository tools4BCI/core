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

#include "ICSerializerRapid.hpp"
#include <tobicore/TCException.hpp>
#include <tobicore/TCTools.hpp>
#include <iostream>
#include <tobicore/rapidxml.hpp>
#include <tobicore/rapidxml_print.hpp>
#include <stdlib.h>
#include <string.h>
#include <vector>

#ifdef __BORLANDC__
using namespace std;
#endif
using namespace rapidxml;
	

ICSerializerRapid::ICSerializerRapid(ICMessage* const message, const bool indent, 
		const bool declaration) 
	: ICSerializer(message) {
	this->_indent = indent;
	this->_declaration = declaration;
}
		
ICSerializerRapid::~ICSerializerRapid(void) {
}
	
std::string* ICSerializerRapid::Serialize(std::string* buffer) {
	if(buffer == NULL)
		return NULL;
	if(ICSerializer::message == NULL)
		throw TCException("iC message not set, cannot serialize");

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

	// Convert frame number to char array
	char cacheFidx[16];
	TCTools::itoa(ICSerializer::message->GetBlockIdx(), cacheFidx);

	// Same thing with timestamp and reference
	std::string timestamp, reference;
	ICSerializer::message->absolute.Get(&timestamp);
	ICSerializer::message->relative.Get(&reference);

	// Root node
	xml_node<>* root = doc.allocate_node(node_element, ICMESSAGE_ROOTNODE);
	root->append_attribute(doc.allocate_attribute(ICMESSAGE_VERSIONNODE,
				ICMESSAGE_VERSION));
	root->append_attribute(doc.allocate_attribute(ICMESSAGE_FRAMENODE, 
				cacheFidx));
	root->append_attribute(doc.allocate_attribute(ICMESSAGE_TIMESTAMPNODE, 
				timestamp.c_str()));
	root->append_attribute(doc.allocate_attribute(ICMESSAGE_REFERENCENODE, 
				reference.c_str()));
	doc.append_node(root);
	
	// Root node
	ICClassifier* cptr = NULL;
	ICClass* kptr = NULL;
	ICClassifierConstIter cit = ICSerializer::message->classifiers.Begin();
	ICSetClassConstIter kit;

	char* ptr_vtype = NULL;
	char* ptr_ltype = NULL;
	xml_node<>* cnode = NULL;
	xml_node<>* knode = NULL;

	// Loop over classifiers
	while(cit != ICSerializer::message->classifiers.End()) {
		cptr = (*cit).second;
		cnode = doc.allocate_node(node_element, ICMESSAGE_CLASSISIFERNODE);
		cnode->append_attribute(doc.allocate_attribute(ICMESSAGE_NAMENODE,
					cptr->GetChName())); 
		cnode->append_attribute(doc.allocate_attribute(ICMESSAGE_DESCNODE, 
					cptr->GetChDescription()));
		root->append_node(cnode);
	
		// Attribute: Value Type
		switch(cptr->GetValueType()) {
			case ICClassifier::ValueUndef:
				ptr_vtype = (char*)ICTYPES_ENTRY_UNDEF;
				break;
			case ICClassifier::ValueProb:
				ptr_vtype = (char*)ICTYPES_ENTRY_PROB;
				break;
			case ICClassifier::ValueDist:
				ptr_vtype = (char*)ICTYPES_ENTRY_DIST;
				break;
			case ICClassifier::ValueCLbl:
				ptr_vtype = (char*)ICTYPES_ENTRY_CLBL;
				break;
			case ICClassifier::ValueRCoe:
				ptr_vtype = (char*)ICTYPES_ENTRY_RCOE;
				break;
		}
		cnode->append_attribute(doc.allocate_attribute(ICMESSAGE_VTYPENODE, 
					ptr_vtype));
		
		// Attribute: Label Type
		switch(cptr->GetLabelType()) {
			case ICClassifier::LabelClass:
				ptr_ltype = (char*)ICTYPES_LABEL_CLASS;
				break;
			case ICClassifier::LabelUndef:
				ptr_ltype = (char*)ICTYPES_LABEL_UNDEF;
				break;
			case ICClassifier::LabelBiosig:
				ptr_ltype = (char*)ICTYPES_LABEL_BIOSIG;
				break;
			case ICClassifier::LabelCustom:
				ptr_ltype = (char*)ICTYPES_LABEL_CUSTOM;
				break;
		}
		cnode->append_attribute(doc.allocate_attribute(ICMESSAGE_LTYPENODE, 
					ptr_ltype));
	
		// Loop over classes
		kit = cptr->classes.Begin();
		while(kit != cptr->classes.End()) {
			kptr = (*kit).second;
			knode = doc.allocate_node(node_element, ICMESSAGE_CLASSNODE, 
					kptr->GetChValue());
			knode->append_attribute(doc.allocate_attribute(ICMESSAGE_LABELNODE,
						kptr->GetChLabel()));
			cnode->append_node(knode);

			// Next class
			++kit;
		}
		// Next classifier
		++cit;
	}
	
	if(this->_indent) 
		print(std::back_inserter(*buffer), doc);
	else
		print(std::back_inserter(*buffer), doc, print_no_indenting);
	return buffer;
}

std::string* ICSerializerRapid::Deserialize(std::string* const buffer) {
	bool initialize = ICSerializer::message->classifiers.Empty();

	xml_document<> doc;
	std::string cache;
	std::vector<char> xml_copy(buffer->begin(), buffer->end());
    xml_copy.push_back('\0');
    doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	/* Trasverse to root node */
	xml_node<>* rootnode = doc.first_node(ICMESSAGE_ROOTNODE);
	if(rootnode == NULL) {
		throw TCException("iC root node not found", __PRETTY_FUNCTION__);
	}

	/* Check version */
	cache = rootnode->first_attribute(ICMESSAGE_VERSIONNODE)->value();
	if(cache.compare(ICMESSAGE_VERSION) != 0) {
		std::string info("iC version mismatch: ");
		info.append(ICMESSAGE_VERSION);
		info.append("/");
		info.append(cache);
		throw TCException(info, __PRETTY_FUNCTION__);
	}
	
	/* Get frame number */
	cache = rootnode->first_attribute(ICMESSAGE_FRAMENODE)->value();
	ICSerializer::message->SetBlockIdx(atol(cache.c_str()));

	// Get timestamp
	cache = rootnode->first_attribute(ICMESSAGE_TIMESTAMPNODE)->value();
	ICSerializer::message->absolute.Set(cache);
	cache = rootnode->first_attribute(ICMESSAGE_REFERENCENODE)->value();
	ICSerializer::message->relative.Set(cache);

	/* Define classifier and class nodes */
	xml_node<>* cnode = NULL;
	xml_node<>* knode = NULL;

	ICClassifier* cptr = NULL;
	ICClass* kptr = NULL;

	/* Trasverse classifier nodes */
	cnode = rootnode->first_node(ICMESSAGE_CLASSISIFERNODE);
	while(cnode != NULL) {
		std::string cname, cdesc, tvtype, tltype;
		ICVtype cvtype;
        ICLtype cltype;

		/* Get name, description, value and label types */
		cname = cnode->first_attribute(ICMESSAGE_NAMENODE)->value();
		if(initialize) {
			cdesc = cnode->first_attribute(ICMESSAGE_DESCNODE)->value();
			tvtype = cnode->first_attribute(ICMESSAGE_VTYPENODE)->value();
			tltype = cnode->first_attribute(ICMESSAGE_LTYPENODE)->value();

			/* Convert value and label types */
			cvtype = ICClassifier::ValueType(tvtype);
			cltype = ICClassifier::LabelType(tltype);
			
			/* Create classifier */
			if(ICSerializer::message->classifiers.Has(cname) == true)
				return NULL;

			cptr = new ICClassifier(cname, cdesc, cvtype, cltype);
			ICSerializer::message->classifiers.Add(cptr);
		} else 
			cptr = ICSerializer::message->classifiers.Get(cname);
		
		/* Trasverse class nodes */
		knode = cnode->first_node(ICMESSAGE_CLASSNODE);
		while(knode != NULL) {
			std::string klabel = 
				knode->first_attribute(ICMESSAGE_LABELNODE)->value();
			std::string tvalue = knode->value();
			
			if(initialize) {
				if(cptr->classes.Has(klabel) == true)
					return NULL;
				kptr = new ICClass(klabel, (ICValue)atof(tvalue.c_str()));
				cptr->classes.Add(kptr);
			} else 
				cptr->classes.Get(klabel)->SetValue(TCTools::atof(tvalue.c_str()));

			/* Next class node */
			knode = knode->next_sibling(ICMESSAGE_CLASSNODE);
		}
		
		/* Next classifier node */
		cnode = cnode->next_sibling(ICMESSAGE_CLASSISIFERNODE);
	}
	return buffer;
}
