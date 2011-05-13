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

#include <cstdio>
#include <iostream>

#include <libtobiic/ICClass.hpp>
#include <libtobiic/ICClassifier.hpp>
#include <libtobicore/TCException.hpp>

int main(void) {
	ICClass class_0("0x300", 0.0f);
	ICClass class_1("0x301", 1.0f);
	ICClass class_2("0x302", 2.0f);
	ICClass class_3("0x303", 3.0f);
	ICClass class_4("0x304", 4.0f);
	ICClass class_5("0x305", 5.0f);
	ICClass class_6("0x306", 6.0f);
	ICClass class_7("0x307", 7.0f);
	ICClass class_8("0x308", 8.0f);
	ICClass class_9("0x309", 9.0f);
	
	ICClassifier classifier("test", "Test Classifier", 
			ICClassifier::ValueRCoe, ICClassifier::LabelBiosig);
	
	try {
		classifier.classes.Add(&class_0);
		classifier.classes.Add(&class_1);
		classifier.classes.Add(&class_2);
		classifier.classes.Add(&class_3);
		classifier.classes.Add(&class_4);
		classifier.classes.Add(&class_5);
		classifier.classes.Add(&class_6);
		classifier.classes.Add(&class_7);
		classifier.classes.Add(&class_8);
		classifier.classes.Add(&class_9);
		classifier.classes.Add(NULL);
	} catch(TCException e) { 
		std::cout << e << std::endl;
	}

	try {
		classifier.classes.Remove(class_0.GetLabel());
		classifier.classes.Remove(class_0.GetLabel());
	} catch(TCException e) { 
		std::cout << e << std::endl;
	}
		
	try {
		classifier.classes.Add(&class_1);
	} catch(TCException e) { 
		std::cout << e << std::endl;
	}
	try {
		classifier.classes.Remove(class_1.GetLabel());
		classifier.classes.Add(&class_1);
	} catch(TCException e) { 
		std::cout << e << std::endl;
	}

	std::cout << 
		ICClassifier::ValueProb  << " " << 
		ICClassifier::ValueType("prob") << std::endl;
	std::cout << 
		ICClassifier::ValueDist  << " " << 
		ICClassifier::ValueType("dist") << std::endl;
	std::cout << 
		ICClassifier::ValueRCoe  << " " << 
		ICClassifier::ValueType("rcoe") << std::endl;
	std::cout << 
		ICClassifier::ValueCLbl  << " " << 
		ICClassifier::ValueType("clbl") << std::endl;
	std::cout << 
		ICClassifier::ValueUndef << " " << 
		ICClassifier::ValueType("undef") << std::endl;
	
	std::cout << 
		ICClassifier::LabelCustom << " " << 
		ICClassifier::LabelType("custom") << std::endl;
	std::cout << 
		ICClassifier::LabelBiosig << " " << 
		ICClassifier::LabelType("biosig") << std::endl;
	std::cout << 
		ICClassifier::LabelUndef << " " << 
		ICClassifier::LabelType("undef") << std::endl;
	
	return 0;
}
