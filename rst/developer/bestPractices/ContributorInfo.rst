Contributor Info
================

The git repository for the project is hosted on Github at
`chapel-lang/chapel`_. Anyone can read the repository. It is open source!

This document contains a mixture of tips for git beginners and specific
Chapel workflow recommendations.

Below are instructions for setting up a github account, developing a
feature, and submitting pull requests.

.. note:: A `contributor license agreement`_ must be signed before any contributing pull requests can be merged.

Overview:

#. `Choosing a task`_

#. `Design`_

    #. `When design discussion is needed`_
    #. `Creating a design issue`_
    #. `Leading a design discussion`_

#. `Development`_

    #. `Get set up`_
    #. `Create new branch`_
    #. `Develop and test contributions locally`_

       #. `Add new tests`_


#. `Contributing changes`_

   #. `Push your work to your feature branch`_
   #. `Ask for feedback on your branch early (optional)`_
   #. `Submit pull request`_
   #. `Find a reviewer`_
   #. `Work with your reviewers`_
   #. `Before merging`_
   #. `After merging`_

#. `Other useful information`_

   #. `Chapel git workflow`_
   #. `Policy details`_
   #. `Chapel release process`_



.. _Choosing a task:

Choosing a task
---------------

If you do not already know what task to work on, the `Contributing`_ page has
some tips for finding a task.

It is important to communicate with other people before working on a task. This
will help address design questions before starting implementation and will
avoid multiple people working on the same task simultaneously.

If an issue exists for the task you are working on, you should always comment
on that issue to let others know you are working on it. If an issue does not
exist for the task you have chosen, you should open an issue first. In many
cases, you will need to discuss the design of the interface or implementation
before starting development.  See the `Design`_ section for more details on
this.


.. _Contributing: https://chapel-lang.org/contributing.html

.. _Design:

Design
------

.. _When design discussion is needed:

When design discussion is needed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Design discussion is necessary when proposing language or standard library
changes. It is also necessary for any large or architectural changes.

In many cases it is reasonable to start development without a full design
review, but such efforts should be open to changing once the design is being
discussed.

If your task does not require a design issue, you may skip ahead to the
`Development`_ section.


.. _Creating a design issue:

Creating a design issue
~~~~~~~~~~~~~~~~~~~~~~~

The design issue should aim to clearly and concisely present some design
decisions. Here are some strategies to make a good design issue:

- Summarize issue at the top, preferably include concise code example(s).
- Try not to make initial issue too long in order to lower the barrier for
  someone getting involved.
- Clearly define the problem this proposal aims to solve.
- It can be useful to consider alternative solutions to the problem and
  describe the trade-offs among them.
- A design issue should have two developers not involved in the effort to review.
- To find reviewers, send an email to chapel-developers_.

  * Use a prefix of ``[Design]`` in the subject header.
  * Include a short summary of the topic, potentially including motivation
    and/or an example.
      - This should not be a copy/paste of the issue contents.
  * Include a link to your issue.
  * Ask for people to volunteer to be involved in design discussions.

    .. code-block:: text

        Subject: [Design] Add feature A to module M

        Hello,

        I would like to add feature A to module M. This feature is motivated by
        X, Y, and Z. For example, this would enable:

          var result = M.A(args);

        See the issue here: https://github.com/chapel-lang/chapel/issues/<number>

        I need 2 developers to identify themselves as API reviewers for this design
        issue.

        Thanks,
        Contributor

.. _Leading a design discussion:

Leading a design discussion
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Many design choices require a consensus among community members.
It can be helpful to reach a consensus on a design decision quicker by making
an effort to lead that discussion.

Here are some ways to progress the discussion:

- Ask people involved what they need to know to make a decision
- Summarize the different solutions that were brought up in the discussion and
  list their pros and cons
- Compare with other languages, libraries, previous work in area

It is common for new design questions to emerge during design discussion
(or less commonly, in code review).
It can be helpful to spin off new issues for design questions that generate a
lot of discussion or design questions that are not completely on-topic.
This will help keep the discussion focused and the goals of the current design
issue clear.

