:title: Mason Guide: *Environment*

Environment Variables
=====================

Mason can be configured by setting the following environment variables:

- ``MASON_HOME`` : Path to a directory where mason will store cached registry
  and package data. Defaults to ``$HOME/.mason``.
- ``MASON_REGISTRY`` : A comma separated list of ``name|location`` pairs, where
  ``name`` is a local name for the registry at ``location``. Defaults to
  ``mason-registry|https://github.com/chapel-lang/mason-registry``. If the
  ``name|`` part of a pair is omitted it is inferred to be the word following
  the final slash in ``location`` with any ``.git`` suffix removed.
- ``MASON_OFFLINE`` : A boolean value that prevents mason from making calls that
  require internet access when set to ``true``. Defaults to ``false``. Mason commands
  that support a ``--[no-]update`` flag can override the ``MASON_OFFLINE`` setting
  when ``--update`` is explicitly passed.

The ``mason env`` command will print the inferred or set values of these
environment variables. If a variable was set by the user, an asterisk will be
printed at the end of the line. For example, if ``$MASON_HOME`` was set:

.. code-block:: text

   > mason env
   MASON_HOME: /path/to/something *
   MASON_REGISTRY: mason-registry|https://github.com/chapel-lang/mason-registry
   MASON_OFFLINE: false

.. warning::

   If ``MASON_REGISTRY`` changes after invoking a mason command that updates the
   local copy of the registry (e.g. ``mason update``), the local copies of the
   registry and dependency sources will be removed.
