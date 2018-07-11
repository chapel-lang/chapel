-----------------------------------------------------
c2chapel -- A tool to generate  C bindings for Chapel
-----------------------------------------------------

``c2chapel`` is a tool to help the Chapel programmer generate extern types,
records, and procedures when given a C99 file. For example, given the following
function declaration in a header file:

.. code-block:: c

  void foo(char* str, int n);

``c2chapel`` will generate Chapel code similar to:

.. code-block:: chapel

  extern proc foo(str: c_string, n : c_int) : void;

See the `online documentation <https://chapel-lang.org/docs/tools/c2chapel/c2chapel.html>`_ for more
information.
