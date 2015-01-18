%   Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
%
%   This file is part of the mexctobiic wrapper
%
%   The libndf library is free software: you can redistribute it and/or
%   modify it under the terms of the version 3 of the GNU General Public
%   License as published by the Free Software Foundation.
%
%   This program is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%   GNU General Public License for more details.
%
%   You should have received a copy of the GNU General Public License
%   along with this program.  If not, see <http://www.gnu.org/licenses/>.

clear all;

% Create a structure for the SMR classifier, with
% - an iC message
% - an iC serializer 
% - a buffer to store the serialized messages
SMR.message = icmessage_new();
SMR.serializer = icserializerrapid_new(SMR.message);
SMR.buffer = '';
% Add a classifier named 'smr' to SMR.message, using:
% - 'biosig' as label type 
% - 'prob' as value type.
icmessage_addclassifier(SMR.message, ...
	'smr', 'SMR classifier', ...
	icmessage_getvaluetype('prob'), icmessage_getlabeltype('biosig'));
% Add two classes to the 'smr' classifier with labels '0x769' and '0x770' and
% values 0.50
icmessage_addclass(SMR.message, 'smr', '0x769', 0.50);
icmessage_addclass(SMR.message, 'smr', '0x770', 0.50);


% As before, create a structure for the EMG classifier, with
% - an iC message
% - an iC serializer 
% - a buffer to store the serialized messages
EMG.message = icmessage_new();
EMG.serializer = icserializerrapid_new(EMG.message);
EMG.buffer = '';
% Add a classifier named 'emg' to EMG.message, using:
% - 'biosig' as label type 
% - 'prob' as value type.
icmessage_addclassifier(EMG.message, ...
	'emg', 'EMG classifier', ...
	icmessage_getvaluetype('prob'), icmessage_getlabeltype('biosig'));
% Add two classes to the 'emg' classifier with labels '0x300' and '0x301' and
% values 0.50
icmessage_addclass(EMG.message, 'emg', '0x300', 0.50);
icmessage_addclass(EMG.message, 'emg', '0x301', 0.50);


% Create a structure for the Fusion.
% We need an SMR and EMG inputs, and the fusion output, and for each one we
% allocate:
% - an iC message
% - an iC serializer 
% - a buffer to store the serialized messages
%
% SMR input
Fusion.SMR.message = icmessage_new();
Fusion.SMR.serializer = icserializerrapid_new(Fusion.SMR.message);
Fusion.SMR.buffer = '';
% EMG input
Fusion.EMG.message = icmessage_new();
Fusion.EMG.serializer = icserializerrapid_new(Fusion.EMG.message);
Fusion.EMG.buffer = '';
% Fusion output 
Fusion.F.message = icmessage_new();
Fusion.F.serializer = icserializerrapid_new(Fusion.F.message);
Fusion.F.buffer = '';

% Add a classifier named 'smr+emg' to Fusion.F.message, using:
% - 'custom' as label type 
% - 'prob' as value type.
icmessage_addclassifier(Fusion.F.message, ...
	'smr+emg', 'SMR+EMG fusion', ...
	icmessage_getvaluetype('prob'), icmessage_getlabeltype('custom'));
% Configure the classes, this time using 'right' and 'left' as custom labels
icmessage_addclass(Fusion.F.message, 'smr+emg', 'right', 0.50);
icmessage_addclass(Fusion.F.message, 'smr+emg', 'left',  0.50);

% Later on we will need a figure to plot the fused probabilities
figure(1);

