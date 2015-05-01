Design principle
================

TiD is designed to distribute BCI events to multiple clients, based on a client--server system.
If a client creates an event, this event is sent to the TiD server, which dispatches it to all
connected clients. To ensure proper timing every TiD message is equipped with a block number
corresponding to the respective block the client was processing when the event occurred. Additional
relative and absolute timestamps (in microseconds) are included into a TiD message, to provide inter-frame
accuracy (assign an event to samples inside a frame).
If a client is not involved in data processing (and is therefore not aware of the actual block number),
the actual frame number is inserted by the TiD server before distributing the event.
Therefore a TiD server needs some communication with the data acquisition system (e.g. TOBI Signal Server
using TiA [TiA-Doc]_ [TiA-IEEE]_).

To ensure events being always synchronous with the data, every processing module/step should forward
the frame number of the actual data.


.. [TiA-Doc] C. Breitwieser and C. Eibel, "TiA -- Documentation of TOBI Interface A",
   ArXiv e-prints, Mar. 2011. (http://arxiv.org/abs/1103.4717)
.. [TiA-IEEE] C. Breitwieser, I. Daly, C. Neuper, and G. R. Müller-Putz, "Proposing a standardized protocol for raw biosignal transmission", IEEE. Trans.
   Biomed. Eng., vol. 59, no. 3, pp. 852-859, 2012.


Connection principle
====================

A TiD server has to provide a TCP port on which clients can establish a connection. Using an
TCP acceptor, this connection is then bound to a dedicated port on the TiD Server.
Each client gets its own connection to the server.

Via this connection TiD messages are sent to the server and distributed to all other clients.

Some important remarks:
1. The messages are encoded in UTF-8
2. All characters are case sensitive!






