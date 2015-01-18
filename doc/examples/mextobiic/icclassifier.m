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

% Add a 'cnbi_mi' classifier to the message
icmessage_addclassifier(message, ...
	'cnbi_mi', ...
	'CNBI MI Classifier', ...
	icmessage_getvaluetype('prob'), ...
	icmessage_getlabeltype('biosig'));
icmessage_dumpmessage(message);

% Add 10 MI classes to the 'cnbi_mi' classifier
icmessage_addclass(message, 'cnbi_mi', '0x300', 0.10);
icmessage_addclass(message, 'cnbi_mi', '0x301', 0.10);
icmessage_addclass(message, 'cnbi_mi', '0x302', 0.10);
icmessage_addclass(message, 'cnbi_mi', '0x303', 0.10);
icmessage_addclass(message, 'cnbi_mi', '0x304', 0.10);
icmessage_addclass(message, 'cnbi_mi', '0x305', 0.10);
icmessage_addclass(message, 'cnbi_mi', '0x306', 0.10);
icmessage_addclass(message, 'cnbi_mi', '0x307', 0.10);
icmessage_addclass(message, 'cnbi_mi', '0x308', 0.10);
icmessage_addclass(message, 'cnbi_mi', '0x309', 0.10);
icmessage_dumpmessage(message);

% Do not forget to clean up
icmessage_delete(message);
icserializerrapid_delete(serializer);
