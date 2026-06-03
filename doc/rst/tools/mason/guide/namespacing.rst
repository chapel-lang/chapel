Namespacing
===========

All Mason packages will exist in a single common namespace with a first-come,
first-served policy. It is easier to go to separate namespaces than to roll
them back, so this position affords flexibility.

It is expected that all accessible symbols should be made available with an
include of the single module at ``src/package-name.chpl``. To facilitate the
use of multiple Chapel modules in a single Chapel module, it is suggested to
use Chapel submodules (see :ref:`readme-module_include`).

Expecting users to use ``-M`` of a Mason package is not recommended as this can
cause conflicting namespaces that would otherwise be avoided by the Mason
registry requirements.
