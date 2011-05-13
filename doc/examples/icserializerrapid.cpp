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
	ICClassifier classifierMI("cnbi_mi", "CNBI MI Classifier", 
			ICClassifier::ValueProb, 
			ICClassifier::LabelBiosig);
	ICClass class_rh("0x301", 0.10f);
	ICClass class_lh("0x300", 0.20f);
	ICClass class_bf("0x302", 0.70f);
	classifierMI.classes.Add(&class_rh);
	classifierMI.classes.Add(&class_lh);
	classifierMI.classes.Add(&class_bf);
	
	ICClassifier classifierERP("fsl_erp", "FSL ERP Classifier", 
			ICClassifier::ValueDist, 
			ICClassifier::LabelCustom);
	ICClass class_a("A", 1.00f);
	ICClass class_b("B", 2.00f);
	ICClass class_c("C", 3.00f);
	classifierERP.classes.Add(&class_a);
	classifierERP.classes.Add(&class_b);
	classifierERP.classes.Add(&class_c);

	ICMessage sender;
	sender.classifiers.Add(&classifierMI);
	sender.classifiers.Add(&classifierERP);
	
	sender.SetBlockIdx(1234567890);
	ICSerializerRapid rapid(&sender, true);

	std::string buffer1;
	rapid.Serialize(&buffer1);
	
	std::string buffer2;
	buffer2 = rapid.SerializeCp();
	
	char buffer3[4800];
	rapid.SerializeCh(buffer3, 4800);

	std::cout << "[1]>> " << buffer1 << std::endl;
	std::cout << "[2]>> " << buffer2 << std::endl;
	std::cout << "[3]>> " << buffer3 << std::endl;

	/* We need to be careful:
	 * - if we pass a string ("0x300"), it will be impossible to retrieve it as
	 * an integer (although I could use sscanf)
	 */
	ICClass class_s("0x300", 0.10f);
	std::cout << "[s,s] This will work >> " << class_s.GetLabel() << std::endl;
	std::cout << "[s,i] This will fail >> " << class_s.GetLabelUInt() << std::endl;
	
	ICClass class_i(0x300, 0.10f);
	std::cout << "[i,s] This will work >> " << class_i.GetLabel() << std::endl;
	std::cout << "[i,s] This will work >> " << class_i.GetLabelUInt() << std::endl;


	return 0;
}
