.. default-domain:: chpl

.. module:: C_Env

C_Env
=====
**Usage**

.. code-block:: chapel

   use MPI.C_Env;

.. function:: proc getenv(name: c_string): c_string

.. function:: proc setenv(name: c_string, envval: c_string, overwrite: c_int): c_int

