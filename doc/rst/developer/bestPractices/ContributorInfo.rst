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

#. `Discuss design`_ changes or big development efforts
#. `Set up a branch for development`_

   #. `Fork repo on github`_
   #. `Create new branch`_

#. `Develop and test contributions locally`_
#. `Submit pull request`_
#. `Test changes`_ (can be done by team at Cray)
#. `Mail chapel-developers@ for review and merge`_
#. `HOWTO/Git/GitHub details`_
#. `Policy details`_

.. _Discuss design:

Discuss design changes or big development efforts
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Before performing major work on the language, please open an issue on the git
repository and/or send an email to chapel-developers_ with a prefix of
``[Design]`` in the subject header for discussion with the community and to
ensure that you are aware of any parallel efforts in that area.

.. _Set up a branch for development:

Set up a branch for development
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This consists of two steps.  The first, `Fork repo on github`_, only needs to
happen once per developer.  The second, `Create new branch`_, should happen
for every new effort.

.. _Fork repo on github:

Fork repo on github
+++++++++++++++++++

These are expected to evolve over time as the governance of Chapel is migrated
from Cray to an external/community body (the major elements are likely to be
similar, though the specific people involved are likely to change and grow).

* `Set up a github account`_. The "Free" plan is sufficient for contributing to
  Chapel.

* Make sure you have configured your environment to work with git. See
  `initial_git_setup`_ instructions.

* Use the GitHub web interface to create a fork of the Chapel repo by visiting
  https://github.com/chapel-lang/chapel and clicking the 'Fork' button (see
  also `Fork the repo`_).  Then `configure your local git`_ and check out your
  fork

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

* Sign a Chapel contributor's agreement and mail it, with your GitHub
  ID, using the instructions here:

  https://github.com/chapel-lang/chapel/tree/master/doc/rst/developer/contributorAgreements/

.. _Create new branch:

Create new branch
+++++++++++++++++

* Develop your feature, bug fix, etc on your fork.  To create a new branch, use
  the following command:

.. code-block:: bash

    git checkout -b <branch_name>

  Using a concisely named branch is encouraged.

.. _Develop and test contributions locally:

Develop and test contributions locally
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Your contribution will take the form of a series of commits.  While including
sensible commit messages is a good idea, it is more important to have a good
merge message once the pull request is going in. Likewise, it is OK to have many
small commits that reflect the history of development rather than commits for
the feature.  See `Development commands`_ for how to perform some common
operations during development.

As you work, you will want to periodically bring in changes from the main Chapel
project to your feature branch:

.. code-block:: bash

    git fetch upstream
    git merge upstream/master

    # or:
    git pull upstream <branch_name>

    # with feature branch checked out:
    git merge [--no-ff] upstream/master

If there are conflicts, you will be asked to resolve them. Once the affected
files have been fixed, stage them with ``git add``, and then call ``git
commit`` to finish the merge process.


.. _Submit pull request:

Submit pull request
~~~~~~~~~~~~~~~~~~~

* `Submit a pull request`_ with your changes (make sure you have `synced with
  the main repo`_).

* In order for a pull request to be accepted and merged, it should a) pass
  testing and b) be reviewed by a member of the core Chapel team (currently, a
  member of the Cray Chapel team).

* It is considered good practice to keep PRs (pull requests) to a reasonable
  size. This ensures that the PR will be reviewed in a timely manner and will
  receive a higher level of attention per line of code during review.

  * When submitting a PR, the contributor should ask themselves if their
    contribution can be separated into smaller logical chunks or independent
    parts. Reviewers will also be pondering the same question and may request a
    break up of the contribution into smaller PRs.

  * Breaking up a PR can sometimes require a great deal of effort and
    creativity, and may not be feasible at all, due to the intertwined nature
    of the code.

  * Ideally, the size of the PR should be proportional to the expected value to
    the developer and user community. For example, a new module introduced as a
    1000-line PR is acceptable, while a set of new tests introduced as a
    1000-line PR is not.

.. _Test changes:

Test changes
~~~~~~~~~~~~

It may be helpful to read the documentation on Chapel's `Test System`_ before
writing or modifying tests.

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

.. _Test System: https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/bestPractices/TestSystem.rst

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

* See `Reviewer Responsibilities`_ for details on what performing a review on
  another contributor's code entails.

.. _HOWTO/Git/GitHub details:

HOWTO/Git/GitHub details
~~~~~~~~~~~~~~~~~~~~~~~~

.. _initial_git_setup:

Initial Git Setup
-----------------

Follow the GitHub directions to setup a new account.

  https://help.github.com/categories/53/articles

If you plan to use ssh to push/pull, setup SSH keys.

  https://help.github.com/categories/56/articles



.. _Configure your local git:

Configure your local git
------------------------

Here is the uncommented version of the commands:

.. code-block:: bash

    git config --global user.name 'Thomas Van Doren'
    git config --global user.email 'thomas@example.com'

    # linux/mac
    git config --global core.autocrlf input

    # windows
    git config --global core.autocrlf true

    # If using ssh keys, verify access to github. It should respond with a
    # message including your github username.
    ssh git@github.com

    # Clone your fork of the chapel repo!
    git clone git@github.com:<github_username>/chapel.git

    # Or, if using HTTPS instead of SSH.
    git clone https://github.com/<github_username>/chapel.git

    # Set up remotes for github
    cd chapel
    git remote add upstream https://github.com/chapel-lang/chapel.git
    # Make sure it works, get up-to-date without modifying your files
    git fetch upstream

.. _Create and switch to a feature branch:

Create and switch to a feature branch
-------------------------------------

.. _Development commands:

Development commands
--------------------

Stage a file/dir for commit:

.. code-block:: bash

    git add path/to/file

    # (sort of) similar to:
    svn add path/to/file

Delete a file/dir and stage the change for commit:

.. code-block:: bash

    git rm [-r] path/to/dir/or/file

    # similar to:
    svn delete path/to/dir/or/file

Move a file/dir:

.. code-block:: bash

    git mv orig/path/a.txt new/path/to/b.txt

    # similar to:
    svn move orig/path/a.txt new/path/to/b.txt

Copy a file/dir and stage target for commit:

.. code-block:: bash

    cp <src> <target>
    git add <target>

    # similar to:
    svn copy <src> <target>

Get the status of files/dirs (staged and unstaged):

.. code-block:: bash

    git status

    # similar to:
    svn status

Get the diff of unstaged changes:

.. code-block:: bash

    git diff

    # similar to:
    svn diff

Get the diff of staged changes (those that were staged with ``git add``):

.. code-block:: bash

    git diff --cached

Backing out unstaged changes:

.. code-block:: bash

    git checkout path/to/file/a.txt

    # similar to:
    svn revert path/to/file/a.txt

Committing staged changes:

.. code-block:: bash

    git commit [-m <message>]

    # similar to:
    svn commit [-m <message>]

Fixing a commit message:

.. code-block:: bash

    git commit --amend

.. note::

    This should only ever be done to commits that **have not been pushed** to
    a remote repository.

Un-do the last commit (leaving changed files in your working directory)

.. code-block:: bash

    git reset --soft HEAD~1

.. note::

    This should only ever be done to commits that **have not been pushed** to
    a remote repository.


More information on using git
-----------------------------

Additional docs available online at: http://git-scm.com/docs/

Git help pages can be viewed with:

.. code-block:: bash

    git help <command>

Other git commands
------------------

Update to HEAD:

(If you use this command on a feature branch, you'll just be updating to the
latest work stored on github. See `Develop and test contributions locally`_ for
how to update a feature branch with new changes from the main Chapel project)

.. code-block:: bash

    git pull

    # or:
    git fetch origin
    git merge origin/master # replace master with whatever branch you're on

    # similar to:
    svn update

Update to specific revision number:

.. code-block:: bash

    git checkout <commit sha1>

    # similar to:
    svn update -r<revision number>

To view "dirty" files, or all those files that are not tracked (includes
ignored files):

.. code-block:: bash

    git ls-files --others


If you've gotten your master branch mucked up but haven't pushed the branch
with errors to your remote fork, you can fix it with the following series of
commands:

.. code-block:: bash

   # This will save your old master state to a different branch name, removing
   # the name "master" from the list of branches you can access on your fork
   git branch -m <name for old, messed up master>

   # You will get a message indicating you are in a "detached HEAD state".  This
   # is expected (and desired).  Now the repository you are in is in line with
   # your fork's master branch.
   git checkout origin/master

   # This will save the state of the repository right now to a new branch, named
   # master.
   git checkout -b master

At this point, a `git push origin master` should work as expected.  Remember, do
not try this with a master branch that has been corrupted on your remote fork.

An alternate method, if you know or can easily find out the last commit that
should be kept:

.. code-block:: bash

   # on any branch that contains commits you do not want.
   git branch <new branch name>

   # do not use --hard if you wish to leave untracked files in your tree
   git reset --hard <last commit you want to keep>


Other Logging Commands
----------------------

To view commits grouped by author (for example, show me commits by author from
1.9.0.1 tag to now):

.. code-block:: bash

    git shortlog --numbered --no-merges

    # With commit sha1 and relative date:
    git shortlog --numbered --no-merges \
      --format='* %Cred[%h]%Creset %s %Cgreen(%cr)%Creset'

    # Set alias
    git config --global alias.sl \
      'shortlog --numbered --no-merges \
       --format=\'* %Cred[%h]%Creset %s %Cgreen(%cr)%Creset\''

    # Show commits by author between 1.8.0 and 1.9.0.1 releases:
    git sl 1.8.0..1.9.0.1


Finding a Pull Request by Commit
--------------------------------

Suppose you have figured out that a particular commit is causing a problem
and you'd like to view the pull request discussion on GitHub. You can go
to
  https://github.com/chapel-lang/chapel/commit/<commit-hash>
and GitHub shows the pull request number at the bottom of the commit message
complete with a link to the pull request page.



.. _Policy details:

Policy details
~~~~~~~~~~~~~~


.. _Reviewer Responsibilities:

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
