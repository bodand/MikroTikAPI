# MikroTikApi
A modern C++ wrapper for the MikroTik API.

---

A wrapper around the [MikroTik API](https://wiki.mikrotik.com/wiki/Manual:API).
Uses C++17, with all of its glory. The API is platform independent and can be 
compiled for Windows and POSIX socket implementations as well.

## Capabilities

The library allows creating a handle to a MikroTik device and execute arbitrary
API commands on it.
The login is abstracted away by the constructor of the `api_handler`, and uses the 
new RouterOS `v6.43` and up scheme. 

## DSL

In case you wanted to define commands to execute in code a DSL, albeit a rather crude
one, exists to help you out with that:
It allows the creation of `sentence` objects which are used when communicating 
with the MikroTik device. 

In `mikrotik::api::literals` there is a `_cmd` user defined literal for strings,
which creates a `command` object. A command can be extended by strings with the
/ operator. 
```c++
using namespace mikoritk::api::literals;
// ...
auto cmd = "system"_cmd / "resource" / "print";
```
This creates a command equivalent to `/system/resource/print`.
\[Note: C++ does not offer me a way to allow putting a forward slash before
things as is. This is sad.]  

If you only need the command, you can just use that as a sentence and send it.
However, if you need attributes or queries the DSL has you kind of covered.

To supply attributes to a command just use `operator[]`, with a string if you 
only need the name, or a braced list of two strings if you need a value as well:
```c++
using namespace mikoritk::api::literals;
// ...
auto snt = 
    ("log"_cmd / "info")
    [{"message", "Fun things are fun"}]; // => =message=Fun things are fun 
//  ["message"]  // => =message=
```
You can chain the `operator[]`s.
\[Note: The original plan was to use `operator=` but it's right associative which 
breaks all the things here. This is also sad, because we now need those parentheses around
the command DSL. :anger:]

You might want to use queries as well. While during the planning phase
these had better support in the DSL, because C++ sadly does not allow arbitrarily
meddling with operators' precedence and whatnot, they lost their high-class support.
Now they are used the same way as attributes, except they use `operator()` instead
of `operator[]` like so:
```c++
auto snt = 
    ("interface"_cmd / "print") 
    ({"type", "ether"})         # => ?type=ether
    ({"type", "vlan"})          # => ?type=vlan
    ("#|");                     # => ?#|
```
\[Warning: The equals sign is only present if a value is assigned
to the name. If you need equals nothing use the empty string as the value]

## RouterOS Support

The DSL supports queries which are present since RouterOS `v3.21`, however,
currently, only RouterOS `v6.43` and up are supported because the new login scheme
the library implements only works there.

## Compiler shenanigans
### MSVC

Because MSVC is special, it'd basically require MS' STL to `__declspec(dllexport)` almost everything,
which it does not do, therefore, building a DLL with MSVC may cause problems, so building as a static library
is advised.
GCC and Clang seem to both work with shared libraries, even on Windows.
I don't know how Clang-CL handles this issue.

### Clang

For some reason, on Windows for sure, possibly elsewhere as well, Clang creates duplicate symbols
when using an already installed `{fmt}`. To get around this, the project will always install a `{fmt}`
for itself when using Clang, even if there would be an applicable `{fmt}` package on the system.

## Licensing

__MikroTikApi__ is licensed under the BSD3 license. For more information
see the provided *LICENSE* file.

__{fmt}__ is used as a dependency, and is licensed under its own license.
For more information see the provided *LICENSE.fmt* file.

__Catch2__ is used as a test dependency, and is licensed under the Boost Software License.
For more information see the provided *LICENSE.boost* file.
