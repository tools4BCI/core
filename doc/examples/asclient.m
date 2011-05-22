%   Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
%
%   This file is part of the mexctobiid wrapper
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

% + Create a client
% Imagine the module is actually at frame 1000
% Imagine we are a "fusion" module, that is:
% - after the classifier
% - before the feedback 
% A module before the fusion might have an higer frame number.
% A module after the fusion might have a lower frame number.
% I say, 'might', because I imagine the fusion might take longer 
% than 1/f (where f is the processing frequency of the BCI) to 
% process a given frame an move to the next.
% In theory, fusion and classification are really quick modules, so 
% this scenario should not happen.
client = idasclient_new();
idasclient_setbidx(client, 1000);	% Client is at frame 1000

% + Fill up the message cue
% This simulates the fact that we receive iD messages
% from the broadcast. In reality, we should first read the messages
% from a socket and deserialize them. 
% - messageC comes from the classifier (same frame number)
% - messageF comes from the feedback (one frame before) 
% - messageD comes from a buggy module that takes more than 1/f to process the
%   current frame. 
messageC = idmessage_new();
idmessage_setdescription(messageC, 'classifier');
idmessage_setbidx(messageC, 1001);
idmessage_setfamilytype(messageC, idmessage_familytype('biosig'));

messageF = idmessage_new();
idmessage_setdescription(messageF, 'feedback');
idmessage_setbidx(messageF, 999);
idmessage_setfamilytype(messageF, idmessage_familytype('biosig'));

messageD = idmessage_new();
idmessage_setdescription(messageD, 'buggymodule');
idmessage_setbidx(messageD, 990);
idmessage_setfamilytype(messageD, idmessage_familytype('biosig'));

% Imagine we receive:
% - two biosig events from the classifier (1 and 2) 
% - three biosig events from the feedback (7, 8 and 9)
idmessage_setevent(messageC, 1); idasclient_add(client, messageC);
idmessage_setevent(messageC, 2); idasclient_add(client, messageC);
idmessage_setevent(messageF, 7); idasclient_add(client, messageF);
idmessage_setevent(messageD, 666); idasclient_add(client, messageD);
idmessage_setevent(messageF, 8); idasclient_add(client, messageF);
idmessage_setevent(messageF, 9); idasclient_add(client, messageF);

% At this point, our client should have in queue 5 messages, received,
% as we are imagining, within the last frame
disp(['[example_asclient] Queued messages: ' num2str(idasclient_size(client))]);

% Goodie. Now it's time to show the concept of Time-Warping in BCI design
% Imagine you want to get all the messages coming from the previous modules.
% I will also create a serializer object to print the content of the messages in
% XML format.
message = idmessage_new();
serializer = idserializerrapid_new(message);
disp('[example_asclient] Dequeing message from previous modules:');
while(true)
	found = idasclient_getprev(client, message, [], []);
	if(found == false)
		break;
	end
	buffer = idmessage_serialize(serializer);
	disp(buffer);

end

% Now we dequeue all the messages coming from the modules afterwards in the
% chain
disp('[example_asclient] Dequeing message from next modules:');
while(true)
	found = idasclient_getnext(client, message, [], []);
	if(found == false)
		break;
	end
	buffer = idmessage_serialize(serializer);
	disp(buffer);
	
	if(idasclient_getbidx(client) - idmessage_getbidx(message) > 1)
		disp(['  Warning: message is too delayed! ' ...
			'Module "' idmessage_getdescription(message) '" is too slow!']);
	end
end
