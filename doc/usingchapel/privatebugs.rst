.. _readme-privatebugs:

===============================
Reporting Private Chapel Issues
===============================

Private bug reports can be mailed directly to the Chapel team at Cray Inc.
through: :disguise:`chapel_bugs@cray.com`


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
