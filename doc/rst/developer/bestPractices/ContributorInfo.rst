Contributor Info
================

Repository
----------

The git repository for the project is hosted on Github at
`chapel-lang/chapel`_. Anyone can read the repository. It is open source!

See the `Developer Workflow`_ instructions below for setting up a github
account, signing the contributor agreement, and submitting pull requests.
This document contains a mixture of tips for git beginners and specific
Chapel workflow recommendations.

Developer Workflow
------------------

Overview:

#. `Discuss design`_ changes or big development efforts
#. `Set up a branch for development`_

   #. `Getting set up`_
   #. `Create new branch`_

#. `Develop and test contributions locally`_

   #. `Adding new tests`_

#. `Request feedback on your changes`_

   #. `Push your work`_
   #. `Ask for feedback early`_ (optional)
   #. `Submit pull request`_
   #. `Find a reviewer`_
   #. `Work with your reviewers`_

#. `Test changes`_
#. `Getting ready to merge`_
#. `Watch automatic testing`_ and address issues

`HOWTO/Git/GitHub details`_

`Policy details`_

.. _Discuss design:

Discuss design changes or big development efforts
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Before embarking on a big project, please open an issue on the git repository
and/or send an email to chapel-developers_ with a prefix of ``[Design]`` in the
subject header for discussion with the community and to ensure that you are
aware of any parallel efforts in that area.  If the change will impact the user
experience, it would be appropriate to additionally send this email to
chapel-users_.

.. _Set up a branch for development:

Set up a branch for development
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This consists of two sets of operations.  The first (`Getting set up`_) only
needs to happen once per developer.  The second (`Create new branch`_) should
happen for every new effort.

.. _Getting set up:

Getting set up
++++++++++++++

Note: these are expected to evolve over time as the governance of Chapel is
migrated from Cray to an external/community body (the major elements are likely
to be similar, though the specific people involved are likely to change and
grow).

#. `Set up a github account`_. The "Free" plan is sufficient for contributing to
   Chapel.

#. Make sure you have configured your environment to work with git. See
   `initial git setup`_ instructions.

#. Use the GitHub web interface to create a fork of the Chapel repo by visiting
   https://github.com/chapel-lang/chapel and clicking the 'Fork' button (see
   also `Fork the repo`_).  Then `configure your local git`_ and check out your
   fork

#. If you're working on a long-term effort, announce it on the
   chapel-developers_ mailing list to make sure toes are not being stepped on,
   work is not being pursued redundantly, etc.  Similarly, fundamental changes
   to the language or architecture should be circulated to the
   chapel-developers_ and/or chapel-users_ lists to make sure effort is not
   wasted.

#. Sign a Chapel contributor's agreement and mail it, with your GitHub
   ID, using the instructions here:

   https://github.com/chapel-lang/chapel/tree/master/doc/rst/developer/contributorAgreements/

* You do not need commit/push access to the main repo in order to
  contribute code.  See `Who has commit access`_ for questions.

* Third-party code requires additional approvals, see the policy details on
  `Third-party code`_.

.. _Create new branch:

Create new branch
+++++++++++++++++

Develop your feature, bug fix, etc on your fork.  To create a new branch, use
the `New branch command`_.  Using a concisely named branch is encouraged.

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
project to your feature branch (described in `Development commands`_), to avoid
code drift.

.. _Adding new tests:

Adding new tests
++++++++++++++++

You will probably need to create new tests for your feature. See `Test System`_
for more information on this process.

Any addition/change to the Chapel test system should pass testing when that
test/directory is run with ``start_test`` (and performance tests should also
pass testing for ``start_test -performance``).

.. _Request feedback on your changes:

Request feedback on your changes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _Push your work:

Push your work to your feature branch
+++++++++++++++++++++++++++++++++++++

Push your changes to your feature branch on GitHub will enable others to see
your work (see `How to push`_ for command details).  Note that if you have
already created a pull request from a feature branch, pushing your work to that
feature branch will update the pull request.

