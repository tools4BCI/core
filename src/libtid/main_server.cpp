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
#include <boost/lexical_cast.hpp>

#include "tid_server.h"

#ifdef WIN32
  #include <windows.h>

  #pragma warning(disable: 4005)
  #include <ntstatus.h>
  //#include <ntdef.h>
  #pragma warning(default: 4005)

  typedef long NTSTATUS;
  typedef NTSTATUS (NTAPI* pSetTimerResolution)(ULONG RequestedResolution, BOOLEAN Set, PULONG ActualResolution);
  typedef NTSTATUS (NTAPI* pQueryTimerResolution)(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);

#endif

using namespace TiD;
using namespace std;

//-------------------------------------------------------------------------------------------------

int setTimerResulution()
{
  #ifdef WIN32
    NTSTATUS status;
    pSetTimerResolution setFunction;
    pQueryTimerResolution queryFunction;
    ULONG minResolution, maxResolution, actualResolution;

    const HINSTANCE hLibrary = LoadLibrary(L"NTDLL.dll");
    queryFunction = (pQueryTimerResolution)GetProcAddress(hLibrary, "NtQueryTimerResolution");
    if (queryFunction == NULL)
    {
      printf("NtQueryTimerResolution is null (%d)\n", GetLastError());
      return 1;
    }

    queryFunction(&minResolution, &maxResolution, &actualResolution);
    printf("Win32 Timer Resolution:\n\tMinimum Value:\t%u\n\tMaximum Value:\t%u\n\tActual Value:\t%u\n\n", minResolution, maxResolution, actualResolution);

    setFunction = (pSetTimerResolution)GetProcAddress(hLibrary, "NtSetTimerResolution");
    if (setFunction == NULL)
    {
      printf("NtSetTimerResolution is null (%d)\n", GetLastError());
      return 1;
    }

    printf("Setting Timer Resolution to the maximum value (%d)...\n", maxResolution);
    status = setFunction(5000, TRUE, &actualResolution);
    if (status == STATUS_SUCCESS)
    {
      printf("Success! (Current resolution: %d)\n", actualResolution);

      queryFunction(&minResolution, &maxResolution, &actualResolution);
      printf("Win32 Timer Resolution:\n\tMinimum Value:\t%u\n\tMaximum Value:\t%u\n\tActual Value:\t%u\n\n", minResolution, maxResolution, actualResolution);

      return 0;
    }

    if (status == STATUS_TIMER_RESOLUTION_NOT_SET)
    {
      printf("Timer not set (Return Code: %d)\n", status);
      return 2;
    }

    printf("Failed, Return Value: %d (Error Code: %d)\n", status, GetLastError());
  #endif
  return -1;
}

//-------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
  string msg_queue2 = "q1";
  string msg_queue3 = "q2";

  int port = 9001;

   if(argc == 2)
  {
    port = boost::lexical_cast<int>(argv[1]);
    cout << "Listening on port: " << port << endl;
  }


  try
  {
    TiDServer test_server;
    test_server.bind (port);
    test_server.reserveNrOfMsgs(10000000);
    test_server.keepIncomingMessages(false);
    test_server.addSHMMessageQueue(msg_queue2);
    test_server.startMessageQueueProcessing(msg_queue2);

    test_server.addSHMMessageQueue(msg_queue3);
    test_server.startMessageQueueProcessing(msg_queue3);

    test_server.start();
    string str;
    cout << endl << ">>";

    while(cin >> str)
    {
        if(str == "q" || str == "quit" || str == "exit")
            break;
        else if(str == "r" )
        {
          std::vector<IDMessage> msgs;
          test_server.getLastMessages(msgs);

          std::cout << "Got " << msgs.size() << " messages" << std::endl;
        }
        else
            cout << "Command '" << str << "' not recognized!" << endl << ">>";
    }


    test_server.stop();
    test_server.stopMessageQueueProcessing(msg_queue2);
    test_server.closeMsgQueue(msg_queue2);

    test_server.stopMessageQueueProcessing(msg_queue3);
    test_server.closeMsgQueue(msg_queue3);

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
