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

% Make sure you add the correct path
%addpath('../../../build/mex/mextobiic/');

% Create a new message and a new serializer associated 
% with the message
message = icmessage_new();
serializer = icserializerrapid_new(message);

% Print all the supported class value types
[values_id, values_keys] = icmessage_getvalues();
fprintf(1, '[example_icmessage] IC value types:\n');
for i = 1:length(values_id)
	fprintf(1, '%3d/%s\n', values_id(i), values_keys{i});
end

% Print all the supported class label types
[labels_id, labels_keys] = icmessage_getlabels();
fprintf(1, '[example_icmessage] IC label types:\n');
for i = 1:length(labels_id)
	fprintf(1, '%3d/%s\n', labels_id(i), labels_keys{i});
end

% Add a 2-classes MI classifier to the message
icmessage_addclassifier(message, ...
	'cnbi_mi', 'CNBI MI Classifier', ...
	icmessage_getvaluetype('prob'), icmessage_getlabeltype('biosig'));
icmessage_addclass(message, 'cnbi_mi', '0x300', 0.60);
icmessage_addclass(message, 'cnbi_mi', '0x301', 0.40);
icmessage_dumpmessage(message);

% Add a 1-class ErrP classifier to the message
icmessage_addclassifier(message, ...
	'cnbi_erp', 'CNBI ErrP Classifier', ...
	icmessage_getvaluetype('prob'), icmessage_getlabeltype('class'));
icmessage_addclass(message, 'cnbi_erp', 'detection', 0.85);
icmessage_dumpmessage(message);

% Play with getters
value = icmessage_getvalue(message, 'cnbi_mi', '0x300');
disp(['[icmessage] Value: ' num2str(value)]);
icmessage_setvalue(message, 'cnbi_mi', '0x300', 1000);
value = icmessage_getvalue(message, 'cnbi_mi', '0x301');
disp(['[icmessage] Value: ' num2str(value)]);

% Serialize the message to buffer
buffer = icmessage_serialize(serializer);
disp(['[icmessage] Sender has: ' buffer]);

% Create a second message and a second serializer
message2 = icmessage_new();
serializer2 = icserializerrapid_new(message2);

% Deserialize the message
icmessage_deserialize(serializer2, buffer);
buffer2 = icmessage_serialize(serializer2);
disp(['[icmessage] Receiver got: ' buffer2]);

% Loop a bit, using tic/toc  and playing with the timestamps
disp('[icmessage] Stress test with 20s loop at 64Hz');
tic;
it = 0;
icmessage_relativetic(message);
icmessage_setbidx(message, 0);
tocs = [];
while(toc < 20)
	icmessage_absolutetic(message);
	icmessage_incbidx(message);
    buffer = icmessage_serialize(serializer2);
    icmessage_deserialize(serializer2, buffer);
    it = it + 1;
    pause(1/64);
	tocs(end+1) = icmessage_absolutetoc(message);
end
disp(['[icmessage] Frame number is: ' num2str(icmessage_incbidx(message))]);
disp(['[icmessage] Reference Tic: ' icmessage_relativeget(message)]);
disp(['[icmessage] Reference Toc: ' num2str(icmessage_relativetoc(message))]);

% Cleanup 
icmessage_delete(message);
icmessage_delete(message2);
icserializerrapid_delete(serializer);
icserializerrapid_delete(serializer2);

% Plot some tic/toc distribution
figure(1);
hist(tocs, 50);
xlabel('Tocs [ms]');
ylabel('Messages');
title('Tocs over iterations');
