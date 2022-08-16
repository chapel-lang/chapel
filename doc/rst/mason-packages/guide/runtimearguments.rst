:title: Mason Guide: *Runtime Arguments*

Runtime/Compilation Arguments
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For an example of forwarding arguments in a call to ``mason run``, a chapel program built in
mason might have a ``config const number`` that corresponds to a value used in ``MyPackage.chpl``.
To try out different values at runtime, pass the values for ``number`` to ``mason run`` as follows::

      mason run -- --number=100
      mason run -- --number=1000


.. note::

   Previous releases allowed flags meant for the compiler or chapel program to be mixed with
   those meant for ``mason build`` or ``mason run``, respectively. As of Chapel 1.25 and
   mason 0.2.0, flags not intended for ``mason`` must follow a double dash ``--`` regardless
   of if they conflict or not.