.. _Development:

Development
-----------

.. _Get set up:

Get set up
~~~~~~~~~~

This should only need to happen once per developer.

Note: these are expected to evolve over time as the governance of Chapel is
migrated to an external/community body (the major elements are likely
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

#. Sign a Chapel `contributor license agreement`_ and mail it, with your GitHub
   ID.

* You do not need commit/push access to the main repo in order to
  contribute code.  See
  `Who has or needs commit access to the main repository?`_.

* Third-party code requires additional approvals, see the policy details on
  `Third-party code`_.

.. _Create new branch:

Create new branch
~~~~~~~~~~~~~~~~~

This should happen once for every new effort.

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

.. _Add new tests:

Add new tests
+++++++++++++

You will probably need to create new tests for your feature. See
`Creating a Simple Test`_ in `Test System`_ for more information on this
process.

Any addition/change to the Chapel test system should pass testing when that
test/directory is run with ``start_test`` (and performance tests should also
pass testing for ``start_test -performance``).

.. _Creating a Simple Test: https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/bestPractices/TestSystem.rst#creating-a-simple-test


.. _Contributing changes

Contributing changes
--------------------

.. _Push your work to your feature branch:

Push your work to your feature branch
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Push your changes to your feature branch on GitHub to enable others to see your
work (see `How to push`_ for command details).  Note that if you have already
created a pull request from a feature branch, pushing your work to that feature
branch will update the pull request.

.. _Ask for feedback on your branch early (optional):

Ask for feedback on your branch early (optional)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Not ready to merge your changes, but still want to see if your work is going in
the right direction?  Feel free to ask for early feedback!  Exposing the code is
generally done by:

- Pointing someone to your feature branch on Github, or
- Creating a `Work-in-progress pull request`_ for your feature branch.  See the
  `Submit pull request`_ section below for how to do this.

Discussion can take place in:

- the `Work-in-progress pull request`_
- a separate Github issue
- the chapel-developers_ mailing list
- a private communication
- some other strategy agreed upon by all involved parties


.. _Submit pull request:

Submit pull request
~~~~~~~~~~~~~~~~~~~

See `How to open a PR`_ for the sequence of steps necessary.

Contributors should be reasonably confident in the testing done on their code
before asking for a final review.  Should additional testing resources be
needed, you can request help from a member of the core Chapel team when creating
your pull request.

In working with your reviewers, you will no doubt change your pull request.
Just do your local development and then update your feature branch as in
`Push your work to your feature branch`_

Please follow the `Pull request guidance`_ and keep PRs reasonably sized.

.. _Find a reviewer:

Find a reviewer
~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~

* Iterate with the reviewer until you're both satisfied.  One should generally
  try to do whatever their reviewer asks.  Sometimes, a reviewer will ask for
  something really hard.  Try to make sure they understand the magnitude of the
  request, and try to discuss if it's really necessary to do before merging.  If
  you can't come to agreement, one of you should bring other developers
  (individually or via chapel-developers_) into the conversation to get a
  broader opinion.  One of the jobs of the reviewer is to serve as a proxy for
  other developers, or to bring those developers into the conversation if they
  feel unqualified to do so.

.. _Before merging:

Before merging
~~~~~~~~~~~~~~

Before the change can be merged, go through this checklist to ensure:

- all design changes have been discussed
- the PR has been reviewed
- the `contributor license agreement`_ (CLA) has been signed
- the `Git history is clear`_ of anything that should not be in the repo
- relevant configurations pass testing

If you did not have the resources to perform at least a full correctness test
run, this is the point at which a reviewer would do so for you.  Contributors
are expected to have verified any new tests work before asking a reviewer to do
this.

Details on how to run tests may be found at `Test System`_, and details on the
appropriate amount of testing before merging the final PR may be found at
`Testing your patch`_.

* Once the pull request is approved, it can be merged. This can be done by
  either the reviewer or developer (given sufficient permissions), as decided
  between the two of them.  See `How to merge a PR`_ for steps to perform this.

* If you are reviewing code from an external contributor without push
  privileges, go through the checklist once more before merging the change.

After the final version of the change has been agreed upon, the person making
the merge should follow the steps for `How to merge a PR`_.


.. _After merging:

After merging
~~~~~~~~~~~~~

After merging, a good contributor should watch automatic testing results for
failures that they may need to address:

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

.. _Other useful information:

Other useful information
------------------------

.. _Chapel git workflow:

Chapel git workflow
~~~~~~~~~~~~~~~~~~~~

The following section walks through some basics of git and GitHub that
are helpful in contributing to Chapel.

.. _initial git setup:

Initial Git Setup
+++++++++++++++++

Follow the GitHub directions to setup a new account.

https://help.github.com/articles/signing-up-for-a-new-github-account/

If you plan to use ssh to push/pull, setup SSH keys.

https://help.github.com/articles/connecting-to-github-with-ssh/



.. _Configure your local git:

Configure your local git
++++++++++++++++++++++++

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

    # (sort of) similar to: svn add path/to/file

Delete a file/dir and stage the change for commit:

.. code-block:: bash

    git rm [-r] path/to/dir/or/file

    # similar to: svn delete path/to/dir/or/file

Move a file/dir:

.. code-block:: bash

    git mv orig/path/a.txt new/path/to/b.txt

    # similar to: svn move orig/path/a.txt new/path/to/b.txt

Copy a file/dir and stage target for commit:

.. code-block:: bash

    cp <src> <target>
    git add <target>

    # similar to: svn copy <src> <target>

Get the status of files/dirs (staged and unstaged):

.. code-block:: bash

    git status

    # similar to: svn status

Get the diff of unstaged changes:

.. code-block:: bash

    git diff

    # similar to: svn diff

Get the diff of staged changes (those that were staged with ``git add``):

.. code-block:: bash

    git diff --cached

Backing out unstaged changes:

.. code-block:: bash

    git checkout path/to/file/a.txt

    # similar to: svn revert path/to/file/a.txt

Committing staged changes:

.. code-block:: bash

    git commit [-m <message>]

    # similar to: svn commit [-m <message>]

There are two different strategies available to bringing changes from Chapel
master into your development branch:

Rebase (replay your changes on top of the master branch):

.. code-block:: bash

    git fetch upstream
    git rebase upstream/master

    # If branch has already been pushed, you will need to force push to update:
    git push -f -u origin <branch_name>


If there are conflicts, you will be asked to resolve them. Once the affected
files have been fixed, stage them with ``git add``, and then call ``git
rebase --continue`` to finish the rebase process.

If there are other development branches working off of your development branch
(something not common in Chapel development), then you should not use this
strategy. Instead, merge the Chapel master branch into your branch as shown
below, which does not require a force push to rewrite git history.


Merge (merge master into your branch):

.. code-block:: bash

    git fetch upstream
    git merge upstream/master

    # or:
    git pull upstream master

    # with feature branch checked out:
    git merge [--no-ff] upstream/master

If there are conflicts, you will be asked to resolve them. Once the affected
files have been fixed, stage them with ``git add``, and then call ``git
commit`` to finish the merge process.

If you want to understand the changes that occurred upstream, see
`Read commit messages for changes from the main Chapel project`_ below.

Using ``git merge upstream/master`` or ``git pull upstream master`` is not
recommended when working in development branches that have no other references
to them (which is typical in Chapel development), because
it pollutes the git history with non-meaningful merge commits. These show up in
the git history as:

.. code-block:: bash

    Merge branch 'master' of github.com:chapel-lang/chapel into dev-branch


.. _How to modify git history:

How to modify git history
+++++++++++++++++++++++++

The following commands **may cause problems** if the changes they overwrite
have been pulled by other repositories.

Fixing a commit message:

.. code-block:: bash

    git commit --amend

Un-do the last commit (leaving changed files in your working directory):

.. code-block:: bash

    git reset --soft HEAD~1

Reapplying changes from the current branch onto an updated version of master:

.. code-block:: bash

    git rebase master

Reapplying changes from the current branch onto an updated version of
upstream/master, without updating your local master (note: you will need to
perform a pull next time you checkout your local master):

.. code-block:: bash

    git fetch upstream
    git rebase upstream/master

Pushing such changes to your repository (again, **this may cause problems** if
other repositories have pulled the changes -- however this is uncommon in the
Chapel development workflow):

.. code-block:: bash

    git push -f origin <branch_name>

.. _Read commit messages for changes from the main Chapel project:

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

Note that ``-f`` is necessary if you've modified changes on your feature branch
(see `How to modify git history`_).

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

  You will have to have signed a `contributor license agreement`_ (CLA).

  Your pull request will be available at a URL like:

  ``https://github.com/chapel-lang/chapel/pull/<number>``

  and you can discuss the patch with your reviewers there.

.. _contributor license agreement: https://github.com/chapel-lang/chapel/tree/master/doc/rst/developer/contributorAgreements/

.. _How to merge a PR:

How to merge a PR:
++++++++++++++++++

If you have commit privileges (see
`Who has or needs commit access to the main repository?`_), navigate to the
pull request:

go to

https://github.com/chapel-lang/chapel/pulls

or

``https://github.com/chapel-lang/chapel/pull/<number>``

and click the friendly green button "Merge pull request" (it is possible to
merge the pull request from the command line also and the pull request page has
details). When you click "Merge pull request", you will need to enter a commit
message. See `Final merge message`_ for a reminder on what that commit message
should entail (generally, this will closely resemble the PR message).

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
latest work stored on github. See `Development commands`_ for how to update a
feature branch with new changes from the main Chapel project)

