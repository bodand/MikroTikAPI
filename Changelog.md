# MikroTik API Changelog

Versioning scheme is `<major>.<minor>.<patch>`. All version with the
same major version are backwards compatible. Minor versions introduce 
new features. Path versions, well, patch things.

Removal takes two steps: getting deprecated in a minor version, then removed 
when the next major version rolls along.

Minor and major versions get their own code-names, patch versions
append a number to the code-name of the version they are patching.

## VERSION v1.1.0 - Teius teyou

Version v1.1.0 is now ready. It mostly adds the documentation, with
a few changes that are backwards API compatible.
A bit of unification on the CMake build script now allows linking against 
the library both as a subproject, and an installed project in the same
way `mikrotik::mikrotikapi`. This breaks CMake backwards compatibility,
but that's not strictly a part of the C++ API, so I deem it fine.
This also renames the build target from `MikroTikApi.whatever` to
`mikrotikapi.whatever`, this was a bit more problematic to accept, but
the universal linking is more useful than the filename.

### Changed:
 - `api_handler::disconnect` now returns the return value of the
   function call that closed the socket.
 - The CMake target is to link against is universally `mikrotik::mikrotikapi`
   instead of `mikrotik::api` for subprojects and `mikrotik::MikroTikApi` for
   installed packages
   
### Added:
 - Doxygen documentation on all API entities
 - Sphinx powered read the docs page [here](https://mikrotikapi.rtfd.io):
   I sweat blood for this (see the commits on `develop`) 
   so go read it and tell me why it's completely unusable garbage

## VERSION v1.0.0 - Salvator rufescens

First release! 
API is now usable. 

### Added: 
 - `api_handler` connects to MikroTik devices
 - `api_handler` logs in with provided user
 - `api_handler` can send and receive sentences
 - DSL commands 
 - DSL sentences
 - API runs on WinSock and POSIX socks
