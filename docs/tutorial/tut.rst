Using the library to interact with the API
==========================================

Headers
-------

The library is structured such that each header file
contains usually one type, possibly some free functions.

All headers reside in the ``mikrotik/api`` directory, so
when any of this documentation talks about a header file, its position is
relative to this directory.

The API
-------

This tutorial assumes you know how the MikroTik API works, and is a tutorial
about how to use the library to deal with the API. If you need a rough explanation
about how the API works check out the `API <api.html>`_ page.

Creating the API handler
------------------------

Before you can do anything you need to connect to the router. It requires
three pieces of information: the IP address of the device, the user to log
in as, and the password for that user.

.. note::
    As of ``v1.1.0`` the only supported IP standard is IPv4.

If you got yourself those the only thing you need now is to create an
``mikrotik::api::api_handler`` to handle the connection and all interaction
with the router.
An ``api_handler`` is constructable from tree strings: ip, name, password, in this order.

.. code-block:: cpp

    mikrotik::api::api_handler api(
        "192.168.1.1",
        "username",
        "password"
    );

.. note::
    As of ``v1.1.0`` the ip needs to be a c-string or a ``std::string_view``.
    This is a known defect.

.. warning::
    If anything goes awry the ``api_handler`` constructor will throw.
    This includes incorrect IP address, or incorrect credentials.
    For more information see the `docs <../library/api_handler.html#_CPPv4N8mikrotik3api11api_handler11api_handlerE10ip_addressNSt11string_viewENSt11string_viewE>`_.

Creating sentences
------------------

After you have set up the connection the second thing to do when using the
MikroTik API is to create a sentence that can be sent over to the router.
This can be done multiple ways using the library depending on you needs.

Using raw words
^^^^^^^^^^^^^^^

I'd guess this is the more likely situation, so we'll begin with it.
If you have some user input define a sentence, for example because you are making a
tty application, this is the route you'll have to take.

Since ``v1.1.0`` a ``mikrotik::api::sentence`` can be default constructed
to be the empty sentence. After that it provides a member function
``add_word`` which allows adding words verbatim to the sentence.

.. code-block:: cpp

    mikrotik::api::sentence snt;
    do {
        std::string word;
        std::getline(std::cin, word);
        snt.add_word(word);
    } while (!snt.words().back().empty());

.. note::
    If you are, for some ungodly reason, stuck on ``v1.0.0`` use a vector
    to aggregate the words an then construct the sentence using the
    iterator pair constructor.

So, if you want to create sentences from words that are generated strictly at
runtime use ``add_word`` to append them to the sentence.

Using the DSL
^^^^^^^^^^^^^

The library provides a rather crude, but at least functional DSL to
create sentences that are known at compile time. This allows sentence creation
to look better than ten continuous lines of ``snt.add_word(...)``.

The DSL relies on a user defined literal ``_cmd`` to create a command word.
This is inside the header ``command.hpp``, and requires ``using namespace mikrotik::api::literals``.

With that you can create a command object easily:

.. code-block:: cpp

    auto cmd = "system"_cmd; // => "/system"

But that's not all: you can use command objects to create more complex commands
by appending string using the forward slash operator:

.. code-block:: cpp

    auto cmd = "system"_cmd / "resource" / "print"; // => "/system/resource/print"

Commands are implicitly convertible to sentences so you can now send it,
as described bellow, but before that:

Attributes & Queries
^^^^^^^^^^^^^^^^^^^^

You may want to append additional attributes or queries to your command
to make a sentence.

Attributes and queries can be added using a pretty similar manner. The only
difference is the operator used.
Because one cannot overload the ``?:`` operator, nor change the associativity of
the assignment operator the DSL is rather crippled:

To add an attribute you can use the brackets operator with a pair of strings
or one string argument if you wish to omit the value:

.. code-block:: cpp

    auto snt = cmd
                [{"attribute-name", "value"}] // => =attribute-name=value
                ["valueless-attribute"]        // => =valueless-attribute=
    ;

Something similar can be played with queries: use the function call operator
with a pair of strings or one string argument to omit the value:

.. code-block:: cpp

    auto snt = cmd
                ({"query-name", "value"})  // => ?query-name=value
                ("valueless-query")         // => ?valueless-query
    ;

.. warning::
    Queries do not append the equals sign if a value is not specified.
    To make a query like this ``?query=`` add the empty string as the value.


Sending sentences
-----------------

If you have your sentence, say in ``snt``, and your ``api_handler``, again, say
in ``api``, you can easily send the sentence:

.. code-block:: cpp

    api.send(snt);

That's it. The ``api_handler`` will make sure that all sentences will be sent correctly.

Receiving replies
-----------------

Getting the reply for your sentence is just as easy:

.. code-block:: cpp

    mikrotik::api::reply rep = api.read();

.. caution::
    After you send a sentence make sure to check for the reply sentences. If you
    do not, it'll stay in buffer until it is read, for example when trying to read
    after the next send, and getting rubbish values.
    I know because login originally had that bug, so every reply was pushed back
    one reply. It isn't fun.

A reply contains two things: 1) a reply::type about the type of reply you got;
and 2) the additional attributes of the reply word in a vector of strings.

Type may be any of the following ``done``, ``trap``, ``fatal``, or ``re``.
