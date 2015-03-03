Contributor Info
================

Repository
----------

The git repository for the project is hosted on Github at
`chapel-lang/chapel`_. Anyone can read the repository. It is open source!

See the `Developer Workflow`_ instructions below for setting up a github
account, signing the contributor agreement, and submitting pull requests.

Developer Workflow
------------------

Overview:

#. `Fork repo on github`_
#. `Create new branch`_
#. `Submit pull request`_
#. `Test changes`_ (can be done by team at Cray)
#. `Mail chapel-developers@ for review and merge`_

.. _Fork repo on github:

Fork repo on github
~~~~~~~~~~~~~~~~~~~

These are expected to evolve over time as the governance of Chapel is migrated
from Cray to an external/community body (the major elements are likely to be
similar, though the specific people involved are likely to change and grow).

* `Set up a github account`_. The "Free" plan is sufficient for contributing to
  Chapel.

* Make sure you have configured your environment to work with git. See
  :ref:`Initial Git Set up <initial_git_setup>` instructions.

* If you will need commit/push access to the main repository,
  `chapel-lang/chapel`_, send a request including your github username to
  chapel_admin _at_ cray.com.

.. note::

    You do not need commit/push access to the main repo in order to contribute
    code. Reviewers on the core team can pull, review, and merge your pull
    requests.

* If you're working on a long-term effort, announce it on the
  chapel-developers_ to make sure toes are not being stepped on, work is not
  being pursued redundantly, etc.  Similarly, fundamental changes to the
  language or architecture should be circulated to the chapel-developers_
  and/or chapel-users_ lists to make sure effort is not wasted.

* If your work will require committing any third-party code that you are not
  developing yourself (or code that you've developed as a standalone package),
  alert the chapel-developers_ mailing list of this as, presently, such code
  packages must be approved by Cray leadership before being committed.

* Sign a Chapel contributor's agreement, located at the URL below, and send it
  to your contact at Cray (ask for one at the chapel_admin _at_ cray.com list
  if you don't).

  https://github.com/chapel-lang/chapel/tree/master/doc/developer/contributorAgreements/

* `Fork the repo`_.

.. _Create new branch:

Create new branch
~~~~~~~~~~~~~~~~~

* Develop your feature, bug fix, etc on your fork. Using a concisely named
  branch is encouraged.

.. _Submit pull request:

Submit pull request
~~~~~~~~~~~~~~~~~~~

* `Submit a pull request`_ with your changes (make sure you have `synced with
  the main repo`_).

* In order for a pull request to be accepted and merged, it should a) pass
  testing and b) be reviewed by a member of the core Chapel team (currently, a
  member of the Cray Chapel team).

.. _Test changes:

Test changes
~~~~~~~~~~~~

* Testing your patch:

  * Any addition/change to the Chapel test system should pass testing when that
    test/directory is run with ``start_test`` (and performance tests should
    also pass testing for ``start_test -performance``).

  * Changes to the Chapel implementation should not cause
    regressions. Developers are responsible for doing a degree of testing
    that's appropriate for their change (see guidance below) and then can rely
    on nightly regression testing to worry about the full cross-product of
    configurations.

    * At a minimum, patches should pass correctness testing for the full test/
      directory hierarchy for:

      * ``CHPL_*_PLATFORM=linux64``
      * ``CHPL_*_COMPILER=gnu``
      * ``CHPL_COMM=none``
      * ``CHPL_TASKS=<default>``

    * Most developers will start by focusing on a subdirectory of tests that
      exercise the features they changed, or test/release/ as a suite of tests
      that exercises a rich and important slice of the language.

    * Changes that are likely to affect multi-locale executions should also be
      tested against tests that exercise multi-locale capabilities with
      ``CHPL_COMM=gasnet``.  A common subset is: ``test/release/``,
      ``test/multilocale/``, and ``test/distributions/``.

    * Changes that are likely to cause portability issues should be tested
      against different platforms and compilers to avoid fallout in the nightly
      testing to the extent possible.

  * Note that the quickest way to do testing is to use the parallel testing
    system across a large number of workstations.  If you have limited testing
    resources available to you, you can request that a member of the core
    Chapel team help.