.. _Ask for feedback early:

Ask for feedback on your branch early (optional)
++++++++++++++++++++++++++++++++++++++++++++++++

Not ready to merge yours changes, but still want to see if your work is going in
the right direction?  Feel free to ask for early feedback!  Exposing the code is
generally done by:

- Pointing someone to your feature branch on Github, or
- Creating a `WIP PR`_ for your feature branch.  See the `Submit pull request`_
  section below for how to do this.

Discussion can take place in:
- the `WIP PR`_
- a separate Github issue
- the chapel-developers_ mailing list
- a private communication
- some other strategy agreed upon by all involved parties


.. _Submit pull request:

Submit pull request
+++++++++++++++++++

See `How to open a PR`_ for the sequence of steps necessary.

Contributors should be reasonably confident in the testing done on their code
before asking for a final review.  Should additional testing resources be
needed, you can request help from a member of the core Chapel team when creating
your pull request.

In working with your reviewers, you will no doubt change your pull request.
Just do your local development and then update your feature branch as in
`Push your work`_

It's a good idea to keep PRs `reasonably sized`_.

.. _Find a reviewer:

Find a reviewer
+++++++++++++++

* Once your PR is ready, you'll need to request a review.  If you know who you'd
  like to review it, @ mention them in a comment on the PR and ask them to have
  a look.  If you don't know their Github id, you can find them in the chat room
  or send them an email.  If you don't know who should review the change, send
  an email to the chapel-developers_ list requesting a review and linking to the
  PR.  Such an email should have a subject line starting with `[PR]`.

  Note: Ideally, someone should volunteer to review your pull request within a
  day or two. If this doesn't happen, feel free to make some noise. Ideally the
  review should take place within a few days, though timing may vary depending
  on other deadlines.

* See `Reviewer responsibilities`_ for details on what performing a review on
  another contributor's code entails.

.. _Work with your reviewers:

Work with your reviewers
++++++++++++++++++++++++

* Iterate with the reviewer until you're both satisfied.  One should generally
  try to do whatever their reviewer asks.  Sometimes, a reviewer will ask for
  something really hard.  Try to make sure they understand the magnitude of the
  request, and try to discuss if it's really necessary to do before merging.  If
  you can't come to agreement, one of you should bring other developers
  (individually or via chapel-developers_) into the conversation to get a
  broader opinion (one of the jobs of the reviewer is to serve as a proxy for
  other developers, or to bring those developers into the conversation if they
  feel unqualified to do so).


.. _Test changes:

Test changes
~~~~~~~~~~~~

If you did not have the resources to perform at least a full correctness test
run, this is the point at which a reviewer would do so for you.  Contributors
are expected to have verified any new tests work before asking a reviewer to do
this.

Details on how to run tests may be found at `Test System`_, and details on the
appropriate amount of testing before merging the final PR may be found at
`Testing guidance`_.

.. _Getting ready to merge:

Getting ready to merge
~~~~~~~~~~~~~~~~~~~~~~

Before the change can be merged, ensure:
- all design changes have been discussed
- the PR has been reviewed
- the contributor agreement has been signed
- the history in the PR does not contain anything not to be in the repo
- relevant configurations pass testing

* Once the pull request is approved, it can be merged. This can be done by
  either the reviewer or developer (given sufficient permissions), as decided
  between the two of them.  See `How to merge a PR`_ for steps to perform this.

  Note: It's not generally possible to completely remove a commit from git by
  the time it makes it in to the master branch. So be very careful not to commit
  anything that you might regret later (e.g., sensitive code, code owned by
  anyone other than yourself). Ideally, the review will catch such issues, but
  the ultimate responsibility is on the developer.

* If you are reviewing code from an external contributor without push
  privileges, please look through the `Reviewer responsibilities`_ once more
  before merging the change.

After the final version of the change has been agreed upon, the person making
the merge should follow the steps for `How to merge a PR`_.


.. _Watch automatic testing:

