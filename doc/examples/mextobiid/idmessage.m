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

[family_type, family_name] = idmessage_getfamilies();

message1 = idmessage_new();
idmessage_setdescription(message1, 'classifier');
idmessage_setfamilytype(message1, 0);
idmessage_setevent(message1, 781);
idmessage_setbidx(message1, 100);

serializer1 = idserializerrapid_new(message1);
buffer1 = idmessage_serialize(serializer1);
disp(['[example_idmessage] XML message1: ' buffer1]);

message2 = idmessage_new();
idmessage_setdescription(message2, idmessage_getdescription(message1));
serializer2 = idserializerrapid_new(message2);
idmessage_deserialize(serializer2, buffer1);
buffer2 = idmessage_serialize(serializer2);
disp(['[example_idmessage] XML message2: ' buffer2]);

message3 = idmessage_new();
serializer3 = idserializerrapid_new(message3);
idmessage_copy(message3, message1);
buffer3 = idmessage_serialize(serializer3);
disp(['[example_idmessage] XML message3: ' buffer3]);

idmessage_dumpmessage(message1)
disp(['[example_idmessage] Family name: ' ...
	idmessage_getfamily(message1)]);
disp(['[example_idmessage] Family type: '...
	num2str(idmessage_getfamilytype(message1))]);
idmessage_getfamilytype(message1);

client = idasclient_new();
idasclient_setbidx(client, 100);
disp(['[example_idmessage] Client size: ' num2str(idasclient_size(client))]);
idasclient_add(client, message1);
idasclient_add(client, message2);
idasclient_add(client, message3);
disp(['[example_idmessage] Client size: ' num2str(idasclient_size(client))]);


messageT = idmessage_new();
while(true)
	found = idasclient_getthis(client, messageT, [], []);
	idmessage_dumpmessage(messageT);
	if(found == false)
		break;
	end
end
%, idmessage_familytype('biosig'), 781);


idmessage_delete(message1);
idmessage_delete(message2);
idmessage_delete(message3);
idserializerrapid_delete(serializer1);
idserializerrapid_delete(serializer2);
idserializerrapid_delete(serializer3);
idasclient_delete(client);