.. _Mail chapel-developers@ for review and merge:

Mail chapel-developers@ for review and merge
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* When you believe your pull request is ready for review, send it to a member
  of the core Chapel team (or chapel-developers_ if there isn't an obvious
  person to review it). Even the developers that have write access to the
  Chapel repository need to have all non-trivial changes reviewed. Developers
  who have been given write access can merge trivial changes (e.g. small bug
  fixes, documentation changes) without review.

.. note::

    Ideally, someone should volunteer to review your pull request within a day
    or two. If this doesn't happen, feel free to make some noise. Ideally the
    review should take place within a few days, though timing may vary
    depending on other deadlines.

* Iterate with the reviewer until you're both satisfied. If you can't come to
  agreement, one of you should bring other developers (individually or via
  chapel-developers_) into the conversation to get a broader opinion (one of
  the jobs of the reviewer is to serve as a proxy for other developers, or to
  bring those developers into the conversation if they feel unqualified to do
  so).

* Once the pull request is approved, it can be merged. This can be done by
  either the reviewer or developer (given sufficient permissions), as decided
  between the two of them.

.. note::

    Recall that while git history can be rewritten, it is both difficult and
    does not remove any local copies that have been pulled down. So be very
    careful not to commit anything that you might regret later (e.g., sensitive
    code, code owned by anyone other than yourself). Ideally, the review will
    catch such issues, but the ultimate responsibility is on the developer.

* In short order, a smoke-test will be run against the commit to make sure that
  nothing basic has been broken by it.  Monitor the
  chapel-test-results-regressions_ mailing list to make sure that nothing
  breaks.

* For the day or two after the commit has gone in, check the
  chapel-test-results-regressions_ mailing list to ensure that there are no new
  failures (or that they are due to a commit other than yours).  Use the
  chapel-developers_ mailing list if you are unsure (a member of the core
  Chapel team will be tasked with diagnosing any testing failures on any given
  night, but it's nice when developers notice the issue first themselves to
  save wasted effort).

* In general, all changes will go through a pull request and have a review. We
  have intentionally made the process as lightweight as possible, so it can be
  used for changes big and small alike.


Reviewer Responsibilities
-------------------------

* If you're reviewing a commit from a developer outside the Chapel core
  team, be sure they have signed the contributor's agreement (see the
  `Developer Workflow`_ instructions for this).  If the developer cannot
  or will not sign the agreement, bring the situation to the attention
  of the Chapel project leadership.

  Care may need to be taken when committing third-party code that
  originates from a different git[hub] repository.  As an example, in
  one case in the past we brought in a copy of an outside commit that
  had originally been made in the git repository belonging to one of our
  third-party packages.  We did that by using git-am to commit a copy of
  their raw commit (in git-send-mail format) to the appropriate
  third-party directory in the Chapel repository.  For the commit in our
  repo, their developer was listed as the author, but the Chapel core
  team member who did the Chapel commit was listed as the contributor.
  Had we instead split the original commit apart into its constituent
  meta-information and patch parts and committed just the patch using
  git-apply, the Chapel core team member would have been listed as both
  author and contributor.  In the end it didn't matter because although
  the outside developer couldn't sign our contributor's agreement, their
  IP attorneys decided that given their license (which was BSD), their
  commit constituted publishing the work rather than contributing it,
  and what Chapel did with it afterward was not their concern.  Also, we
  would have picked up the same commit the next time we updated our
  third-party release of that package.  Nevertheless, this gives an
  example of how tricky this kind of situation can be, and shows why
  decisions may need to be made (or at least understood) at a high
  level.

.. _chapel-developers: chapel-developers@lists.sourceforge.net
.. _chapel-test-results-regressions: chapel-test-results-regressions@lists.sourceforge.net
.. _chapel-users: chapel-users@lists.sourceforge.net
.. _chapel-lang/chapel: https://github.com/chapel-lang/chapel
.. _Set up a github account: https://help.github.com/articles/signing-up-for-a-new-github-account
.. _Fork the repo: https://guides.github.com/activities/forking/
.. _Submit a pull request: https://help.github.com/articles/using-pull-requests
.. _synced with the main repo: https://help.github.com/articles/syncing-a-fork
