Chapel's Deprecation Policy
===========================

When removing a feature in Chapel, the feature is to be
deprecated and supported for at least 1 release cycle.

Major feature removals can be supported for more than 1 release cycle.

Non-public interface features can be removed or modified without deprecation.

This is the deprecation process:

1. Provide a warning or error message when a deprecated feature is used. This
   may be in the module code, compiler, or runtime, and can be emitted at
   compile-time or runtime.

The deprecation message uses the following format:

..code-block::

    foo is deprecated - please use bar

   For compile-time messaging you can use ``compilerWarning()`` to do
   this, but when deprecating symbols the ``deprecated`` keyword often
   works better because it can be applied to any symbol in a uniform
   way.  Look in the test/deprecated and test/deprecated-keyword
   directories for many examples.  Note that the ``deprecated`` keyword
   itself is likely to be replaced by different syntax in the future,
   though this may not affect its use in practice since deprecations
   typically only last for one release anyway.

2. Update the deprecated feature documentation to state that it is deprecated
   and point to the preferred feature.

For example:

..code-block:: rst

    Foo module short description.

    .. warning::

        This module has been deprecated - please use :mod:`bar` instead.

    Foo module long description.


3. Add a deprecation test to ``test/deprecated/`` to:

  - confirm the deprecation warning works as intended
  - remind us to to remove the deprecated feature in the following release

Optionally include deprecation timeline in comments of deprecation test:

.. code-block:: chapel

    // Deprecated in Chapel 1.14, to be removed in Chapel 1.15
