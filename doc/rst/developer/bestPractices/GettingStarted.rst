===============
Getting Started
===============

It is recommended that all Chapel users and developers get acquainted
with the project via the following steps:

* Start taking notes as you undertake these steps in order to track
  any suggestions, points of confusion, or areas for possible
  improvement.  Since Chapel is open-source, you should feel empowered
  to help improve anything that you find that would benefit from it.
  You'll only be new to Chapel once (and the rest of us aren't
  anymore), so this is the best time to identify things that could be
  clearer for new users.

* This `chapter about Chapel
  <https://chapel-lang.org/publications/PMfPC-Chapel.pdf>`_ is
  probably the best document for getting a broad overview of the
  history of the project, the motivation for the language, and a
  high-level overview of its features.  For updates on what's happened
  since that chapter was published, it may be helpful to refer to:

  - the CUG 2018 paper or presentation, "Chapel Comes of Age..."
    available from https://chapel-lang.org/papers.html

  - the "State of the Project" talk from the most recent `CHIUW
    workshop <https://chapel-lang.org/CHIUW.html>`_

  - the `release notes <https://chapel-lang.org/releaseNotes.html>`_
    from the most recent Chapel release.


* Download the official Chapel release from `GitHub
  <https://github.com/chapel-lang/chapel/releases>`_ and put yourself
  in the shoes of a Chapel user (alternatively, you can work from the
  development copy of the git repository where you found this README
  -- there are minor differences in organization between the two and
  the trunk subsumes everything in the release.
  ``$CHPL_HOME/README.devel`` exists to help map between the two
  views).

  Start from the top-level ``QUICKSTART.rst`` to build the compiler,
  run it, and execute the resulting binary.  Follow as many of the
  **"What's next?"** steps as possible in order to explore the
  contents of the release and familiarize yourself with the language,
  compiler, documentation, and example programs as presented to the
  end-user.

* To learn the language, your best bets are to read the Chapel chapter
  mentioned above, read through the release's primer examples (located
  in ``test/[release/]examples/primers``), watch or browse a `Chapel
  presentation <https://chapel-lang.org/presentations.html>`_, or
  refer to the `learning Chapel page
  <https://chapel-lang.org/learning.html>`_.

* Familiarize yourself with the `language specification
  <https://chapel-lang.org/docs/language/spec/index.html>`_.  This can
  be a fairly dry/tedious task, so most people will tend to find other
  tasks to interleave it with, like programming.  Nevertheless, we
  want to emphasize how important it is for developers to be familiar
  with the language spec, to get an understanding both of the
  language's scope and the state of the documentation today.

* Code up sample computations from scratch to get a feel for the
  language and compiler.  Get feedback on your codes from an
  experienced Chapel developer on ways to improve them from elegance
  and/or performance standpoints.

* Once you're ready to move from a user role into more of a developer
  role, refer to ``ContributorInfo.rst`` in this directory, which has
  a step by step introduction to contributing to the project.

