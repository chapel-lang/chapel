Git "cheat sheet" (with comparisons to svn)
===========================================

See ContributorInfo.rst for more information about the process of contributing
code to Chapel. The basic workflow is:

G) `Test your feature`_
H) `Push your work to your feature branch`_
I) `Create a pull request and work with your reviewers`_
J) `Once your reviewers are satisfied, merge in your pull request`_


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

