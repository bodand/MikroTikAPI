FetchContent
============

If you happen to want a way to auto-install the library for your users then
there is CMake utility module to help you: ``FetchContent``.

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.11)

    include(FetchContent)

Used layout
-----------

This example assumes that the code for getting the dependencies is located
in a ``dependencies-ProjectName.cmake`` file which is included
in the ``CMakeLists.txt`` file which creates the target.

CMakeLists.txt
--------------

In the main CMakeLists, we included our ``dependencies-ProjectName.cmake``, either as a module,
or directly as a file.

Other than that the only thing we need is to link against the library, like with the manual
install method:

.. code-block:: cmake

    target_link_libraries(your-target
                             PRIVATE mikrotik::mikrotikapi)

Dependencies
------------

Now this is where the magic happens: first we include the FetchContent module:

.. code-block:: cmake

    include(FetchContent)

Then we go and get the git URL for this library. Let me help: ``https://github.com/bodand/MikroTikApi.git``.
Now we just go and tell CMake to get this project for us:

.. code-block:: cmake

    FetchContent_Declare(
        MikroTikApi
        GIT_REPOSITORY https://github.com/bodand/MikroTikApi.git
        GIT_TAG v1.1.0 # or any version you desire
    )

This tells CMake to where this library is found and which revision are we trying to use.
After this we just need to get the library itself:

.. code-block:: cmake

    FetchContent_MakeAvailable(MikroTikApi)

And now you can safely use the library, as seen in above in the ``CMakeLists.txt``.

Local search
------------

Optionally you may want to allow your users to use an already installed version of the library
instead of forcing them to download a version automatically.
This is like mixing the `Manual Installation <install.html>`_ with a fallback if they didn't
actually install it.

It's rather easy, we just wrap the ``FetchContent_xxx`` functions in an ``if-endif`` block,
after checking an ``find_package``:

.. code-block:: cmake

    find_package(MikroTikApi)
    if (NOT MikroTikApi_FOUND)
        message(STATUS "Local installation of MikroTikApi could NOT be found: installing from git")
        FetchContent_Declare(
            MikroTikApi
            GIT_REPOSITORY https://github.com/bodand/MikroTikApi.git
            GIT_TAG v1.1.0
        )
        FetchContent_MakeAvailable(MikroTikApi)
    endif()

.. note::
    You may want to specify version in the ``find_package`` call to make
    sure the user not only has the package, but also the required version.
    If you do this it is advised to add the version in the ``message`` command
    so they know which version was not found.

The library invested testing to make sure both of these versions end up creating the same target
which is to be linked against: this means the ``CMakeLists.txt`` file does not need to change.