Watch automatic testing and address issues
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* In short order, a smoke-test will be run against the commit to make sure that
  nothing basic has been broken by it.  Monitor the
  chapel-test-results-regressions_ mailing list to make sure that nothing
  breaks.

* For the day or two after the commit has gone in, check the
  chapel-test-results-regressions_ mailing list to ensure that there are no new
  failures caused by your commit.  Use the chapel-developers_ mailing list if
  you are unsure (a member of the core Chapel team will be tasked with
  diagnosing any testing failures on any given night, but it's nice when
  developers notice the issue first themselves to save wasted effort).

.. _HOWTO/Git/GitHub details:

HOWTO/Git/GitHub details
~~~~~~~~~~~~~~~~~~~~~~~~

.. _initial git setup:

Initial Git Setup
+++++++++++++++++

Follow the GitHub directions to setup a new account.

https://help.github.com/categories/53/articles

If you plan to use ssh to push/pull, setup SSH keys.

https://help.github.com/categories/56/articles



.. _Configure your local git:

Configure your local git
++++++++++++++++++++++++

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
    # Change remote for upstream push to "no_push" 
    git remote set-url --push upstream no_push 
    # Optionally add remotes for commonly viewed branches
    git remote add <branch_owner_username> https://github.com/<branch_owner_username>/chapel.git

.. _New branch command:

New branch command
++++++++++++++++++

.. code-block:: bash

    git checkout -b <branch_name>

.. _Development commands:

Development commands
++++++++++++++++++++

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

Bring in changes from the main Chapel project:

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

If you want to understand the changes that occurred upstream, see
`Read commit messages`_ below.

Fixing a commit message:

.. code-block:: bash

    git commit --amend

Note: This should only ever be done to commits that **have not been pushed** to
a remote repository.

Un-do the last commit (leaving changed files in your working directory)

.. code-block:: bash

    git reset --soft HEAD~1

Note: This should only ever be done to commits that **have not been pushed** to
a remote repository.

.. _Read commit messages:

Read commit messages for changes from the main Chapel project
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

To view only the commits that happened on master (in other words, the old svn
commits and the merge commits for pull requests):

.. code-block:: bash

    git log --first-parent

    # or with line wrapping
    git log --first-parent | less

    # or including files changed
    git log --first-parent -m --stat

    # or similar to svn log
    git log --first-parent -m --name-status

More logging commands are described in `Other logging commands`_ below.

.. _How to push:

How to push
+++++++++++

.. code-block:: bash

    git push origin <branch_name>

    # or if you don't like typing your complicated branch name,
    # you can use this command to push the current branch:
    git push origin HEAD

    # if you forgot your branch name, you can get it by running
    git branch

    # it is the starred one...

.. _How to open a PR:

How to open a PR:
+++++++++++++++++

* `Submit a pull request`_ with your changes (make sure you have `synced with
  the main repo`_).

  To do this, after pushing your changes to your feature branch on GitHub,
  you can use the GitHub web interface to create a pull request. Visit

  ``https://github.com/<username>/chapel``

  and look for a "Compare & pull request" button for your feature branch.
  Alternatively, navigate to your feature branch, and click the green icon next
  to the branch dropdown to "Compare, review, create a pull request".

  Next, put in a message to your reviewer about the purpose of your pull request
  and give the pull request a useful title.  Your PR message will introduce the
  changes to reviewers and form the basis for the merge message.  See
  `Final merge message`_ for recommendations on what that commit message should
  look like.

  You will have to have signed a contributors agreement.  See https://github.com/chapel-lang/chapel/tree/master/doc/rst/developer/contributorAgreements/

  Your pull request will be available at a URL like:

  ``https://github.com/chapel-lang/chapel/pull/<number>``

  and you can discuss the patch with your reviewers there.

.. _How to merge a PR:

How to merge a PR:
++++++++++++++++++

If you have commit privileges (see `Who has commit access`_), navigate to the
pull request:

go to

https://github.com/chapel-lang/chapel/pulls

or

``https://github.com/chapel-lang/chapel/pull/<number>``

