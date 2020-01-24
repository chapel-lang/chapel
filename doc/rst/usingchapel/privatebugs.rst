.. _readme-privatebugs:

=========================
Private Issues Guidelines
=========================

If for any reason you are uncomfortable reporting your bug in a public setting,
you may submit a private bug report.

Private issues can be sent to the core Chapel team at HPE using:
:disguise:`chapel_bugs@cray.com`

When submitting a private bug report, please include the following information:

.. note::

    **Summary of Problem**

    - What behavior did you observe when encountering this issue?
    - What behavior did you expect to observe?

    **Steps to Reproduce**

    - Please attach source code that will reproduce the problem.
        - To the extent possible, providing simplified programs demonstrating the
          problem will be appreciated.
        - If you are unable to submit code, please describe how to recreate the
          issue.
    - **How did you compile your program? e.g.** ``chpl foo.chpl -o foo``
    - How did you execute your program? e.g. ``./foo 42``
        - If any input files are required, please include them as well.

    **Configuration Information**

    - Output of ``chpl --version``:
    - Output of ``$CHPL_HOME/util/printchplenv --anonymize``:
    - Back-end compiler and version, e.g. ``gcc --version`` or ``clang --version``:
    - (For Cray systems only) Output of ``module list``:
