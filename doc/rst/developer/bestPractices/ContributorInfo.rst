Contributor Info
================

The git repository for the project is hosted on GitHub at
`chapel-lang/chapel`_. Anyone can read the repository. It is open source!

This document contains a mixture of tips for git beginners and specific
Chapel workflow recommendations.

Below are instructions for setting up a GitHub account, developing a
feature, and submitting pull requests.

.. note:: All commits must be signed according to the DCO (see below) in order to be merged.

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
- To find reviewers, post an inquiry to the `Chapel Developers`_
  Discourse forum.

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
- Compare with other languages, libraries, previous work in the area

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

#. `Set up a GitHub account`_. The "Free" plan is sufficient for contributing to
   Chapel.

#. Use the GitHub web interface to create a fork of the Chapel repo by visiting
   https://github.com/chapel-lang/chapel and clicking the 'Fork' button (see
   also `Fork the repo`_).

#. Make sure you have configured your environment to work with
   git. See `Git tips for Chapel developers`_ for details,
   particularly if you are new to git.

#. Make sure you understand how to sign your commits with respect to the DCO.

   Chapel enforces the Developer Certificate of Origin (DCO) on all
   pull requests.  This requires all commits you make to be signed to
   indicate that they adhere to the DCO policy.  If you're not already
   familiar with DCOs, read `Getting started with Chapel and the
   Developer Certificate of Origin`_ to learn more about them and how
   to sign your commits.

#. If you're working on a long-term effort, announce it in the
   `Chapel Developers`_ Discourse category to make sure toes are not being stepped on,
   work is not being pursued redundantly, etc.  Similarly, fundamental changes
   to the language or architecture should be circulated in the
   `Chapel Developers`_ and/or `Chapel Users`_ categories to make sure effort is not
   wasted pursuing an unpopular idea.

* You do not need commit/push access to the main repo in order to
  contribute code.  See
  `Who has or needs commit access to the main repository?`_.

* Third-party code requires additional approvals, see the policy details on
  `Third-party code`_.

.. _Git tips for Chapel developers: git.rst
.. _Getting started with Chapel and the Developer Certificate of Origin: DCO.rst

   
.. _Create new branch:

Create new branch
~~~~~~~~~~~~~~~~~

This should happen once for every new effort.

Develop your feature, bug fix, etc. on your fork.  To create a new
branch, use ``git checkout -b <branch_name>``.  Using a concisely
named branch is encouraged.



.. _Develop and test contributions locally:

Develop and test contributions locally
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Your contribution will take the form of a series of commits.  While including
sensible commit messages is a good idea, it is more important to have a good
merge message once the pull request is going in. Likewise, it is OK to have many
small commits that reflect the history of development rather than commits for
the feature.  Review git `Development commands`_ for how to perform some common
operations during development.

As you work, you will want to periodically bring in changes from the main Chapel
project to your feature branch (described in `Development commands`_), to avoid
code drift.

.. _Development commands: git.rst#development-commands

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

.. _How to push: git.rst#how-to-push

.. _Ask for feedback on your branch early (optional):

Ask for feedback on your branch early (optional)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Not ready to merge your changes, but still want to see if your work is going in
the right direction?  Feel free to ask for early feedback!  Exposing the code is
generally done by:

- Pointing someone to your feature branch on GitHub, or
- Creating a `Work-in-progress pull request`_ for your feature branch.  See the
  `Submit pull request`_ section below for how to do this.

Discussion can take place in:

- the `Work-in-progress pull request`_
- a separate GitHub issue
- the `Chapel Developers`_ Discourse forum
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

.. _How to open a PR: git.rst#how-to-open-a-pr

.. _Find a reviewer:

Find a reviewer
~~~~~~~~~~~~~~~

* Once your PR is ready, you'll need to request a review.  If you know who you'd
  like to review it, @ mention them in a comment on the PR and ask them to have
  a look.  If you don't know their GitHub id, you can find them in the chat room
  or send them an email.  If you don't know who should review the change, post
  to the `Chapel Developers`_ Discourse topic requesting a review and linking to the
  PR.  By convention, such topics often use a `[PR]` prefix in their titles.

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
  you can't come to an agreement, one of you should bring other developers
  (individually or via the `Chapel Developers`_ forum) into the conversation to get a
  broader opinion.  One of the jobs of the reviewer is to serve as a proxy for
  other developers, or to bring those developers into the conversation if they
  feel unqualified to do so.

.. _Before merging:

Before merging
~~~~~~~~~~~~~~

Before the change can be merged, go through this checklist to ensure:

- all design changes have been discussed
- all commits contain the required "Signed-off-by:" line to indicate
  compliance with the `Developer Certificate of Origin`_ (DCO)
- the PR has been reviewed
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

.. _Developer Certificate of Origin: DCO.rst
.. _How to merge a PR: git.rst#how-to-merge-a-pr


.. _After merging:

After merging
~~~~~~~~~~~~~

After merging, a good contributor should watch automatic testing results for
failures that they may need to address:

* In short order, a smoke-test will be run against the commit to make sure that
  nothing basic has been broken by it.  Your reviewer should alert you to any
  problems.

* After the commit has gone in, check with your reviewer to ensure that
  there are no new failures caused by your commit in our nightly testing.

.. _Other useful information:

Other useful information
------------------------


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
`chapel-lang/chapel`_, send a request including your GitHub username to
chapel_admin _at_ cray.com.

.. _Third-party code:

Third-party code
++++++++++++++++

If your work will require committing any third-party code that you are not
developing yourself (or code that you've developed as a standalone package),
alert the `Chapel Developers`_ Discourse forum of this as, presently, such code
packages must be approved before being committed.

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

Please include the answers to these questions when you post to the
`Chapel Developers`_ forum, if you believe the code should be included or
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
GitHub PR interface provides, such as public comment history and quick links
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
conflicting changes made on upstream master, so it is not a hard requirement.

An alternative approach is to use the "squash and merge" option on the GitHub
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
  team, be sure their commits are signed via the DCO bot (one of several
  github action checks that will run on each PR).  If they're not, help
  the developer understand the requirement.

  Note that using GitHub's "squash and merge" feature will effectively
  drop all DCO signature lines from the pull request, and so should
  not be used on external commits (or potentially even for commits
  from the core team?) in order to preserve the signed nature of the
  commits.

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

.. _Chapel Developers: https://chapel.discourse.group/c/developers
.. _Chapel Users: https://chapel.discourse.group/c/users
.. _chapel-lang/chapel: https://github.com/chapel-lang/chapel
.. _Set up a GitHub account: https://help.github.com/articles/signing-up-for-a-new-github-account
.. _Fork the repo: https://guides.github.com/activities/forking/

What Copyright Should I Use?
++++++++++++++++++++++++++++

By opening a PR with signed commits, you are agreeing that code you contribute
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