.. code-block:: bash

    git pull

    # or:
    git fetch origin
    git merge origin/master

    # similar to: svn update

Update to specific revision number:

.. code-block:: bash

    git checkout <commit sha1>

    # similar to: svn update -r<revision number>

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

.. _Who has or needs commit access to the main repository?:

Who has or needs commit access to the main repository?
++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
packages must be approved by our lawyers before being committed.

Here are some guiding questions to determine whether a third-party package you
rely on should be committed to the chapel repository:

- How large is the third-party code you wish to include?

  - If the code is very large, perhaps it would be better to add directions on
    how to install this dependency.

- Under what license does this code operate?

  - We try not to add dependencies on code that is under GPL or LGPL, as those
    licenses have copyleft properties and force derivative works to be
    distributed under the same license.

    - Is there an alternate package with a more permissive license that can
      accomplish the same purpose?

      - If so, we recommend relying on that package instead.

      - If not, it would be better to add directions on how to install this
        dependency.

- How easy is this code to obtain?

  - Will it be installed by default on an ordinary machine?

    - If so, we do not need to redistribute it ourselves.

- How much of the Chapel implementation will rely on this code?

  - The compiler for ordinary Chapel?  A commonly used runtime configuration?

    - In these cases, we will probably want to include the code in our
      distribution.

  - A standard or package module that is not included by default?

    - Depending on the circumstances, it might be better to just include
      directions on how to install this code.

