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
#include <libtobiic/ICSerializerRapid.hpp>
#include <libtobicore/TCException.hpp>

int main(void) {
	ICClassifier classifierMI("cnbi_mi", "CNBI MI Classifier", 
			ICClassifier::ValueProb, 
			ICClassifier::LabelBiosig);
	ICClass class_rh("0x300", 0.60f);
	ICClass class_lh("0x301", 0.40f);
	try { 
		classifierMI.classes.Add(&class_rh);
		classifierMI.classes.Add(&class_lh);
		classifierMI.classes.Add(NULL);
	} catch(TCException e) { 
		std::cout << e << std::endl;
	}
	std::cout << "Size: " << classifierMI.classes.Size() << std::endl;

	ICClassifier classifierERP("cnbi_errp", "CNBI ErrP Classifier", 
			ICClassifier::ValueDist, 
			ICClassifier::LabelCustom);
	ICClass class_ed("ed", 0.85f);
	classifierERP.classes.Add(&class_ed);

	printf(">> Step 1: Sender configs a message\n");
	ICMessage sender;
	sender.classifiers.Add(&classifierMI);
	sender.classifiers.Add(&classifierERP);
	sender.Dump();
	sender.GetValue("cnbi_mi", "0x300");
	

	printf(">> Step 2: Sender serializes the message\n");
	std::string buffer1;
	ICSerializerRapid rapidSender(&sender, true);
	rapidSender.Serialize(&buffer1);
	
	printf(">> Step 3: Receiver deserializes the message\n");
	ICMessage receiver;
	ICSerializerRapid rapidReceiver(&receiver, true);
	rapidReceiver.Deserialize(&buffer1);
	receiver.Dump();

	std::cout << buffer1<<std::endl;

	printf(">> Step 4: Receiver modifies the message\n");
	receiver.GetClass("cnbi_mi", "0x301")->SetValue(0.01f);
	receiver.GetClass("cnbi_mi", "0x300")->SetValue(0.02f);
	receiver.GetClass("cnbi_errp", "ed")->SetValue(10);
	receiver.Dump();

	printf(">> Step 5: Receiver serializes the message\n");
	std::string buffer2;
	rapidReceiver.Serialize(&buffer2);
	
	printf(">> Step 6: Sender deserializes the message\n");
	rapidSender.Deserialize(&buffer2);
	sender.Dump();

	std::string channel;
	printf(">> Step 7: Stress test\n");
	for(unsigned int j = 0; j < 1000; j++) {
		channel.clear();
		receiver.GetClass("cnbi_errp", "ed")->SetValue(j);
		rapidReceiver.Serialize(&channel);
		rapidSender.Deserialize(&channel);
	}
	receiver.Dump();
	sender.Dump();
	
	printf(">> Step 8: Stress test\n");
	sender.SetBlockIdx(1000);
	for(unsigned int j = 0; j < 1000; j++) {
		channel.clear();
		sender.GetClass("cnbi_mi", "0x300")->SetValue(2*j);
		sender.IncBlockIdx();
		rapidSender.Serialize(&channel);
		rapidReceiver.Deserialize(&channel);
	}
	sender.Dump();
	receiver.Dump();
	return 0;
}
