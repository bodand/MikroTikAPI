The MikroTik API
================

The MikroTik API is a pretty low level API, it relies on the target software
communicating with RouterOS through sockets. This tutorial does not indulge in
implementation details like how to calculate the length of a word (see below)
before sending, just provides a the information a user should know when using it.

Words
-----

The API's most basic building block is the word. A word is basically just a string.
It does not contain any metadata or anything of the sort, literally juts a string.

There are however different types of words:

 - The `command word` is the instruction the device is to execute
 - `Attribute words` contain additional information for the command
 - `API Attribute words` contain additional information for the device. The library
   currently does not support API attribute words.
 - `Query words` can be used to filter commands that may generate lots of output,
   for example printing the systems packages
 - The `reply word` is sent back by the MikroTik device as a response to the sent
   command, akin to HTTP responses

Command word
^^^^^^^^^^^^

All command words start with a forward slash, then an identifier which
is the command to execute.
Not all commands are executable, however. Some like ``/system``, for example,
are only provided to allow the creation of a hierarchy of commands. This
allows more complex commands can be created without requiring continously longer
and longer command names. This easily leads to the conclusion that commands can
be chained to create a final executable command to send. An example for such a
command is ``/system/resource/print``.

Attribute word
^^^^^^^^^^^^^^

An attribute word contains a key and a value. It is used by the API for all
kinds of information transport: to the device, by forming sentences with command
words; and from the device containing the reply it generates.
They are in the form ``=<name>=<value>`` where ``<name>`` is the name of the attribute
and ``<value>`` is, suprisingly, its value.

.. note::
    Neither the name nor the value part require enclosing quotation marks.
    The value may contain spaces and equals signs without requiring special escaping.

They are basically a pair of strings, and in most cases can be handled as such.

API attribute word
^^^^^^^^^^^^^^^^^^

They are special attribute words. They, instead of the leading equals sign,
start with a period: ``.<name>=<value>``. As of now (2020-07-02) the only
API Attribute word supported by RouterOS is ``tag``: all it does is tells the router
to add the given ``tag`` API attribute word to all sentences that are replies
to the original sentence with the tag. As of version ``v1.1.0`` this library
does not support attribute words as first class citizens and must be added as
raw string to a sentence.

Reply word
^^^^^^^^^^

A reply word contains the type of the reply sent by the router. It's always the
first word in a reply sentence and is of the form ``!<type>`` where type is one
of the following:

 - ``done``: indicates the end of the reply sentences
 - ``trap``: an error occurred during the execution of the command
 - ``fatal``: a fatal error occurred and the connection will now be terminated
 - ``re``: indicates that further replies are coming, and contains attributes
   which contain the produced output from the command's execution. Reply sentences
   with this reply word are called data replies.

Query word
^^^^^^^^^^

A query word is like an attribute word, except its job is to filter out the
responses that match a required criteria. For example ``/interface/print`` is
going to produce quite a lot of output, especially on larger routers with
many interfaces, however, we now are only interested in the vlan interfaces.
Whether an interface is vlan or not is encoded in a data reply's ``type`` attribute.

How are we to go about this? This is where query words come in: they have the
form ``?[<prefix>]<name>[=[<value>]]`` where brackets indicate being optional.
For a complete list of the query word syntax see the
`wiki <https://wiki.mikrotik.com/wiki/Manual:API#Queries>`_ or for a EBNF
grammar of my doing on the `query's <../library/query.html>`_ documentation.

With these you can tell the router to filter specific data replies based on
the attributes they contain eg. ``?type=vlan`` will only send the data
replies that have a ``type`` attribute and its value is ``vlan``.

Sentences
---------

Sentences are what's actually gets sent to the router and back. They are
made up of consequent words and end with the zero length word aka the empty string.

All sent sentences are made up of a command word, zero or more attributes
or queries and the zero length word.

Received sentences differ in that instead of a command word they contain a reply word,
and never contain queries.

Example sent sentence:

.. code-block:: null

    >>> /log/info
    >>> =message=A nice log message
    >>>

Example reply to that sentence:

.. code-block:: null

    <<< !done

... ok, here is a bit less useless example:

.. code-block:: null

    >>> /user/print
    >>>
    <<< !re    <
    <<< =.id=*1
    <<< =name=admin
    <<< =group=full
    <<< =address=
    <<< =last-logged-in=jul/02/2020 16:18:29
    <<< =disabled=false
    <<< =comment=system default user
    <<< !done  <

Words prefixed with (``>>>``) are part of the sent sentence,
words prefixed with (``<<<``) are parts of a reply sentence.
The beginning of the two reply sentences are marked with a (``<``), those
are the reply words.

Using the API
-------------

Now that you know all this using the library is not that hard. The process
is:

 1. You construct a sentence
 2. Send it to the router
 3. Router processes it
 4. Router sends reply sentence
 5. Check results: success/failure
 6. Is success use the data you got

That's it.
