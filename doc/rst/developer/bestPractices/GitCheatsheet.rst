Git "cheat sheet" (with comparisons to svn)
===========================================

See ContributorInfo.rst for more information about the process of contributing
code to Chapel. The basic workflow is:

J) `Once your reviewers are satisfied, merge in your pull request`_


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

