#lsa
List only these files that you have access to

-

###How to use:

`lsa [-rwx] [-u username] [-v view] [dir]`

* `-rwx` -- which permissions should be tested (defaults to rw)
* `-u` -- user, which should have permissions (defaults to current user)
* `-v` -- view: l - long, s - short, m - minimal (defaults to l)  
  _note: only first character is parsed, so you can use eg. `-v long` or `-v letmeseeallinfo`_  
* `dir` -- specify a directory (defaults to .)

-

###Building & installing:
* **Linux**:  
  As the build process is pretty straightforward, no makefile is included.  
  This should work on every Linux distro out there (provided you have gcc installed):  
  `./make.sh`  
  To install, just copy `lsa` executable to some location included in `$PATH`.  
* **Mac OS X / *BSD**:  
  OS X and BSD systems might not have `linux/limits.h` header.  
  Although, the only values needed from this file are  
  `PATH_MAX` that is maximum path length (usually 4096) and  
  `NGROUPS_MAX` that is maximum supplemental GIDs available (usually 65536).  
  You can either find a file in your OS that defines these constants, or hardcode them yourself.  
  Rest of build and install process is the same as for Linux.
* **Windows**:  
  Windows does not support file permissions at all, so this program existence on Windows platform makes completely no sense. Also, Windows does not have at least half of necessary headers. Better go get Linux!
