Design principle
================

TiD is designed to distribute BCI events to mutiple clients, based on a client--server system.
If a client creates an event, this event is sent to the TiD server, which dispatches it to all
connected clients. To ensure proper timing every TiD message is equipped with a frame number
corresponding to the respective frame the client was processing when the event occured. Additional
relative and absolute timestamps (in microseconds) are included into a TiD message, to provide inter-frame
accuracy (assign an event to samples inside a frame).
If a client is not processing data, the actual frame number is inserted by the TiD server before
distributing the event.
Therfore a TiD server needs some communication with the data acquisition system (e.g. TOBI Signal Server
using TiA [TiA]_).

To ensure events being always synchronous with the data, every processing module/step **has to foreward
the frame number** of the actual data.


.. [TiA] C. Breitwieser and C. Eibel, “TiA – Documentation of TOBI Interface A,”
   ArXiv e-prints, Mar. 2011. (http://arxiv.org/abs/1103.4717)


Frame Number
------------

A group (or block) of samples being aquired together.

e.g. the data acquisition source is acquiring data with 500 Hz and provides data in blocks of 10
samples ... every frame has 10 samples


Connection principle
====================

A TiD server has to provide a TCP port on which clients can create a connection.
Each client gets its own connection to the server.

Via this connection TiD messages are sent to the server and distributed to all other clients.

Some important remarks:

1. The protocol is in the style of HTTP (line structured text messages)
2. The messages are encoded in UTF-8
3. The message is split into lines which are terminated by the 0x0A character (also known as ``\n``, "line feed" or <LF>)
4. Some messages contain additional XML-structured content which is UTF-8 encoded
5. All characters are case sensitive!


TiD Message Structure
---------------------

Each message which is send from the client to the server or vice versa is structured as followed:

1. Version line
2. Command line
3. Optional content description line
4. An empty line
5. Optional xml-structured content

Example:
::

  TiD 1.0\n
  CheckProtocolVersion\n
  \n

Example with additional xml-structured content
::

  TiD 1.0\n
  Event\n
  Content-Length: 79\n
  \n
  <?xml version="1.0" encoding="UTF-8"?>
  <tobiid version="0.0.2.0" description="feedback" frame="98" family="biosig" event="781"/>


Reply Messages
--------------

**QUESTION: Should we implement reply msgs to inform a client if he sent an unreadable message?**



Message Types
-------------

A TiA 1.0 server implementation has to support the following commands:

* Check protocol version
* Event
* **???**

Check Protocol Version
^^^^^^^^^^^^^^^^^^^^^^

**Related with reply msgs -- to clarify!**

This message may be used by the client to check if the server understands the messages the client wants to send.
The server has to respond with an OK message if it understands messages of the given protocol version.

Representation:
::

  TiD 1.0\n
  CheckProtocolVersion\n
  \n

Server responses either with an OK or an error message.


Event
^^^^^

This message is used to transmit events to the server and to the clients (by the server).

Representation:
::

  TiD 1.0\n
  Event\n
  Content-Length: [Length of XML Content in Bytes]\n
  \n
  <tobiid version="0.0.2.0" description="feedback" frame="98" family="biosig" event="781"/>


**TODO: How to deal with timestamps and the TiD message version? Include <xml version ... -- but where?**



