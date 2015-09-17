======================================================
Initial support for formatting numeric types in Chapel
======================================================

This README describes some initial support that we have added to the
compiler for formatting type-to-string conversions in Chapel.  This
mechanism should be considered a stopgap technology until we have
developed and implemented a more robust story, which is why it's being
described in this README rather than the language specification.


Value-to-string formatting
--------------------------

For the most part, value-to-string conversions in Chapel have a
default, fixed format that cannot be changed by the user.  We are
working towards a rich value-to-string cast mechanism that will
support a number of formatting options, but in the meantime we have
added a built-in function, "format()" that can be used to convert a
numeric value to a string.  

The prototype for the format function is as follows:

.. code-block:: chapel

   proc format(fmt: string, val): string;

where "fmt" is a format string and "val" is a scalar value.  This
function can currently be used in one of two forms:

1) ``format(<C format string>, val)``

   In this form, the first argument is expected to be a string that
   can serve as a C sprintf()-style format string for the second
   argument.  Note that because C's integer types can vary in size and
   the format flags correspond to the C types, format flags may need
   to be changed between back-end C compilers/platforms that support
   different data type sizes.

   As an example:

    .. code-block:: chapel

            var onethird = 1.0/3.0;
            var numItems = 32;

            writeln("onethird is: ", format("%13.8f", onethird));
            var myString: string = format("%12d", numItems);


   The current implementation of format does not provide good support
   for complex numbers, requiring the user to treat the real and
   imaginary components manually.  Moreover, for imaginary types, the
   "i" character will not appear unless it is explicitly given in the
   format string.


2) ``format(<pattern string>, val)``

   In this form, the first argument is a string composed of ``#`` and
   ``.`` characters that indicates the pattern that should be used to
   print the value.  In this pattern, a ``#`` character indicates a
   digit or a blank while ``.`` indicates where the decimal place should
   fall.  The second argument must be a numeric type -- integral,
   real, imaginary, or complex.

   As an example:

.. code-block:: chapel

     var pi = 3.14159265;
     var numItems = 32;

     writeln("pi is: ", format("#.####", pi));
     var myString: string = format("###", numItems);


Future Directions
-----------------

Over time, we will be improving the support and robustness of these
format() calls, and moving them into a more language-oriented solution
for performing value-to-string conversions.  If you have specific
feature requests or suggestions, please let us know at:
chapel_info@cray.com.
