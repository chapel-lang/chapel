:title: Mason Guide: *Submitting*

.. _submit-a-package:

Submit a Package
================

The mason registry will hold the manifest files for packages submitted by developers.
To contribute a package to the mason-registry a chapel developer will need to host their
package and submit a pull request to the mason-registry with the toml file pointing
to their package. For a more detailed description follow the steps below. Publishing
can be done with ``mason publish`` or manually.

``mason publish`` Steps:
      1) Write a library or binary package in chapel using mason
      2) Host the package in a git repository. (e.g. GitHub)
      3) Fork the mason-registry on GitHub
      4) Ensure your package has a remote origin.
      5) Run ``mason publish`` in your package
      6) Go to the link provided to open a pull request to the mason registry.
      7) Wait for mason-registry maintainers to approve PR.

Manual Steps:
      1) Write a library or binary package in chapel using mason
      2) Host that package in a git repository. (e.g. GitHub)
      3) Create a tag of your package that corresponds to the version number prefixed with a 'v'. (e.g. v0.1.0)
      4) Fork the mason-registry on GitHub
      5) Create a branch of the mason-registry and add your package's ``Mason.toml`` under ``Bricks/<package_name>/<version>.toml``
      6) Add a source field to your ``<version>.toml`` pointing to your package's repository.
      7) Open a PR in the mason-registry for your newly created branch containing just your <version>.toml.
      8) Wait for mason-registry maintainers to approve the PR.

Once your package is uploaded,  please notify the chapel team if your package should be taken down.
Your package may be removed by the Mason maintainers if the integrity of the package is not maintained.



If you have a personal remote registry, ``mason publish <path-to-registry>``  also accepts
a remote path to a git repository. This will create a branch to your registry that adds
your package, and you can approve the PR to merge your new package into your registry.
Make sure to ensure that your package has a remote origin in order to publish remotely.

Publishing to a personal remote registry

.. code-block:: sh

   cd PackageA
   mason publish <remote-path-to-registry>

To assess the ability of your package to be published to the mason-registry or
a personal registry, run ``mason publish --dry-run <path-to-registry>`` for a
series of quick checks or ``mason publish --check <path-to-registry`` for a more
in depth check that will build your packages and run the full test suite.

Semantic Versioning
===================

To assist version resolution, the mason registry will enforce the following conventions:

The format for all versions will be ``a.b.c``.
   Major versions are denoted by ``a``.
   
   Minor versions are denoted by ``b``.
   
   Bug fixes are denoted by ``c``.
   

- If the major version is 0, no further conventions will be enforced.

- The major version must be advanced if and only if the update causes breaking API changes,
  such as updated data structures or removed methods and procedures. The minor and bug fix
  versions will be zeroed out. (ex. 1.13.1 -> 2.0.0)

- The minor version must be advanced if and only if the update adds functionality to the API
  while maintaining backward compatibility with the current major version. The bug fix
  version will be zeroed out. (ex. 1.13.1 -> 1.14.0)

- The bug fix must be advanced for any update correcting functionality within a minor revision.
  (ex. 1.13.1 -> 1.13.2)


Incompatible Version Resolution Strategy
========================================

The current resolution strategy for Mason 0.1.0 is the IVRS as described below:
    1. If multiple bug fixes of a package are present in the package,
       mason will use the latest bug fix. (ex. 1.1.0, 1.1.1 --> 1.1.1)
    2. If multiple minor versions of a package are present in the package,
       mason will use the latest minor version within the common major version.
       (ex. 1.4.3, 1.7.0 --> 1.7)
    3. If multiple major versions are present, mason will print an error.
       (ex. 1.13.0, 2.1.0 --> incompatible)

