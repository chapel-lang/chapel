.. default-domain:: chpl

.. module:: ChapelComplex_forDocs
   :synopsis: Complex is a predefined structured type in Chapel. It is specified

Complex
=======

Complex is a predefined structured type in Chapel. It is specified
in the Types chapter of the Chapel Language Specification.


.. record:: complex

   
   It is defined as if it were a record with two fields.
   Note that both of these fields are of type *real*.
   Specifically the imaginary component is not of type *imag*.
   This is important when using the getter/setter for the
   field *im*.
   


   .. attribute:: var re: real

   .. attribute:: var im: real

.. method:: proc complex.re ref

   
   When used as a value, this returns the real component of
   the complex number as a *real*.
   
   When used as an lvalue, this is a setter that assigns the
   real component.
   

.. method:: proc complex.im ref

   
   When used as a value, this returns the imaginary component of
   the complex number as a *real*.
   
   When used as an lvalue, this is a setter that assigns the
   imaginary component.
   

