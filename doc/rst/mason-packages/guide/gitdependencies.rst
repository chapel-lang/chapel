:title: Mason Guide: *Git Dependencies*

.. _mason-git-dependencies:

Specifying Dependencies from ``git`` Repositories
=================================================
To depend on a library located in a git repository, a git key pointing to the repository URL is all that is required:

.. code-block:: text


    [dependencies]
    HelloWorld = { git = "https://github.com/bmcdonald3/HelloWorld" }


The repository that is specified must be a mason package itself, but it does not need to be contained in a mason registry.

Without specifying the branch to be used, mason will default to a usage of the project ``HEAD``. After the first run of ``mason build``, the revision will be locked in and continuously reused until a ``mason update`` command is run. Then, the latest changes will be fetched and the revision will be updated in the ``Mason.lock`` file and used until the next run of ``mason update``. Executing the ``mason build`` command will not update the revision in ``Mason.lock``.

To specify a different branch, the ``branch`` key can be used:

.. code-block:: text


    [dependencies]
    HelloWorld = { git = "https://github.com/bmcdonald3/HelloWorld", branch = "test-branch" }

When a ``branch`` tag is specified, your first run of ``mason build`` will lock in the revision in the ``Mason.lock`` file and will continue to use that revision until the ``mason update`` command is executed. The ``mason update`` command will fetch the latest changes and then update the revision used to the current tip of the specified ``branch``.

If you would like to lock in a specific revision that should not be updated, a specific revision can be specified for your git dependency by explicitly specifying a ``rev`` tag, and this revision will never be modified by any mason command:

.. code-block:: text


    [dependencies]
    HelloWorld = { git = "https://github.com/bmcdonald3/HelloWorld", rev = "43d462682851dd2fed6edf123e8fb699db124183" }
                
Now, when running ``mason build`` or ``mason update``, the latest changes will be fetched from the repository, but the commit specified by ``rev`` will always be checked out and never changed by mason.

The examples above have used the inline subtable syntax to format the git dependency. Git dependencies can also be specified with subtable syntax and mason commands that modify the ``Mason.toml`` file may reformat dependencies to be of this format:

.. code-block:: text


    [dependencies.HelloWorld]
    git = "https://github.com/bmcdonald3/HelloWorld"


