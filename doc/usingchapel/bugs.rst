.. _readme-bugs:

=====================
Reporting Chapel Bugs
=====================

If you encounter a bug, internal error, or unclear error message when
using Chapel, please do not hesitate to send a bug report to the
Chapel team.  Do so by sending mail to one of the following mailing
lists:

  * chapel-bugs@lists.sourceforge.net : for bugs that are suitable for `public archive <https://sourceforge.net/p/chapel/mailman/chapel-bugs/>`_
  * chapel_bugs@cray.com              : for bugs that the Chapel team at Cray Inc. should keep private

Please include the following information in your email:

  1. the version number of your Chapel compiler (``chpl --version``)
  2. the output of the ``$CHPL_HOME/util/printchplenv --anonymize`` script
  3. the back-end C compiler you are using and its version (e.g., ``gcc --version``)
  4. the output of ``module list`` (if using a Cray system)
  5. one of the following:

     a) preferably, a reduced program demonstrating the issue
     b) alternatively, the original program demonstrating the issue
     c) as a last resort, a description of how to recreate the issue

  6. any helper modules or input files required to reproduce the issue
  7. a description of the behavior you are seeing

A list of known bugs and unimplemented features can be found in
``$CHPL_HOME/STATUS``.  Even if you run into a known issue, we
encourage you to send us a bug report, particularly if it is impeding
your progress.  Depending on the issue, we may be able to suggest a
workaround or to prioritize the development of a fix.


Filing Futures
--------------

If you are a Chapel contributor and familiar with the `Chapel Testing
System`_, you can file bugs by submitting `Future tests`_.

.. _Chapel testing system: https://github.com/chapel-lang/chapel/blob/master/doc/developer/bestPractices/TestSystem.rst
.. _Future tests: https://github.com/chapel-lang/chapel/blob/master/doc/developer/bestPractices/TestSystem.rst#user-content-futures-a-mechanism-for-tracking-bugs-feature-requests-etc


JIRA Issues
-----------

The Chapel team is working to bring our `JIRA issues site`_ up to
date. Bugs can be viewed without creating an account.

.. _JIRA issues site: https://chapel.atlassian.net/projects/CHAPEL/issues/
