TiD message format
==================

Example
-------
.. literalinclude:: sample_tid_event.xml
   :language: xml
   :encoding: utf-8
   :linenos:

TiD messages must have:
* a description
* a related frame
* an event type family
* the event or event code itself

Additionals:
* relative timestamp
* absolut timestamp
* ??

Possible Event families
-----------------------

* biosig
* BCI2000 (??)
* ??

**TODO: Is an "event subscription" needed or useful? E.g. the client sends a list of possible events + description
to the server --> traffic reduction (no description needed anymore).**

**Additions in the TiD msgs?**

