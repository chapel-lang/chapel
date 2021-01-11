.. _readme-buildingdocs:

=============================
Building Chapel Documentation
=============================

The easiest way to build the Chapel web documentation is to use the top level
make target::

   make docs

This will build the documentation for the standard and internal modules using
chpldoc, and symlink all of the ``$CHPL_HOME/doc/`` documents over to
a sphinx project that lives in ``$CHPL_HOME/doc/``. Once built,
the documentation can be viewed at ``$CHPL_HOME/doc/html/index.html``.
