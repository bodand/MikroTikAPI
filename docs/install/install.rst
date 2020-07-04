CMake install
=============

One way of using the library is just pulling the sources, and building it.
This may seem old-fashioned, but may actually be the easiest way to go about this.

Building
--------

Sources
"""""""

First get the sources. The GitHub `repository <https://github.com/bodand/MikroTikAPI>`_
holds all there is to be held. Anything on the master branch should work, but if you want
you can of course go for a specific version.

Configure Options
"""""""""""""""""

After you have your sources you need to configure them with your old pal CMake.
This should work as is on most mentally sane systems (including Windows).
The options you can consider:

 - ``MikroTikApi_BUILD_TESTS:BOOL`` you might want to disable this. It's enabled by
   default if the project is the main project that's being configured.
 - ``MikroTikApi_BUILD_EXAMPLES:BOOL`` builds the example projects that come
   with the library. Default is off.
 - ``MikroTikApi_BUILD_DOCS:BOOL`` builds the documentation which you are reading
   right now. Default is off. This requires ``doxygen``, ``python``, and the
   ``sphinx``, ``breathe``, ``sphinx_rtd_theme`` pip packages.
 - ``MikroTikApi_BUILD_SHARED:BOOL`` builds a dll/so file instead of a static
   library. Default is on.

Install
"""""""

If you decided on your options, just call CMake and then the build system of your
choice. After the build succeeds you can run the ``install`` target (with elevated privileges)
which will install the library on your system.

Usage
-----

Find
""""

Now you are ready to use the library in your CMake projects.
First simply tell CMake to get this package from the system for you:

.. code-block:: cmake

    find_package(MikroTikApi REQUIRED)

The ``REQUIRED`` part is optional, if you want the library to possibly not exist.

Link
""""

Thankfully to modern CMake practices we now only need to link to the library
and everything will be set up that you can easily use the library how you wish.

.. code-block:: cmake

    target_link_libraries(your-target
                             PRIVATE mikrotik::mikrotikapi)

After this CMake will do everything that's needed to make things Just Work™.

Include & use
"""""""""""""

Now we are done with the CMake part of things. We just need to use the library.
The library's headers are inside ``mikrotik/api`` directory in your preprocessor's
include path. Knowing this you can easily use the library now:

.. code-block:: cpp

    #include <mikrotik/api/api_handler.hpp>

    int main() {
        mikrotik::api::api_handler handle;
        // do what you want to do
    }

