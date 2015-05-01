TiD Server
==========

The server is responsible for receiving and dispatching of incoming events.

Events without a frame number and/or timestamp get the actual values from the data acquisition.
An unknown block number is marked with "-1".
Therefore it needs to have access to the data acquisition system. It is suggested to include a
TiD server into the data acquisition like the TOBI SignalServer.

It provides the possibility to gather events received from connected clients for saving purposes.
These events are stored an can get saved elsewhere (e.g. in a simple text file or a .gdf file).

All TiD connections are running in separated threads to facilitate fast event processing, especially
on nowadays multi-core computer systems. In case of an error, the connection and the receiver thread 
get closed.

The TiD Server is listening at port 9001 by default.
To decrease delivery latency, Nagle's algorithm is deactivated (TCP_NODELAY flag is set).


TiD client
==========

A TiD client can send and receive TiD messages using the connection to the TiD server. All
communication is handled via this connection.

The client is responsible by itself for processing the TiD messages. It can simply ignore messages
it does not care for.

Every client, also processing incoming data (e.g. biosignals, classification results) has to include
the according frame number into an outgoing TiD message.
If a client is not in touch with the BCI processing chain (pre-processing, feature extraction,
classification, fusion, shared control), e.g. a P300 speller just sending events, has to leave the
frame number blank, the actual value will be inserted by the server before dispatching.


TiD Architecture
================

As mentioned at the beginning, TiD is acting in a bus oriented manner.
The following figure illustrates the principle:

.. figure:: TiD_scheme.pdf
   :scale: 90 %
   :alt: TiD principle illustrated on a BCI processing pipeline.

   
   
   
TiD Performance Tuning
======================

Unix/Linux:
-----------

In a Unix/Linux environment it is suggested to use a custom kernel.

Potential tuning options would be the modification of the timer frequency:
frequency to quickly react on events
::
  Symbol: HZ_1000 [=y]
  Location:
  -> Processor type and features
  -> Timer frequency (<choice> [=y])


And the modification of the preemption model:
::
  Symbol: PREEMPT_VOLUNTARY [=n]
  Location:
  -> Processor type and features
  -> Preemption Model (<choice> [=y])


As suggested by RedHat (http://developerblog.redhat.com/2015/02/11/low-latency-performance-tuning-rhel-7/, https://access.redhat.com/videos/898583), 
adjusting system settings can also affect network latency.

According to the RedHat sources mentioned above following additional settings for sysctl.conf should be meaningful
::
  net.core.wmem_max=12582912
  net.core.rmem_max=12582912
  net.ipv4.tcp_rmem= 10240 87380 12582912
  net.ipv4.tcp_wmem= 10240 87380 12582912


  net.core.busy_read=50
  net.core.busy_pull=50
  net.ipv4.tcp_fastopen=1
  kernel.numa_balancing=0

  kernel.sched_min_granularity_ns = 10000000
  kernel.sched_wakeup_granularity_ns = 10000000
  vm.dirty_ratio = 10
  vm.dirty_background_ratio = 3
  vm.swappiness=10
  transparent_hugepage=never
  kernel.sched_migration_cost_ns = 5000000


Windows:
--------

The windows scheduler acts, depending on operating system and host related configuration, in defined
time slots, also often called "quantum".
Such a quantum can get interpreted as a guaranteed amount of time for a certain thread/process.

Such quantums can get interrupted by operating system interrupts. As discussed in "The Windows Timestamp Project"
(http://www.windowstimestamp.com/description), Windows offers some undocumented functions to reconfigure the 
interrupt time resolution (like "*NtSetTimerResolution*").

The code within the performance tests of the TiD library already contains some demo code, setting
the scheduler to the maximum timer granularity, resulting in potential smaller delays, even in higher 
workload situations.

Unfortunately Microsoft does not offer as many latency tuning options as Linux. Some additional
options, which are unfortunately (partly) only available on Windows Server OS are described on the following iste:
https://technet.microsoft.com/en-us/library/jj574151.aspx

Thus, it is suggested to disable power saving options of the network adapter and further also disable 
power saving options from the CPU (like C-states).


TiD Performance Tests
=====================

The TiD library offers automated performance testing methods.
These methods are available in the "tid_tests" sub-project.


Within these tests, the individual latency then sending/receiving or dispatching a defined amount
of messages for variable TiD message lengths can get obtained.
Furthermore, the testing of the localhost as well as the network latency is possible.

This sub-project also offers Matlab script to load and analyze the test results. Histograms show the overall network latency. The individual transfer function for the individual 
plots were calculated as well, presenting the influence of the jitter when averaging data series, aligned 
on the basis of TiD events.

That way, every TiD user can determine the network environment latency conditions on his/her own to get a 
feeling for the potential limitations.

.. This sections shows results from performance tests of the TiD client/server system.
.. Latency was measured when sending, receiving and dispatching 10^6 packets on Windows 7 and Debian SID (May 2015).
.. Furthermore, the overall network latency over a Localhost and different network connections 
.. (Ethernet: 1 GBit and 100 MBit; Wifi: 811.g - 54 MBit) were analyzed as well.
.. 



