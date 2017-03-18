/*
    This file is part of the libTiD test routine.

    The libTiD test routine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The libTiD test routine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the libTiD test routine. If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "timing_values.h"

unsigned int NR_TID_MESSAGES = 50;
int NR_CLIENTS = 0;
unsigned int STATISTICS_WINDOW_SIZE = 5000;

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

using std::cout;
using std::cin;
using std::endl;

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

void printCurrentTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
  std::string str(buffer);

  std::cout << str << std::endl;
}

//-------------------------------------------------------------------------------------------------

int main( int argc, const char* argv[] )
{
  #ifdef WIN32
    setTimerResulution();
  #endif

  if(argc >1)
  {
    NR_TID_MESSAGES = boost::lexical_cast<unsigned int>(argv[1]);
  }

  if(argc > 2)
  {
    NR_CLIENTS = boost::lexical_cast<unsigned int>(argv[2]);
  }

  #ifdef WIN32
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
  #endif

  srand (time(NULL));
  std::cout << std::endl << "Build on " << __DATE__ << " at " << __TIME__ << std::endl;
  std::cout << "  ...  running " << NR_TID_MESSAGES << " messages on " << NR_CLIENTS << " clients ";
  std::cout << "with " << SLEEP_TIME_BETWEEN_MSGS << " between the messages." << std::endl<< std::endl;

  printCurrentTime();


  const UnitTest::TestList& allTests( UnitTest::Test::GetTestList() );

  if( argc > 3)
  {
    //if first arg is "suite", we search for suite names instead of test names
    //const bool bSuite = strcmp( "suite", argv[ 3 ] ) == 0;
    const bool bSuite = 0;

    //walk list of all tests, add those with a name that
    //matches one of the arguments  to a new TestList
    //const UnitTest::TestList& allTests( UnitTest::Test::GetTestList() );
    UnitTest::TestList selectedTests;
    UnitTest::Test* p = allTests.GetHead();
    while( p )
    {
      //for( int i = 3 ; i < argc ; ++i )
        if( strcmp( bSuite ? p->m_details.suiteName
                           : p->m_details.testName, argv[ 3 ] ) == 0 )
        {
          std::cout << "Adding test: " << p->m_details.testName << std::endl;
          p->m_nextTest = 0;
          selectedTests.Add( p );
        }
      p = p->m_nextTest;
    }

    //run selected test(s) only
    UnitTest::TestReporterStdout reporter;
    UnitTest::TestRunner runner( reporter );
    return runner.RunTestsIf( selectedTests, 0, UnitTest::True(), 0 );
  }
  else
  {

    std::cout << "Available tests:" << std::endl;

    const UnitTest::TestList& allTests( UnitTest::Test::GetTestList() );
    UnitTest::Test* p = allTests.GetHead();
    while( p )
    {
      std::cout << "Site: " << p->m_details.suiteName;
      std::cout << "  --  Test: " << p->m_details.testName << std::endl;

      p = p->m_nextTest;
    }

    char type;
    do
    {
        cout << "Run all tests? [y/n]" << endl;
        cin >> type;
    }
    while( !cin.fail() && type!='y' && type!='n' );

    if(type == 'y')
      return UnitTest::RunAllTests();

    return -1;
  }

  cout << "Test finished: " << endl;
  printCurrentTime();

}

//-------------------------------------------------------------------------------------------------

//#ifdef WIN32
//  SetPriorityClass( ptr->native_handle(), REALTIME_PRIORITY_CLASS);
//  SetThreadPriority(ptr->native_handle(), THREAD_PRIORITY_HIGHEST );
//  SetPriorityClass(GetCurrentProcess(),   REALTIME_PRIORITY_CLASS);
//#endif

//SKIP_TOBIID_SERIALIZE_TEST
//SKIP_TOBIID_DESERIALIZE_TEST
//SKIP_LIBTID_CLIENT_SEND_TEST
//SKIP_LIBTID_CLIENT_RECV_TEST
//SKIP_LIBTID_LPT_TEST
//SKIP_LIBTID_LOCALHOST_SEND_RECEIVE_TEST
//SKIP_LIBTID_REMOTE_SEND_RECEIVE_TEST
//SKIP_LIBTID_SERVER_DISPATCH_TEST
//SKIP_STREAM_PARSING_TEST

