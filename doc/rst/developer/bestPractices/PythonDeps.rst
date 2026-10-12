.. _readme-python-deps:

============================
Updating Python Dependencies
============================

The process for updating our Python dependencies is relatively straight-forward,
but the steps are listed here for convenience.

Go through:
  - Sphinx domain repo requirements files
  - requirements files in Chapel repo third-party/chpl-venv
  - Chapel repo tools/chapel-py/pyproject.toml file

For each package listed, search pypi.org for the package name and see if there
is a more recent version.

.. note::

  Be sure to check the Python versions required by the package to make sure it
  is the oldest version we run.  If not, you may need to update the Python
  version we run on various systems.

Sometimes package updates conflict with the requirements of other packages.  You
may need to update to a slightly older version of one of the packages to resolve
this conflict.

If updates are needed to the Sphinx domain’s requirements, you will need to push
a new version to pypi, follow the release instructions for that listed in the
repo's `README file
<https://github.com/chapel-lang/sphinxcontrib-chapeldomain/blob/main/README.rst>`__.

This process should generally happen every six months.