and click the friendly green button "Merge pull request" (it is possible to
merge the pull request from the command line also and the pull request page has
details). When you click "Merge pull request", you will need to enter a commit
message. See `Final merge message`_ for a reminder on what that commit message
should entail.

More information on using git
+++++++++++++++++++++++++++++

Additional docs available online at: http://git-scm.com/docs/

Git help pages can be viewed with:

.. code-block:: bash

    git help <command>

Other git commands
++++++++++++++++++

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


.. _Other logging commands:

Other logging commands
++++++++++++++++++++++

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
++++++++++++++++++++++++++++++++

Suppose you have figured out that a particular commit is causing a problem
and you'd like to view the pull request discussion on GitHub. You can go
to

``https://github.com/chapel-lang/chapel/commit/<commit-hash>``

and GitHub shows the pull request number at the bottom of the commit message
complete with a link to the pull request page.



.. _Policy details:

Policy details
~~~~~~~~~~~~~~

.. _Who has commit access:

Who has/needs commit access to the main repository?
+++++++++++++++++++++++++++++++++++++++++++++++++++

Core team members have commit access to the main repository.  Reviewers on the
core team can pull, review, and merge your pull requests.  Even the developers
that have write access to the Chapel repository need to have all non-trivial
changes reviewed. Developers who have been given write access can merge trivial
changes (e.g. small bug fixes, documentation changes) without review.

If you will need commit/push access to the main repository,
`chapel-lang/chapel`_, send a request including your github username to
chapel_admin _at_ cray.com.

.. _Third-party code:

Third-party code
++++++++++++++++

If your work will require committing any third-party code that you are not
developing yourself (or code that you've developed as a standalone package),
alert the chapel-developers_ mailing list of this as, presently, such code
packages must be approved by Cray leadership before being committed.

.. _Testing guidance:

Testing your patch
++++++++++++++++++
* Changes to the Chapel implementation should not cause regressions. Developers
  are responsible for doing a degree of testing that's appropriate for their
  change (described in the following bullets) and then can rely on nightly
  regression testing to worry about the full cross-product of configurations.

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

  * Changes that are likely to cause portability issues should be tested against
    different platforms and compilers to avoid fallout in the nightly testing to
    the extent possible.

* Note that the quickest way to do testing is to use the parallel testing system
  across a large number of workstations.  If you have limited testing resources
  available to you, you can request that a member of the core Chapel team help.

.. _Test System: https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/bestPractices/TestSystem.rst

.. _WIP PR:

Work-in-progress pull requests (WIP PRs)
++++++++++++++++++++++++++++++++++++++++

This is a special kind of pull request that is not yet intended to be merged.
Such PRs are created to take advantage of what the Github PR interface provides,
such as public comment history and quick links between the WIP PR and other
related issues and pull requests.  They allow the developer to get early
feedback on a change.

The status of the WIP PR should be clearly stated, including what steps need to
be taken before the PR is ready for final review.

It is generally advisable to "close" such PRs until they are ready for
final review and testing, as their development can span a large period of time
and would thus add clutter to the list of open PRs.

It is perfectly acceptable to abandon such PRs (especially in favor of a cleaned
up version of the code) when the git history becomes too large, so long as a
link to the original PR is provided when the change is eventually merged.

.. _reasonably sized:

Pull request guidance
+++++++++++++++++++++

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

.. _Final merge message:

Final merge message
+++++++++++++++++++

- start with a single topic line with at most 75 characters
- then have a blank line
- then have a more detailed explanation including motivation for the
  change and how it changes the previous behavior
- use present tense (e.g. "Fix file iterator bug")
- manually wrap long lines in the explanation to 75 or 80 characters

It is good practice for the pull request title to match the topic line, and the
detailed explanation to match the pull request description.  For contributors
without merge permissions, this will allow the reviewer to merge changes with
minimal additional support.


.. _Reviewer responsibilities:

Reviewer responsibilities
+++++++++++++++++++++++++

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
