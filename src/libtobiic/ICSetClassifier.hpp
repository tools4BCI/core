/*
    Copyright (C) 2010  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobiic library

    The libtobiic library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ICCLASSIFIERS_HPP
#define ICCLASSIFIERS_HPP

#include <string>
#include "ICClassifier.hpp"
#include "ICMapClassifier.hpp"

class ICSetClassifier {
	public:
		ICSetClassifier(void);
		virtual ~ICSetClassifier(void);
		
		virtual ICClassifier* Add(ICClassifier* pclr);
	
		virtual ICClassifier* Remove(std::string nclr);
		virtual ICClassifier* Remove(ICClassifier* pclr);
		
		virtual ICClassifier* Get(std::string nclr) const;
		virtual ICClassifier* Get(ICClassifier* pclr) const;
		
		virtual bool Has(std::string nclr) const;
		virtual bool Has(ICClassifier* pclr) const;
		virtual bool Empty(void) const;
		virtual unsigned int Size(void) const;
		
		virtual void Clear(void);
		virtual void Destroy(void);
		
		virtual ICClassifierIter Begin(void);
		virtual ICClassifierIter End(void);
		virtual ICClassifierConstIter Begin(void) const;
		virtual ICClassifierConstIter End(void) const;
	
	public:
		ICClassifierMap _map;
};

#endif
