Chapel Contributor Agreements
=============================

Chapel enforces the Developer Certificate of Origin (DCO) 
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


[0]: https://developercertificate.org/
