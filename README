===============================
Chapel Compiler Release: README
===============================

This is the 1.9.0 release of the Chapel compiler, intended to give
potential users a look at what we're doing and the opportunity to
provide us with feedback.  See the LICENSE file for the release's
licensing terms.


-----------
Quick Start
-----------

1) Make sure that your shell is in the directory containing this
   README file.  For example:

        cd ~/chapel-1.9.0


2) Build the compiler and runtime libraries using:

        gmake

   or if your default make is GNU make compatible (as on Cygwin), use:

        make


3) Set up your environment to use Chapel.

   If you use...                               then type...

     ...a bash-compatible shell (bash)         source util/setchplenv.bash
     ...a csh-compatible shell (csh, tcsh)     source util/setchplenv.csh
     ...the fish shell (fish)                  . util/setchplenv.fish
     ...the Bourne shell (sh)                  . util/setchplenv.sh

   You must be in the top-level "chapel-*" directory for these command
   scripts to work properly.  Note that there is no requirement to use
   these scripts long-term, they are merely designed to get new users
   up and running quickly.

   If you use a different shell, refer to doc/README.chplenv for
   information about what environment settings you'll need to make in
   order to use Chapel satisfactorily (and if you're able to create an
   equivalent util/setchplenv.* script for your shell, please mail a
   copy of it to us at chapel_info@cray.com for inclusion in our next
   release).


4) To ensure you have installed Chapel properly, you can optionally
   run an automatic sanity check using few example programs:

        gmake check

   or:

        make check


5) Compile an example program using:

        chpl -o hello examples/hello.chpl


6) Execute the resulting executable:

        ./hello


------------
What's next?
------------

For more detailed information about...      refer to...

  ...changes since the last release          ... CHANGES
  ...implementation status                   ... STATUS
  ...performance status                      ... PERFORMANCE
  ...Chapel's file and directory structure   ... README.files
  ...prerequisites for using this release    ... doc/README.prereqs
  ...setting Chapel environment variables    ... doc/README.chplenv
  ...building the compiler                   ... doc/README.building
  ...using the compiler                      ... doc/README.compiling
  ...executing Chapel programs               ... doc/README.executing
  ...reporting bugs                          ... doc/README.bugs

  ...Chapel modes for emacs and vim          ... etc/README

  ...example Chapel programs                 ... examples/README
  ...a quick reference for Chapel syntax     ... doc/quickReference.pdf
  ...the Chapel language specification       ... doc/chapelLanguageSpec.pdf

  ...executing programs on multiple locales  ... doc/README.multilocale
  ...how Chapel tasks are mapped to threads  ... doc/README.tasks

  ...using Chapel on a specific platform...
     ...a Cray system                        ... doc/platforms/README.cray
     ...Cygwin over Windows                  ... doc/platforms/README.cygwin
     ...an IBM system                        ... doc/platforms/README.ibm
     ...an SGI Altix system                  ... doc/platforms/README.sgi

  ...calling C code from Chapel              ... doc/technotes/README.extern
  ...other user and technical notes          ... doc/README

  ...third-party software that we use        ... third-party/README
