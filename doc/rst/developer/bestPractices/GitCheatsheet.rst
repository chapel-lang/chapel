Git "cheat sheet" (with comparisons to svn)
===========================================

See ContributorInfo.rst for more information about the process of contributing
code to Chapel. The basic workflow is:

E) `Update your feature branch with changes from the main Chapel project`_
F) `Read commit messages for changes from the main Chapel project`_
G) `Test your feature`_
H) `Push your work to your feature branch`_
I) `Create a pull request and work with your reviewers`_
J) `Once your reviewers are satisfied, merge in your pull request`_


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
ContributorInfo.rst and TestSystem.rst for more information on this process.
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





More Information on Using Git
=============================


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

