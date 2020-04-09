===============
Getting Started
===============

It is recommended that all Chapel developers get acquainted with the
project via the following steps:

* Start taking notes as you undertake these steps in order to track
  suggestions, any points of confusion, and areas for possible
  improvement.  Since Chapel is open-source, you should feel empowered
  to help improve anything that you find which needs improving.
  You'll only be new to Chapel once (and the rest of us aren't
  anymore), so this is the best time to identify things that could be
  clearer for new users.

* The papers **"The State of the Chapel Union"** and **"A Brief Overview of
  Chapel"** (available at https://chapel-lang.org/papers.html) are the
  current best documents providing a broad overview of the history of
  the project, the motivation for the language, a high-level language
  overview, and the future.  The former is a bit more recent while the
  latter goes a bit more in-depth.  If you have time and motivation,
  reading both is likely to be useful and any duplication is likely to
  only reinforce important points.

* Download the official Chapel release from `GitHub <https://github.com/chapel-lang/chapel/releases>`_ and put
  yourself in the shoes of a Chapel user (alternatively, you can work
  from a trunk/ copy of the project where you found this README --
  there are minor differences in organization between the two and the
  trunk subsumes everything in the release.  ``$CHPL_HOME/README.devel``
  exists to help map between the two views).  

  Start from the top-level ``QUICKSTART.rst`` to build the compiler, run it, and
  execute the resulting binary.  Follow as many of the **"What's next?"**
  steps as possible in order to explore the contents of the release
  and familiarize yourself with the language, compiler, documentation,
  and example programs as presented to the end-user.

* To learn the language, your best bets are to read the **"Brief
  Overview of Chapel"** paper mentioned above, read through the
  release's primer examples (located in ``test/[release/]examples/primers``), 
  look through tutorial slides from the Chapel webpage (located at:
  https://chapel-lang.org/tutorials.html), and/or other Chapel overview
  presentations (located at https://chapel-lang.org/presentations.html).
  Most slide decks are better with a human delivering them, so see if
  you can find someone to walk you through any that aren't
  self-explanatory.

* Familiarize yourself with the language specification.  This can be a
  fairly dry/tedious task, so most people will tend to find other
  tasks to interleave it with, like programming.  Nevertheless, we
  want to emphasize how important it is for developers to be familiar
  with the language spec, to get an understanding both of the
  language's scope and the state of the documentation today.

* Code up sample computations from scratch to get a feel for the
  language and compiler.  Get feedback on your codes from an
  experienced Chapel developer on ways to improve them from elegance
  and/or performance standpoints.  If you need guidance on what to
  code up, ask for suggestions from the team; we usually have more
  computations to write than time to write them.

* If you haven't already, clone the `Chapel git repository
  <https://github.com/chapel-lang/chapel>`_ from GitHub. Get familiar with its
  organization, build and use the compiler. In most respects its directory
  structure is a superset of the official release, but in some cases there are
  differences. Refer to the top-level ``README.devel`` to see what some of these
  differences are.

* As a sub-step of the above, augment your shell environment:
   - set ``CHPL_HOME`` to your preferred or current tree,

   - incorporate the settings from ``$CHPL_HOME/util/setchplenv.bash``
     (or ``.csh``) e.g. by source'ing it,

   - add ``$CHPL_HOME/util`` and ``$CHPL_HOME/util/devel`` to your PATH,

   - set ``CHPL_DEVELOPER`` to **true** (see also ``CHPL_DEVELOPER.rst``).

* Get acquainted with the testing system which is a crucial technology
  for the development team for test-first development, pre-commit
  sanity checks, and nightly correctness and performance regression
  testing.  See ``TestSystem.rst`` in this directory for an introduction.

* Get acquainted with the compiler architecture via the compiler
  overview document in ``$CHPL_HOME/doc/rst/developer/compilerOverview/``
