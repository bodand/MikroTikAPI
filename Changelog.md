# MikroTik API Changelog

Versioning scheme is `<major>.<minor>.<patch>`. All version with the
same major version are backwards compatible. Minor versions introduce 
new features. Path versions, well, patch things.

Removal takes two steps: getting deprecated in a minor version, then removed 
when the next major version rolls along.

Minor and major versions get their own code-names, patch versions
append a number to the code-name of the version they are patching.

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
