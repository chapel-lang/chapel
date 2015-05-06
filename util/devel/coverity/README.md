Coverity Scan for Chapel
========================

The Chapel Coverity Scan project is here:

https://scan.coverity.com/projects/1222

Model File
----------

The model file used by the project is in this directory,
[`coverity_model.cpp`](coverity_model.cpp). For more information on modeling,
refer to the Coverity documentation:

https://scan.coverity.com/tune

Remember that changes to the model file must be committed to the repository
(for auditing purposes) **and** uploaded to the project page.

As of May 2015, we prefer to add to the model file instead of
annotating the source code, when we want to convey something
to Coverity and both options are available. E.g. to indicate
that a function never returns, such a function can be placed
in the model file with the body of __coverity_panic__();
or it can be preceeded with // coverity[+kill] in the source
code. The policy is to do the former, on the grounds that
this is useful only for Coverity.

Running the Scan
----------------

The Chapel team has configured a job to automatically run the Coverity Scan
periodically. You should not need to run this manually.

The [`submit_build.bash`](submit_build.bash) script in this directory is used
to run the scan. Once everything is configured (see below), clone a clean copy
of the repo and run the script.

Scan Configuration
-------------------

* Download and unpack the analysis tools (currently Chapel team is using
  linux64 to run the scan):https://scan.coverity.com/download?tab=cxx
* Create a `~/.coverity` file with the following:

```
#!/usr/bin/env bash

COV_SCAN_EMAIL=<coverity user email>
COV_SCAN_TOKEN=<coverity scan token>
COV_BUILD_PREFIX=<coverity analysis tools location>
```

For example:

```
#!/usr/bin/env bash

COV_SCAN_EMAIL=thomas@example.com
COV_SCAN_TOKEN=my-coverity-token
COV_BUILD_PREFIX=/opt/cov-analysis/cov-analysis-linux64-7.5.0/bin
```

The token can be found on
[New Build page](https://scan.coverity.com/projects/1222/builds/new).
