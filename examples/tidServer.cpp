
/*
    This file is part of TOBI Interface D (TiD).

    TOBI Interface D (TiD) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TOBI Interface D (TiD) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TOBI Interface D (TiD).  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#include <iostream>
#include "libtid/tid_server.h"

using namespace TiD;
using namespace std;

//-------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
  int port = 9001;

  try
  {
    TiDServer test_server;
    test_server.bind (port);
    test_server.start();

    cout << endl << ">>";

    while(1)
    {
      cout << "Sleeping for 0.5s";
      usleep(500000);

      vector<IDMessage> msgs;
      test_server.getLastMessages(msgs);

      cout << " ... got " << msgs.size() << " messages" << endl;

    }

    test_server.stop();


  }
  catch(std::exception& e)
  {
    cerr << "Exception caught: "<< e.what() <<  endl;
  }
  catch(...)
  {
    cerr << "Unknown exception caught!"<< endl;
  }

  return 0;
}