% We will simulate 100 iterations of the BCI loop
for i = 1:100
	% Simulate we are running at 16 Hz
	pause(1.00/16);

	% Imagine the SMR and EMG classifier are running in sync.
	% They would have the same frame number
	icmessage_setfidx(SMR.message, i);
	icmessage_setfidx(EMG.message, i);

	% Generate a random SMR probability and set the values for the classes
	% of the 'smr' classifier
	p = rand();
	icmessage_setvalue(SMR.message, 'smr', '0x769', p);
	icmessage_setvalue(SMR.message, 'smr', '0x770', 1-p);

	% Again, generate a random EMG probability and set the values for the
	% classes of the 'emg' classifier
	p = rand();
	icmessage_setvalue(EMG.message, 'emg', '0x300', p);
	icmessage_setvalue(EMG.message, 'emg', '0x301', 1-p);
	
	% Ok, we need a bit of imagination here.
	% Imagine the SMR and EMG classifers serialize their iC messages and send
	% them via network to the fusion. 
	% 
	% This is how you serialize the messages, writing the XML messages in the 
	% buffers.
	SMR.buffer = icmessage_serialize(SMR.serializer);
	EMG.buffer = icmessage_serialize(EMG.serializer);
	% When the Fusion module receives the serialized messages, it would 
	% store it in its internal buffers
	Fusion.SMR.buffer = SMR.buffer;
	Fusion.EMG.buffer = EMG.buffer;
	% And then it would deserialize the buffers to the appropriate iC messages
	icmessage_deserialize(Fusion.SMR.serializer, Fusion.SMR.buffer);
	icmessage_deserialize(Fusion.EMG.serializer, Fusion.EMG.buffer);
	% When the Fusion receives messages, it must be sure they represent the
	% output of two classifiers that classified the very same frame.
	% So, we first extract the frame number and then we check if it is the 
	% same.
	% Keep in mind that in this example the frame number will always be the
	% same.
	fnEEG = icmessage_getfidx(Fusion.SMR.message);
	fnEMG = icmessage_getfidx(Fusion.EMG.message);
	if(fnEEG == fnEMG)
		icmessage_setfidx(Fusion.F.message, fnEEG);
	else
		disp('Error!');
	end

	% At this point the Fusion fuses the 'smr' '0x769' class value with the
	% 'emg' '0x300' one, and puts the resulting value in 'smr+emg' 'right'. 
	% Sometimes when I refer to this example I mention
	% "Bayesian fusion", but here I simply do averaging. 
	icmessage_setvalue(Fusion.F.message, 'smr+emg', 'right', ...
		0.5 * icmessage_getvalue(Fusion.SMR.message, 'smr', '0x769') + ...
		0.5 * icmessage_getvalue(Fusion.EMG.message, 'emg', '0x300'));
	% The same is done for the other class. Keep in mind this is just an 
	% example to show you how to use the methods, it woud be a bit redundant 
	% to perform this operation in real life.
	icmessage_setvalue(Fusion.F.message, 'smr+emg', 'left', ...
		0.5 * icmessage_getvalue(Fusion.SMR.message, 'smr', '0x770') + ...
		0.5 * icmessage_getvalue(Fusion.EMG.message, 'emg', '0x301'));

	% At this point we make some people happy by plotting the fused 
	% probability output.
	bar([icmessage_getvalue(Fusion.F.message, 'smr+emg', 'right') ...
		 icmessage_getvalue(Fusion.F.message, 'smr+emg', 'left')]);
	ylim([0 1]);
	xlim([0 3]);
	set(gca, 'XTickLabel', {'Right', 'Left'});
	title(['Probabilities after fusion - Frame ' ...
		num2str(icmessage_getfidx(Fusion.F.message))]);
	
	% Imagine we have to send the fused probabilities to another module.
	% You need first to serialize the message and place the XML output in the
	% appropriate buffer.
	Fusion.F.buffer = icmessage_serialize(Fusion.F.serializer);
	% Then, we display the buffer.
	disp(Fusion.F.buffer);
end

% Let's deallocate all the serializers...
icserializerrapid_delete(SMR.serializer);
icserializerrapid_delete(EMG.serializer);
icserializerrapid_delete(Fusion.SMR.serializer);
icserializerrapid_delete(Fusion.EMG.serializer);
icserializerrapid_delete(Fusion.F.serializer);
% ... and all the messages
icmessage_delete(SMR.message);
icmessage_delete(EMG.message);
icmessage_delete(Fusion.SMR.message);
icmessage_delete(Fusion.EMG.message);
icmessage_delete(Fusion.F.message);
