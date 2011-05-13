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


% As before, create a structure for the ErrP classifier, with
% - an iC message
% - an iC serializer 
% - a buffer to store the serialized messages
ErrP.message = icmessage_new();
ErrP.serializer = icserializerrapid_new(ErrP.message);
ErrP.buffer = '';
% Add a classifier named 'errp' to ErrP.message, using:
% - 'biosig' as label type 
% - 'prob' as value type.
icmessage_addclassifier(ErrP.message, ...
	'errp', 'ErrP classifier', ...
	icmessage_getvaluetype('prob'), icmessage_getlabeltype('custom'));
% Add two classes to the 'errp' classifier with label 'detected' and value 0.00
icmessage_addclass(ErrP.message, 'errp', 'detected', 0.00);


% Create a structure for the Fusion.
% We need an SMR and ErrP inputs, and the fusion output, and for each one we
% allocate:
% - an iC message
% - an iC serializer 
% - a buffer to store the serialized messages
%
% SMR input
Fusion.SMR.message = icmessage_new();
Fusion.SMR.serializer = icserializerrapid_new(Fusion.SMR.message);
Fusion.SMR.buffer = '';
% ErrP input
Fusion.ErrP.message = icmessage_new();
Fusion.ErrP.serializer = icserializerrapid_new(Fusion.ErrP.message);
Fusion.ErrP.buffer = '';
% Fusion output 
Fusion.M.message = icmessage_new();
Fusion.M.serializer = icserializerrapid_new(Fusion.M.message);
Fusion.M.buffer = '';

% Add a classifier named 'smr' and one 'errp' to Fusion.M.message, using
% the correct label and value types.
icmessage_addclassifier(Fusion.M.message, ...
	'smr', 'SMR classifier', ...
	icmessage_getvaluetype('prob'), icmessage_getlabeltype('biosig'));
icmessage_addclassifier(Fusion.M.message, ...
	'errp', 'ErrP classifier', ...
	icmessage_getvaluetype('prob'), icmessage_getlabeltype('custom'));
% Configure the classes
icmessage_addclass(Fusion.M.message, 'smr', '0x769', 0.50);
icmessage_addclass(Fusion.M.message, 'smr', '0x770', 0.50);
icmessage_addclass(Fusion.M.message, 'errp', 'detected', 0.00);

% Later on we will need a figure to plot the fused probabilities
figure(1);

% We will simulate 100 iterations of the BCI loop
for i = 1:100
	% Simulate we are running at 16 Hz
	pause(1.00/16);

	% Imagine the SMR and ErrP classifier are running in sync.
	% They would have the same frame number
	icmessage_setbidx(SMR.message, i);
	icmessage_setbidx(ErrP.message, i);

	% Generate a random SMR probability and set the values for the classes
	% of the 'smr' classifier
	p = rand();
	icmessage_setvalue(SMR.message, 'smr', '0x769', p);
	icmessage_setvalue(SMR.message, 'smr', '0x770', 1-p);

	% Again, generate a random ErrP probability and set the values for the
	% classes of the 'errp' classifier
	p = rand();
	icmessage_setvalue(ErrP.message, 'errp', 'detected', p);
	
	% Ok, we need a bit of imagination here.
	% Imagine the SMR and ErrP classifers serialize their iC messages and send
	% them via network to the fusion. 
	% 
	% This is how you serialize the messages, writing the XML messages in the 
	% buffers.
	SMR.buffer = icmessage_serialize(SMR.serializer);
	ErrP.buffer = icmessage_serialize(ErrP.serializer);
	% When the Fusion module receives the serialized messages, it would 
	% store it in its internal buffers
	Fusion.SMR.buffer = SMR.buffer;
	Fusion.ErrP.buffer = ErrP.buffer;
	% And then it would deserialize the buffers to the appropriate iC messages
	icmessage_deserialize(Fusion.SMR.serializer, Fusion.SMR.buffer);
	icmessage_deserialize(Fusion.ErrP.serializer, Fusion.ErrP.buffer);
	% When the Fusion receives messages, it must be sure they represent the
	% output of two classifiers that classified the very same frame.
	% So, we first extract the frame number and then we check if it is the 
	% same.
	% Keep in mind that in this example the frame number will always be the
	% same.
	fnEEG = icmessage_getbidx(Fusion.SMR.message);
	fnErrP = icmessage_getbidx(Fusion.ErrP.message);
	if(fnEEG == fnErrP)
		icmessage_setbidx(Fusion.M.message, fnEEG);
	else
		disp('Error!');
	end

	% At this point the Fusion merges the 'smr' and the 'errp'  messages
	% in a single one.
	icmessage_setvalue(Fusion.M.message, 'smr', '0x769', ...
		icmessage_getvalue(Fusion.SMR.message, 'smr', '0x769'));
	icmessage_setvalue(Fusion.M.message, 'smr', '0x770', ...
		icmessage_getvalue(Fusion.SMR.message, 'smr', '0x770'));
	icmessage_setvalue(Fusion.M.message, 'errp', 'detected', ...
		icmessage_getvalue(Fusion.ErrP.message, 'errp', 'detected'));

	% At this point we make some people happy by plotting the fused 
	% probability output.
	bar([icmessage_getvalue(Fusion.M.message, 'smr', '0x769') ...
		 icmessage_getvalue(Fusion.M.message, 'smr', '0x770') ...
		 icmessage_getvalue(Fusion.M.message, 'errp', 'detected')]);
	ylim([0 1]);
	xlim([0 4]);
	set(gca, 'XTickLabel', {'0x769', '0x770', 'detected'});
	title(['Probabilities after merging - Frame ' ...
		num2str(icmessage_getbidx(Fusion.M.message))]);
	
	% Imagine we have to send the merged probabilities to another module.
	% You need first to serialize the message and place the XML output in the
	% appropriate buffer.
	Fusion.M.buffer = icmessage_serialize(Fusion.M.serializer);
	% Then, we display the buffer.
	disp(Fusion.M.buffer);
end

% Let's deallocate all the serializers...
icserializerrapid_delete(SMR.serializer);
icserializerrapid_delete(ErrP.serializer);
icserializerrapid_delete(Fusion.SMR.serializer);
icserializerrapid_delete(Fusion.ErrP.serializer);
icserializerrapid_delete(Fusion.M.serializer);
% ... and all the messages
icmessage_delete(SMR.message);
icmessage_delete(ErrP.message);
icmessage_delete(Fusion.SMR.message);
icmessage_delete(Fusion.ErrP.message);
icmessage_delete(Fusion.M.message);