- Do we require Chapel-specific modifications to the code in order to use it?

  - If so, we will probably want to distribute this package, or at least include
    the modifications and an easy way to install them.

Please include the answers to these questions when you contact the
chapel-developers_ mailing list, if you believe the code should be included or
you remain uncertain.

.. _Testing your patch:

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

.. _Work-in-progress pull request:

Work-in-progress pull request
+++++++++++++++++++++++++++++

A work-in-progress (WIP) PR is a special kind of pull request that is not yet
intended to be merged.  Such PRs are created to take advantage of what the
Github PR interface provides, such as public comment history and quick links
between the WIP PR and other related issues and pull requests.  They allow the
developer to get early feedback on a change.

The status of a WIP PR can be stated by using the GitHub "draft PR" feature.
The PR description should include what steps need to be taken before the PR is
ready for final review. If a WIP PR was not opened as draft PR, it can be
marked with the ``stat: work in progress`` label by a core contributor.

It is perfectly acceptable to abandon such PRs (especially in favor of a cleaned
up version of the code) when the git history becomes too large, so long as a
link to the original PR is provided when the change is eventually merged, to
preserve the discussion.

.. _Pull request guidance:


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
    the developer and user community.

.. _Pull request description:

Pull request description
++++++++++++++++++++++++

* Generally, try to come up with a single-line headline of 75 characters at
  most to describe the change.  Think of this as a statement of what the PR
  accomplishes, ideally with a user perspective (as opposed to the "what I did
  to the code" perspective).

* Depending on the magnitude of the change, write a short paragraph to longer
  set of paragraphs describing the change (again, focusing on the user
  experience, at least in the initial paragraphs... why would a Chapel user
  care that you merged this?  Or if it's not user-facing, why would a developer
  care).

* If it's truly complex such that you think the code changes themselves warrant
  describing in the PR (because they're complex or clever, say), additional
  paragraphs talking about the code changes / approach taken can be nice (but
  aren't strictly required).

* Depending on the complexity of the PR, it can be reassuring to reviewers to
  denote how you have tested the PR either in the PR description or an initial
  comment.

It's not uncommon for the contents of the PR description to evolve over the
lifetime of the PR and its review.  Initially it should help the person doing
the code review understand what's being done (and potentially why).  Then, when
the code review is done, make sure the text is still accurate.

.. _Final merge message:

Final merge message
+++++++++++++++++++

When merging, copy and paste the PR description into the GitHub merge dialog
box. Ensure that the title of the PR is at the top.

It is good practice to @-mention the reviewer(s) at the end of the PR.
This is typically formatted in brackets:

.. code-block:: bash

    [Reviewed by @reviewer1 and @reviewer2]

.. _Git history is clear:

Git history is clear
++++++++++++++++++++

In general, having logical commits with meaningful commit messages is helpful
for maintaining a clean git history. This is particularly important for
critical or complex code changes. Depending on how critical or complex your
changes are, it may be a good idea to do an interactive rebase to squash any
non-meaningful commits:

.. code-block:: bash

    git fetch upstream
    git rebase -i upstream/master

Note that this can be particularly cumbersome when there has been significant
conflicting changes made on upstream master, so is not a hard requirement.

An alternative approach is to use the "squash and merge" option on the github
merge button which will reduce all commits to a single commit.

It's not generally possible to completely remove a commit from git by the time
it makes it in to the master branch. So be very careful not to commit anything
that you might regret later (e.g., sensitive code, code owned by anyone other
than yourself). Ideally, the review will catch such issues, but the ultimate
responsibility is on the developer.


.. _Reviewer responsibilities:

Reviewer responsibilities
+++++++++++++++++++++++++

* If you're reviewing a commit from a developer outside the Chapel core
  team, be sure they have signed the `contributor license agreement`_ (see the
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

What Copyright Should I Use?
++++++++++++++++++++++++++++

By signing a Contributor Agreement, you have agreed that code you contribute
will be governed by the license and copyright of the project as a whole.  A
standard block of license text is required at the top of every compiler,
runtime, and module code file.  Browse other files of the same type to see the
required license block.

Additional copyrights may also be applied, as appropriate.

.. _Chapel release process:

Chapel release process
~~~~~~~~~~~~~~~~~~~~~~

When a Chapel release nears, merging of pull requests is temporarily frozen for
a few weeks to ensure stability of the release. These freezes come in two
phases: feature freeze and code/doc freeze.

After feature freeze, only bug fixes and documentation updates will be accepted
for the upcoming release. After code/doc freeze, no contributions will be
accepted for the upcoming release, barring critical bug fixes. At some point
between code/doc freeze and the release, a release branch will be "cut", and
the master branch will begin accepting all contributions again.

The dates for these freezes will be posted on the `Chapel events page`_
sometime close to each release.

.. _Chapel events page: https://chapel-lang.org/events.html
