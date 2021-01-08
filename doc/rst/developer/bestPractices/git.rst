Git tips for Chapel developers
==============================

This document walks through some basics of using git and GitHub that
may be helpful in contributing to Chapel.  If you're already
comfortable with ``git`` and GitHub, this document is unlikely to be
of much use to you.

.. _Chapel git workflow:

Chapel git workflow
~~~~~~~~~~~~~~~~~~~~

The following section walks through some basics of git and GitHub that
are helpful in contributing to Chapel.

.. _initial git setup:

Initial Git Setup
+++++++++++++++++

Follow the GitHub directions to set up a new account.

https://help.github.com/articles/signing-up-for-a-new-github-account/

If you plan to use ssh to push/pull, set up SSH keys.

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

    # If using ssh keys, verify access to GitHub. It should respond with a
    # message including your GitHub username.
    ssh git@github.com

    # Clone your fork of the chapel repo!
    git clone git@github.com:<github_username>/chapel.git

    # Or, if using HTTPS instead of SSH.
    git clone https://github.com/<github_username>/chapel.git

    # Set up remotes for GitHub
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

    # Accept the Developer Certificate of Origin by adding a Signed-off-by line to commit messages:

    git commit -s [-m <message>]

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

  You will have to add "signed-off-by" in your commits to accept `Developer Certificate of Origin`_ (DCO)

  Your pull request will be available at a URL like:

  ``https://github.com/chapel-lang/chapel/pull/<number>``

  and you can discuss the patch with your reviewers there.

.. _Submit a pull request: https://help.github.com/articles/using-pull-requests
.. _synced with the main repo: https://help.github.com/articles/syncing-a-fork
.. _Developer Certificate of Origin: https://github.com/chapel-lang/chapel/blob/master/.github/CONTRIBUTING.md

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

.. _Who has or needs commit access to the main repository?: ContributorInfo.rst#who-has-or-needs-commit-access-to-the-main-repository
.. _Final merge message: ContributorInfo.rst#final-merge-message

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
latest work stored on GitHub. See `Development commands`_ for how to update a
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



