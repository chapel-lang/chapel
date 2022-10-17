:title: Mason Guide: *Documentation*

Documenting a Package
~~~~~~~~~~~~~~~~~~~~~

Creating a website for package documentation is a breeze with Mason. Mason uses :ref:`readme-chpldoc`, which turns any ``.chpl`` file
into ``Sphinx`` documentation. To document a package, run the command ``mason doc`` while inside of a package. The
documentation will be automatically generated as long as ``chpldoc`` has been set up. For instructions on how to set up
``chpldoc``, view its documentation. Documentation will be built into the ``doc/`` folder that will be created upon
the first call of ``mason doc``.
