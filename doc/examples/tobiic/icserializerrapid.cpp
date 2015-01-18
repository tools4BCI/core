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

#include <tobiic/ICClass.hpp>
#include <tobiic/ICClassifier.hpp>
#include <tobiic/ICSerializerRapid.hpp>
#include <tobicore/TCException.hpp>

int main(void) {
	// Create a MI classifier w/ 3 classes
	ICClassifier classifierMI("cnbi_mi", "CNBI MI Classifier", 
			ICClassifier::ValueProb, 
			ICClassifier::LabelBiosig);
	ICClass class_rh("0x301", 0.10f);
	ICClass class_lh("0x300", 0.20f);
	ICClass class_bf("0x302", 0.70f);
	classifierMI.classes.Add(&class_rh);
	classifierMI.classes.Add(&class_lh);
	classifierMI.classes.Add(&class_bf);
	
	// Create an ERRP classifier w/ 3 classes
	ICClassifier classifierERP("fsl_erp", "FSL ERP Classifier", 
			ICClassifier::ValueDist, 
			ICClassifier::LabelCustom);
	ICClass class_a("A", 1.00f);
	ICClass class_b("B", 2.00f);
	ICClass class_c("C", 3.00f);
	classifierERP.classes.Add(&class_a);
	classifierERP.classes.Add(&class_b);
	classifierERP.classes.Add(&class_c);

	// Create an IC message and add the two classifiers
	ICMessage sender;
	sender.classifiers.Add(&classifierMI);
	sender.classifiers.Add(&classifierERP);
	
	// Set a block number
	sender.SetBlockIdx(1234567890);

	// Serialize the message to buffer1
	ICSerializerRapid rapid(&sender, true);
	std::string buffer1;
	rapid.Serialize(&buffer1);
	
	// Or to buffer2 with SerializeCp
	std::string buffer2;
	buffer2 = rapid.SerializeCp();
	
	// Or to buffer3 with SerializeCh
	char buffer3[4800];
	rapid.SerializeCh(buffer3, 4800);

	// The three buffers are the same
	std::cout << "[1]>> " << buffer1 << std::endl;
	std::cout << "[2]>> " << buffer2 << std::endl;
	std::cout << "[3]>> " << buffer3 << std::endl;

	/* Although supported, it would be better not to use strings for the class
	 * labels.
	 */
	ICClass class_s("0x300", 0.10f);
	std::cout << "[s,s] This will work >> " << class_s.GetLabel() << std::endl;
	std::cout << "[s,i] This will fail >> " << class_s.GetLabelUInt() << std::endl;
	
	// It is always better to use integers
	ICClass class_i(0x300, 0.10f);
	std::cout << "[i,s] This will work >> " << class_i.GetLabel() << std::endl;
	std::cout << "[i,s] This will work >> " << class_i.GetLabelUInt() << std::endl;

	return 0;
}
