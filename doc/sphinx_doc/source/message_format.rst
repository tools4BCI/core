TiD Message
===========

Structure
---------

Each message, which is send from the client to the server or vice versa, simply contains an XML
TiD message string as follows:

-------
Example
-------

.. literalinclude:: sample_tid_event.xml
   :language: xml
   :encoding: utf-8
   :linenos:
   
The TiD message has some mandatory and some optional attributes. These attributes 
and their intention are be described in succession.
   
Version
-------

**Mandatory**

A version attribute to avoid version incompatibilities during message processing.

The versions follow the $CURRENT.$REVISION.$MINOR.$BUGFIX schema and the following rules:

- If any "big" new features have get added, resulting in heavy interface changes,
  increment $CURRENT, and set all lower fields to "0".
- If any interfaces have been added, removed, or changed since the last update, 
  increment $REVISION, and set all lower fields to "0".
- If the library source code has changed at all since the last update then 
  increment $MINOR and set $BUGFIX to "0"
- If any bugs have got fixed since the last public release, then increment $BUGFIX.


Description
-----------

**Mandatory**

A short human readable description of the event.
Mainly intended to distinguish events more easily during manual event inspection
(e.g., "beep", "cue left", "flash row 5", ...)


Block
-----

The data sample (or potentially block), the event belongs to

Dependent on the data acquisition hardware, a group (or block) of samples might get acquired 
together (e.g., done by the g.USBAmp -- http://www.gtect.at).

In such a case, the data acquisition source is acquiring data with a defined sampling rate, but delivers
the data over an API in data blocks. For example, the sampling rare is 500 Hz and the acquisition system 
provides data in blocks of 10 samples. Thus, every block contains 10 samples and the blocks are delivered 
with a rate of 50 blocks per second.


Family
------

**Mandatory**

The family can be seen as a parent group, the event belongs to.
For example, the biosg project (http://biosig.sf.net) already defines a big amount of unique events 
for different areas (http://sourceforge.net/p/biosig/code/ci/master/tree/biosig4matlab/doc/eventcodes.txt). 

In a common environment, all events might usually be from the same family, so the occurrence of an event
clash or event misinterpretation is unlikely. However, to avoid such issues, an event family can get defined in TiD.

That way, a client can choose to react only on events from a known family. So even heterogeneous systems
with different event sources become possible.

Current event family definitions available in TiD:

* biosig *(to support events defined by the biosig project)*

* custom *(to support custom events, not being defined anywhere)*

The number of event families could also get extended in future to further support events fro other
prominent BCI systems, as mentioned below.

Potential other families for the future:

* BCI2000
* OpenViBE


Event
-----

**Mandatory**

In TiD an event is treated as a an occurence of a unique happening. Out of this reasons, events
are currently assigned to an event code, inspired by the biosig project, mentioned above.

The event is just an integer value.
This brings the advantage of fast event processing without the need to parse event strings.

In reverse, every event type (for example a "beep") needs to get an event code assigned.
It is a valid point of discussion to provide simple strings as a potential event as well.
The current implementation only provides the processing of integer values.


Absolute
--------

**Mandatory**

An absolute timestamp in microseconds since 1970-01-01 00:00:00.
Internally, the "gettimeofday()" method is utilized to obtain the time.

With this value, a proper synchronization of events becomes possible.
Please note: TiD does not offer a clock synchronization mechanism itself. The system
clocks of different computers need to get synchronized by other systems like 
NTP (network time protocol) or PTP (precision time protocol).


Relative
--------

**Mandatory**

This timestamp provides a timing value in microseconds, relative to a definable time point.
Currently, the start of the TiD server is used as a reference.

That way, a customizable point of time can get used as "0" and a dedicated timeserver as stratum.

Furthermore, the relative timestamp is also allowed to get reset at any point of time, for 
example to trace the processing pipeline. By resetting the relative timestamp when the event is created 
and reading out its value when the event is finally processed, tracing becomes easily possible, especially 
on local systems.


Source
------

**Optional**

The source provides an optional field to specify the event origin like a P300 speller, etc.
It is a simple descriptive string, which could be used to process events only from a defined source.


Value
-----

**Optional**

The value is an optional floating point number, defining a potential value related to the event.
For example, "BeepWithFrequency" as event and "1500" as value.

A more generic value typing system (e.g., to also allow sting) would be a meaningful extension.
However, it was just not needed up to now.

