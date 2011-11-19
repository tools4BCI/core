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
	// I create a classifier for MI
	ICClassifier classifierMI("cnbi_mi", "CNBI MI Classifier", 
			ICClassifier::ValueProb, 
			ICClassifier::LabelBiosig);
	
	// I create two MI classes
	ICClass class_rh("0x300", 0.60f);
	ICClass class_lh("0x301", 0.40f);
	
	// I try to add the two classes to the classifier
	try { 
		classifierMI.classes.Add(&class_rh);
		classifierMI.classes.Add(&class_lh);
	} catch(TCException e) { 
		std::cout << e << std::endl;
	}
	std::cout << "Size: " << classifierMI.classes.Size() << std::endl;

	// I create a classifier for ERRP
	ICClassifier classifierERP("cnbi_errp", "CNBI ErrP Classifier", 
			ICClassifier::ValueDist, 
			ICClassifier::LabelCustom);

	// I create an ERRP class and I add it to the ERRP classifier
	ICClass class_ed("ed", 0.85f);
	classifierERP.classes.Add(&class_ed);

	// Now I create a message
	printf(">> Step 1: Sender configs a message\n");
	ICMessage sender;
	
	// And I add the MI and ERRP classifiers to the message
	sender.classifiers.Add(&classifierMI);
	sender.classifiers.Add(&classifierERP);
	
	// This is just to show you how to use Dump and GetValue
	sender.Dump();
	sender.GetValue("cnbi_mi", "0x300");
	
	/* Now I serialize the message, simulating what an IC sender would do.
	 * - First, I create a string buffer
	 * - Then I create a concrete serializer and I bind the serializer to the
	 * message I want to serialize
	 * - Finally, I serialize the message in the buffer
	 */
	printf(">> Step 2: Sender serializes the message\n");
	std::string buffer1;
	ICSerializerRapid rapidSender(&sender, true);
	rapidSender.Serialize(&buffer1);
	
	/* Now I de-serialize the message, simulating what an IC receiver 
	 * would do.
	 */
	printf(">> Step 3: Receiver deserializes the message\n");
	ICMessage receiver;
	std::string buffer2;
	ICSerializerRapid rapidReceiver(&receiver, true);
	rapidReceiver.Deserialize(&buffer2);
	receiver.Dump();

	// If buffer1 and buffer2 are the same, it works. 
	printf(">> Step 3 and 1/2: Check the buffers\n");
	std::cout << "  1>> " << buffer1 <<std::endl;
	std::cout << "  2>> " << buffer1 <<std::endl;

	/* Once the receiver de-serialized a message, it can still change 
	 * its internal structure, and serialize the message again.
	 * Imagine the receiver receives a message, modifies its content 
	 * and sends it back to the sender. This is what we do now.
	 */
	printf(">> Step 4: Receiver modifies the message\n");
	receiver.GetClass("cnbi_mi", "0x301")->SetValue(0.01f);
	receiver.GetClass("cnbi_mi", "0x300")->SetValue(0.02f);
	receiver.GetClass("cnbi_errp", "ed")->SetValue(10);
	receiver.Dump();

	// Ok, I serialize the message to buffer3
	printf(">> Step 5: Receiver serializes the message\n");
	std::string buffer3;
	rapidReceiver.Serialize(&buffer3);
	
	// And the sender de-serializes it
	printf(">> Step 6: Sender deserializes the message\n");
	rapidSender.Deserialize(&buffer3);
	sender.Dump();

	// And now I play with loops
	std::string channel;
	printf(">> Step 7: loop\n");
	for(unsigned int j = 0; j < 1000; j++) {
		channel.clear();
		receiver.GetClass("cnbi_errp", "ed")->SetValue(j);
		rapidReceiver.Serialize(&channel);
		rapidSender.Deserialize(&channel);
	}
	receiver.Dump();
	sender.Dump();
	
	// I play with loops once more
	printf(">> Step 8: loop\n");
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
