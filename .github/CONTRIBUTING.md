# How to Contribute

As an open-source project, Chapel welcomes source contributions from 
the community.  Chapel enforces the Developer Certificate of Origin (DCO) 
on Pull Requests. It requires all commit messages to contain the 
Signed-off-by line with an email address that matches the commit 
author and the name on your GitHub account.

The Developer Certificate of Origin (DCO) is a lightweight way for 
contributors to certify that they wrote or otherwise have the right 
to submit the code they are contributing to the project. Here is the 
full [text of the DCO][0], reformatted for readability:

    By making a contribution to this project, I certify that:

      (a) The contribution was created in whole or in part by me and I have the right to submit it under the open source 
          license indicated in the file; or

      (b) The contribution is based upon previous work that, to the best of my knowledge, is covered under an > 
          appropriate open source license and I have the right under that license to submit that work with modifications, 
          whether created in whole or in part by me, under the same open source license (unless I am permitted to submit 
          under a different license), as indicated in the file; or

      (c) The contribution was provided directly to me by some other person who certified (a), (b) or (c) and I have not 
          modified it.

      (d) I understand and agree that this project and the contribution are public and that a record of the contribution 
          (including all personal information I submit with it, including my sign-off) is maintained indefinitely and may 
          be redistributed consistent with this project or the open source license(s) involved.

Contributors sign-off that they adhere to these requirements by adding a `Signed-off-by` line to commit messages.

    This is my commit message

    Signed-off-by: Random J Developer <random@developer.example.org>

Git has a `-s | --signoff` command line option to append this automatically to your commit message:

    git commit -s -m "This is my commit message"

If you have authored a commit that is missing the signed-off-by line, you can amend your commits and push them to GitHub.

    git commit --amend --signoff

If you've pushed your changes to GitHub already you'll need to force push your branch after this with `git push -f`.

## Fixing DCO

If your Pull Request fails the DCO check, it's necessary to fix the entire commit history in the PR. Best practice is to 
squash the commit history to a single commit, append the DCO sign-off as described above, and force push. For example, 
if you have 2 commits in your history (Note the ~2):

    git rebase -i HEAD~2
    (interactive squash + DCO append)
    git push origin -f

Note, that in general rewriting history in this way may introduce issues to the review process and this should only be done to 
correct a DCO mistake.


## Resources for contributing to Chapel

[Contributing Page][2] : A task-oriented guide on how to get started contributing to Chapel.

[ContributorInfo.rst][1] : A description of the mechanisms and policies for contributing to Chapel

---

[0]: https://developercertificate.org/
[1]: https://github.com/chapel-lang/chapel/tree/master/doc/rst/developer/bestPractices/ContributorInfo.rst
[2]: https://chapel-lang.org/contributing.html

