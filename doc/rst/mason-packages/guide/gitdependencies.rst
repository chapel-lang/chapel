Specifying Dependencies from `git` Repositories
===============================================
To depend on a library located in a git repository, a git key pointing to the repository URL is all that is required:

.. code-block:: text


    [dependencies]
    HelloWorld = { git = "https://github.com/bmcdonald3/HelloWorld" }


The repository that is specified must be a mason package itself, but it does not need to be contained in a mason registry.

Without specifying the branch to be used, a default branch of `master` will be assumed and if there is no master branch, the dependency will not work. To specify a different branch, the `branch` key can be used:

.. code-block:: text


    [dependencies]
    HelloWorld = { git = "https://github.com/bmcdonald3/HelloWorld", branch = "test-branch" }


Now, when running `mason build` or `mason update`, the latest changes from the repository will be pulled.

.. note::
   
   Currently, the manifest file from git repository dependencies are not checked, so any essential information there will need to be manually copied into the `Mason.toml` from the repository using the dependency. This is currently a work in progress and will hopefully be fixed in the near future. Additionally, locking in the revision at the time of specification and allowing specific revisions to be specified are also works in progress.
