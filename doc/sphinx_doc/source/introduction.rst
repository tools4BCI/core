Introduction
============

TiD (TOBI Interface D) describes a protocol to distribute events and markers used for brain-computer interface (BCI)
purposes. It is based on a client--server principle, whereby the server acts as a distributor,
dispatching incomming messages to every other connected client.
The principle is somehow similar to a bus.

TiD is mainly intended to facilita event distribution in a network envoronment.

It is not intended to replace any direct function calls wihin an established systems.
A direct function call will always be the fastest way to exchange information within on 
and the same process, so TiD will never be a replacement for that.

License
-------

The TiD library is licenced under the `LGPLv3`_.


Contact
-------

For further information please contact `c.breitwieser@tugraz.at`_.


.. _c.breitwieser@tugraz.at: c.breitwieser@tugraz.at
.. _michele.tavella@epfl.ch: michele.tavella@epfl.ch
.. _LGPLv3: http://www.gnu.org/licenses/lgpl.html

