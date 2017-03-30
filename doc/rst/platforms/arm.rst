.. _readme-arm:

==========================
Using Chapel on 64-bit ARM
==========================

Initial support is provided for running Chapel programs on a 64-bit
ARM system.

Building and using Chapel for ARM is similar to other platforms in
most respects.  Please refer to the :ref:`chapelhome-quickstart` for
more information.

------------------
Installation notes
------------------

Only cross compilation has been tested.  This requires setting
``CHPL_TARGET_PLATFORM`` to ``aarch64`` before building Chapel.  For
example:

.. code-block:: sh

    export CHPL_TARGET_PLATFORM=aarch64

However, it should be possible for an advanced user to get a native
Chapel compiler running, which would include setting
``CHPL_HOST_PLATFORM`` to ``aarch64``.

Multilocale support is only available via the GASNet UDP conduit at
this time.  Please see :ref:`readme-multilocale` for details.

Building multilocale support for ARM requires gcc 6 or later.
