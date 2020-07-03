MikroTTyk
=========

Synopsis
--------
.. code-block:: null

    >>> /log/info
    >>> =message=A nice log message
    >>>
    <<< !done

This is an example of how one might use MikroTikApi to build
a tty client to interact with MikroTik devices.

Source
------

This page contains a thorough explanation of what's happening and why in the
source. If you don't care just want to source it's available in the
``example/tty`` subdirectory in the repository.

Prerequisites
-------------

This example presumes that you have the following dependencies
installed and made available to whichever build system you happen to use.
This could be done without most of these, but to keep unrelated code to a minimum, these
dependencies are introduced.

 - `cxxopts <https://github.com/jarro2783/cxxopts>`_
 - `magic_enum <https://github.com/Neargye/magic_enum>`_
 - `{fmt} <https://github.com/fmtlib/fmt>`_

And, of course, the MikroTikApi library.

The Code
--------

CLI options
^^^^^^^^^^^

First we declare the command line options we want to allow the user to specify.
These are the IPv4 address to connect to, the user and the password associated
with the provided user. We use ``cxxopts`` to handle options, so this part is
not going to get more explanation: see the cxxopts `GitHub page <https://github.com/jarro2783/cxxopts>`_
for more information about this library.

.. code-block:: cpp

    int
    main(int argc, char** argv) {
        cxxopts::Options opts("mikrottyk",
            "A MikroTik device tty presenting the usage of MikroTikApi.");

        opts.add_options()
               ("u,user", "The user to log in as into the MikroTik device. [admin]",
                    cxxopts::value<std::string>()->default_value("admin"))
               ("p,password", "The password of the user to log in as into the MikroTik device. []",
                    cxxopts::value<std::string>()->default_value(""))
               ("4,ip4", "The IPv4 address of the MikroTik device. [192.168.88.1]",
                    cxxopts::value<std::string>()->default_value("192.168.88.1"))
               ("h,help", "Print this help message")
        ;

        auto ops = opts.parse(argc, argv);

        if (ops.count("help")) {
            fmt::print(opts.help());
            return 0;
        }

        std::string usr = ops["user"].as<std::string>();
        std::string passwd = ops["password"].as<std::string>();
        std::string ip4 = ops["ip4"].as<std::string>();

Now we have our user, password, and IPv4 address in ``usr``, ``passwd``,
and ``ip4`` respectively.

Reading sentences
^^^^^^^^^^^^^^^^^

After that we need to read the sentences the user wishes to send. To follow
existing practice on the MikroTik wiki we are going to prefix read sentences
with ``>>>`` and received reply sentences with ``<<<``.

Error handling
""""""""""""""

We put the whole thing in a try-catch because the library uses exceptions
to signal things breaking. At the catch clause the connection is already
thorn down by the destructor, so we just need to tell the user that what happened
and exit:

.. code-block:: cpp

    try {
        // ... see below
    } catch (const std::exception& ex) {
        fmt::print(ex.what());
        return -1;
    }

Setting up the handle
"""""""""""""""""""""

With this we are done with error handling, we just need the actual code to
communicate with the MikroTik device.
First we need an ``api_handler`` object. While this also presents
defaults, we already have our defaults handled by the CLI interface, so
we pass all arguments. Note that code hereafter goes in the try branch.

.. code-block:: cpp

    mikrotik::api::api_handler api(
        ip4.c_str(), // this is to allow implicit construction of the actual parameter type
        usr,
        passwd
    );

Reading user input
""""""""""""""""""

We want to run indefinitely so we now wrap everything in an infinite loop.
Use the infinite loop style of your choice; the example uses ``for (;;)``.
We also need a vector of strings to contain the words. We declare this outside
the loop because that can help us with unnecessary allocations.

.. caution::
    This was not benchmarked and as we know unbenchmarked optimizations might as
    well be pessimizations but this is not production code so at the moment I can't
    be bothered to benchmark it.

Now we need to read user input. A sentence is terminated by the empty string
so we are just going to leverage that knowledge and actually read until we encounter it.
Also we want a way to exit our application: this is not a immersive VR game
so it's not fun trying to trap the users.
Since I'm a proud member of the Cult of Vi, I went with ``:q`` to allow exiting
the tty session. If you are somehow unable to use Vi commands, just use whatever
command you want.
So, to get back on topic, we read from the user until we get the empty string, or
the exit command. Then we check if we left with the exit command, and if yes, we
just break from the infinite loop and the program exits.
Putting these steps together we get this:

.. code-block:: cpp

    std::vector<std::string> words;
    for (;;) {
        do {
            std::string inp;
            fmt::print(">>> ");
            std::getline(std::cin, inp);

            words.push_back(inp);
        } while (!(words.back().empty()
                   || words.back() == ":q"));
        if (words.back() == ":q")
            break;

        // ... read on
    }

Sending sentences
^^^^^^^^^^^^^^^^^

Sending the sentence
""""""""""""""""""""

Now we just need to create a sentence from the given strings and
send it. This is easy:

.. code-block:: cpp

    mikrotik::api::sentence snt(words.begin(), words.end());

    mikrotik::api::reply rep;
    api.send(snt);

We just create a sentence and a reply, and send the sentence easy as that.
The reply is going to be used in the next step.

Receiving replies
^^^^^^^^^^^^^^^^^

Receiving the reply
"""""""""""""""""""

After all this we now only need two things: read the reply
and clear the vector after we are done.
The second is pretty easy. The first is also easy but requires a bit
more code:
we once again indulge in a do-while loop and read until the reply type is a
data reply, as in the first received word is ``!re``. This is because this
way we also end the loop if we encounter a ``!trap`` or ``!fatal`` reply,
and a ``!re`` reply is always followed by something, if only a ``!done`` reply.
After reading we just print everything to the user, and we finished our job.

.. code-block:: cpp

    do {
        rep = api.read();
        fmt::print("<<< !{}\n", magic_enum::enum_name(rep.reply_type));
        for (const auto& resp : rep.attributes) {
            fmt::print("<<< {}\n", resp);
        }
    } while (rep.reply_type == rep.re);
    words.clear();
    // btw here is the end of the for (;;) loop

