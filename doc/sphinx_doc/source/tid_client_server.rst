TiD Server
==========

The server is responsible for receiving and dipatching of incomming events.

Events without a frame number and/or timestamp get the actual values from the data acquisition.
Therefore it needs to have access to the data acquisition system. It is suggested to include a
TiD server into the data acquisition.

It provides the possibility to gather latest events received from connected clients for saving purposes.

**QUESTION: How to deal with unknown events (especially in case of saving events to a files)?
Should we also save the description somewhere?**

**OTHER QUESTION: How to deal with timestamps?**


TiD client
==========

A TiD client can send and receive TiD messages using the connection to the TiD server. All
communication is handled via this connection.

The client is responsible by itself for processing the TiD messages. It can simply ignore messages
it does not care for.

Every client, also processing incomming data (e.g. biosignals, classification results) has to include
the according frame number into an outgoing TiD message.
If a client is not in touch with the BCI processing chain (pre-processing, feature extraction,
classification, fusion, shared control), e.g. a P300 speller just sending events, has to leave the
frame number blank, the actual value will be inserted by the server before dispatching.

**QUESTION: How to deal with timestamps?**