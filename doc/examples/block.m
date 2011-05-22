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

message = idmessage_new();

before.ts = idmessage_timestampget(message);
pause(2);
before.rc = idmessage_referenceget(message);
disp(['b> ' before.ts]);
disp(['b> ' before.rc])

%idmessage_timestamptic(message);
pause(1);
%idmessage_referencetic(message);
pause(1);
tic.ts = idmessage_timestampget(message);
tic.rc = idmessage_referenceget(message);
toc.ts = idmessage_timestamptoc(message);
toc.rc = idmessage_referencetoc(message);

disp(['>> ' tic.ts]);
disp(['>> ' tic.rc]);
disp(['t> ' num2str(toc.ts)]);
disp(['t> ' num2str(toc.rc)]);

idmessage_delete(message);
