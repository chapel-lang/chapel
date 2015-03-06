Git "cheat sheet" (with comparisons to svn)
===========================================

See ContributorInfo.rst for more information about the process of contributing
code to Chapel. The basic workflow is:

A) `Let the Chapel community know about any long-term or big efforts`_
B) `Use GitHub to fork the Chapel project`_
C) `Create and switch to a feature branch`_
D) `Develop your contribution locally`_
E) `Update your feature branch with changes from the main Chapel project`_
F) `Read commit messages for changes from the main Chapel project`_
G) `Test your feature`_
H) `Push your work to your feature branch`_
I) `Create a pull request and work with your reviewers`_
J) `Once your reviewers are satisfied, merge in your pull request`_


Let the Chapel community know about any long-term or big efforts
----------------------------------------------------------------

Email chapel-developers@lists.sourceforge.net -- see ContributorInfo.rst

Use GitHub to fork the Chapel project
-------------------------------------

See Initial Git Setup below for information on getting a GitHub account and
setting up Git.

Use the GitHub web interface to create a fork of the Chapel repo by visiting
 https://github.com/chapel-lang/chapel
and clicking the 'Fork' button. Then configure your local git and check out
your fork:

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

Create and switch to a feature branch
-------------------------------------

.. code-block:: bash

    git checkout -b <branch_name>

Develop your contribution locally
---------------------------------

Your contribution will take the form of a series of commits.  While including
sensible commit messages is a good idea, it is more important to have a good
merge message once the pull request is going in. Likewise, it is OK to have
many small commits that reflect the history of development rather than commits
for the feature.

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

    This should only every be done to commits that **have not been pushed** to
    a remote repository.

Update your feature branch with changes from the main Chapel project
--------------------------------------------------------------------
Bring in changes from the main Chapel project to your feature branch:

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


Read commit messages for changes from the main Chapel project
-------------------------------------------------------------

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

More logging commands are described in "Other Logging Commands" below.

Test your feature
-----------------

You will probably need to create new tests for your feature. See
ContributorInfo.rst and TestSystem.txt for more information on this process.
To do the most basic testing, you'd do:

.. code-block:: bash

    # run Hello tests
    make check

    # run tests that end up in $CHPL_HOME/examples
    start_test test/release/examples

    # run all tests
    start_test test/


Push your work to your feature branch
-------------------------------------

Push your changes to your feature branch on GitHub

.. code-block:: bash

    git push origin <branch_name>

    # or if you don't like typing your complicated branch name,
    # you can use this command to push the current branch:
    git push origin HEAD

    # if you forgot your branch name, you can get it by running
    git branch

    # it is the starred one...

Note that if you have already created a pull request from a
feature branch, pushing your work to that feature branch will
update the pull request.

Create a pull request and work with your reviewers
--------------------------------------------------

After pushing your changes to your feature branch on GitHub, use the GitHub web
interface to create a pull request.  Visit

  https://github.com/<username>/chapel

and look for a "Compare & pull request" button for your feature branch.
Alternatively, navigate to your feature branch, and click the green icon next
to the branch dropdown to "Compare, review, create a pull request".

Next, put in a message to your reviewer about the purpose of your pull request
and give the pull request a useful title. It's a good time to draft the commit
message that you will need when merging the pull request in step (J).

Your contribution will need to be tested and reviewed, and you will have to
have signed a contributors agreement. See ContributorInfo.rst for more
information.

Your pull request will be available at a URL like:
  https://github.com/chapel-lang/chapel/pull/<number>

and you can discuss the patch with your reviewers there.

In working with your reviewers, you will no doubt change your pull request.
Just do your local development and then update your feature branch as in (H)
and the pull request will change.

Once your reviewers are satisfied, merge in your pull request
-------------------------------------------------------------

After you and your reviewers agree upon the final version of your change,
navigate to the pull request you created:

go to 
  https://github.com/chapel-lang/chapel/pulls
or
  https://github.com/chapel-lang/chapel/pull/<number>

and click the friendly green button "Merge pull request" (it is possible to
merge the pull request from the command line also and the pull request page has
details). When you click "Merge pull request", you will need to enter a commit
message.

This commit message should:
 - start with a single topic line with at most 75 characters
 - then have a blank line
 - then have a more detailed explanation including motivation for the
   change and how it changes the previous behavior
 - use present tense (e.g. "Fix file iterator bug")
 - manually wrap long lines in the explanation to 75 or 80 characters





.. _initial_git_setup:

Initial Git Setup
=================

Follow the GitHub directions to setup a new account.

  https://help.github.com/categories/53/articles

If you plan to use ssh to push/pull, setup SSH keys.

  https://help.github.com/categories/56/articles

Use the GitHub web interface to create a fork of the Chapel repo by visiting
https://github.com/chapel-lang/chapel and clicking the 'Fork' button. Then
configure your local git and check out your fork:

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


More Information on Using Git
=============================

Additional docs available online at: http://git-scm.com/docs/

Git help pages can be viewed with:

.. code-block:: bash

    git help <command>


Other Git Commands
------------------

Update to HEAD:

(If you use this command on a feature branch, you'll just be updating to the
latest work stored on github. See (E) for how to update a feature branch with
new changes from the main Chapel project)

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


