Getting started with Chapel and the Developer Certificate of Origin (DCO)
=========================================================================

In order to merge a pull request (PR) to the Chapel repository, all
git commits must be signed by their developers to certify that they
wrote, or otherwise have the right to submit, the changes to the
project.  The full text of the DCO is available in Chapel's
`CONTRIBUTING.md`_ file.

This means that the message for each commit you submit must contain a
line that reads:

.. code-block:: bash

     Signed-off-by: Random J Developer <random@developer.example.org>

where the name and email address must match those of your GitHub
account as indicated by ``git config user.name`` and ``git config
user.email``.  The presence of this line signifies that you're making
the commit in accordance with the DCO.

The rest of this document describes some tips and tricks for signing
commits proactively or retroactively.

.. _CONTRIBUTING.md: https://github.com/chapel-lang/chapel/blob/master/.github/CONTRIBUTING.md


Ways to Sign Commits
--------------------

#. Signing each commit manually

   Of course, when you are making a commit and editing the commit
   message, you can sign it manually, by typing the line above but
   with your GitHub account's name and email address.  However, this
   has the obvious downsides of being tedious and error-prone.


#. Using ``git -s``

   Git has a ``-s | --signoff`` command-line flag that will
   automatically add your 'Signed-off-by' line to your commit message.
   For an interactive commit like the following:

   .. code-block:: bash
        git commit -s

   you should see the 'Signed-off-by' line in your editor when it
   brings up the buffer representing the commit message.  If you
   supply the commit message on the command-line, the 'Signed-off-by'
   line will be automatically added for you.

   .. code-block:: bash
        git commit -s -m "This is my commit message"

   This approach has the advantage of being fairly straightforward and
   requiring no configuration, but the downside that you need to
   remember to use the flag with each commit.


#. Using a git commit template

   This approach uses a template file that contains your signature for
   interactive commit messages.  It has the advantage of being fairly
   straightforward and making your signature visible to you when you
   commit.  However, a downside is that it doesn't apply your
   signature to git commits that skip the interactive editing of
   commit messages, like ``git revert`` or ``git commit -m ...``.

   To take this approach, create a file (say, ``~/.gitmessage``) that
   will form the basis for any interactive commit messages, containing
   your signature line:

   .. code-block:: bash

        ---
        Signed-off-by: Random J Developer <random@developer.example.org>

   Then, tell your git configuration to open this file for any new
   interactive commit messages using one of the following forms.  To
   set this up for the current repository only:

   .. code-block:: bash

        git config commit.template ~/.gitmessage

   To set it up across repositories via your ``~/.gitconfig`` file:
     
   .. code-block:: bash

        git config --global commit.template ~/.gitmessage
     

#. Using a git commit hook

   This approach uses a script to automatically add your signature
   line to new commits within a given repository if it isn't found in
   the commit message.  Relative to the previous approach, it has the
   advantage of being used more consistently across different styles
   of committing, but the slight downside of making the fact that
   you're signing your commits less visible to you / easier to forget
   about.

   To take this approach, copy the script from
   ``$CHPL_HOME/util/misc/pre-commit-msg-hook`` to
   ``$CHPL_HOME/.git/hooks/commit-msg``:

   .. code-block:: bash

        cp $CHPL_HOME/util/misc/pre-commit-msg-hook $CHPL_HOME/.git/hooks/commit-msg

   You can also use variations on this script to exit with an error if
   the signature line is missing rather than adding it automatically.
   

Troubleshooting DCOs
--------------------
    
If you have authored a commit that is missing its 'Signed-off-by'
line, you can amend your commits and push them to GitHub.

.. code-block:: bash

     git commit --amend --signoff

If you've pushed your changes to GitHub already you'll need to force
push your branch after this with ``git push -f``.

If your Pull Request fails the DCO check, it will be necessary to fix
the entire commit history for the PR. Best practice is to squash the
commit history to a single commit, append the DCO sign-off as
described above, and force push. For example, if you have 2 commits in
your history (Note the ~2):

.. code-block:: bash

     git rebase -i HEAD~2
     (interactive squash + DCO append)
     git push origin -f

Note that, in general, rewriting history in this way may introduce
issues to the review process and this should only be done to correct a
DCO mistake.
