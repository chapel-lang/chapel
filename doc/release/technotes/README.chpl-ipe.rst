The Interactive Programming Environment
========================================

The Chapel team has initiated an effort to develop an Interactive
Programming Environment (IPE) for Chapel.  This system will allow
Chapel developers to write applications in an incremental and
interactive fashion e.g.::

    IPE 0.1

    1 > var x = 20;
    2 > proc double(x : int) : int return x + x;
    3 > writeln('2 * x is ', double(x));
        2 * x is    40

without the traditional edit-compile-run cycle.

Release 1.11 includes a proof-of-concept implementation of this
effort.  The current functionality is extremely limited, and somewhat
fragile, but we believe that there is enough behavior to provide a
sense of the direction we are pursuing.

It is expected that the IPE will mature reasonably quickly over the
coming months.  If the IPE is of interest to you then you may wish
to track our progress on Github.

**Contents**:

#. `No Additional Installation`_
#. Overview_
#. `The IPE subset of Chapel`_
#. `Future directions`_


.. _No Additional Installation:

No Additional Installation
--------------------------

For this release there are no additional installation instructions.
If you have a working version of the Chapel compiler then the IPE
should also be in your path!  Just run ``chpl-ipe``.


.. _Overview:

Overview
--------

To use the interpreter on a unix-like system do::

    prompt> chpl-ipe


You will be greeted with::

    IPE 0.1

    1 >

i.e. an indication that you have started version 0.1 of the IPE and then an
interactive prompt.  At this point you can type simple Chapel statements e.g.::

    IPE 0.1

    1 > var x = 20;
    2 > writeln('X  is ', x);
        X  is    20
    3 > proc double(x : int) : int return x + x;
    4 > writeln('2X is ', double(x));
        2X is    40
    5 >



For convenience you can specify the name of a file that contains Chapel definitions
on the command line.  This file will be processed by the interpreter and then the
interactive prompt will appear.  This saves typing the same code each time you
start the IPE. Suppose the file *Square.chpl* contains the definition of a procedure
that computes the square of an integer value.  You could then do::

     > chpl-ipe Square.chpl

     IPE 0.1

     1 > writeln('square(5) => ', square(5));
         square(5) =>    25
     2 >




.. _The IPE subset of Chapel:

The IPE subset of Chapel
------------------------

The version of the IPE that is included in the 1.11 release implements a
very small subset of Chapel.  Once again, this is expected to change
quickly during the coming months and so you may wish to track the github
version of the code base.


Variables
~~~~~~~~~

The IPE supports variables with types bool, int, real, and c_string.
These are declared in the natural way with local type inference e.g.::

      1 > var x : int = 20;
      2 > var y = 30.3;             // Equivalent to var y : real = 30.3;
      3 > var z : int;              // Equivalent to var z : int  =  0;


Assignment
~~~~~~~~~~

The assignment operator is defined for bool, int, and real e.g::

      1 > var x : int;              // x is  0
      2 > var y = 42;               // y is 42
      3 > x = y;                    // x is now 42

The types of the LHS and RHS must match.  There is currently no
support for Chapel's implicit coercion from int to real during
assignment.



Arithmetic operators
~~~~~~~~~~~~~~~~~~~~

The IPE implements the unary +, - operators on int and real and the
binary operators +, -, *, / on int and real.  Once again the IPE
does not currently implement implicit coercion from int to real.



Equality operators
~~~~~~~~~~~~~~~~~~

The IPE implements == and != on pairs of bool, int, and real.



Comparison operators
~~~~~~~~~~~~~~~~~~~~

The IPE implements <, >, <=, >= on pairs of int and real.




If statement
~~~~~~~~~~~~

The IPE implements the IF statement e.g. the code::

   var value : int = 0;

   writeln('value (expect  0) ', value);

   if true then
     value =  6;

   writeln('value (expect  6) ', value);

   if false then
     value =  2;

   writeln('value (expect  6) ', value);

   if value > 9 then
     value =  2;
   else
     value = 10;

   writeln('value (expect 10) ', value);

will produce::

   value (expect  0)     0
   value (expect  6)     6
   value (expect  6)     6
   value (expect 10)    10




While statement
~~~~~~~~~~~~~~~

The IPE implements a restricted form of the while statement; it does
not support the break or continue statement e.g. entering the following
in to the IPE::

   var i   : int =  0;
   var n   : int = 10;

   var res : int =  1;

   while (i < n)
   {
     res = res * 2;
     i   = i   + 1;
   }

   writeln('i:   ', i);
   writeln('n:   ', n);
   writeln('res: ', res);

will produce::

   i:      10
   n:      10
   res:  1024


User defined procedures
~~~~~~~~~~~~~~~~~~~~~~~

The IPE supports user defined procedures with the limitation that
any formals must be explicitly typed and cannot be named or take a
default value.  Additionally the type of the return value, if any,
must be specified.  If there is no return value, then void must be
specified.  For example the following implements square() for an
integer::

   1> proc square(x : int) : int return x * x;


Return statement
~~~~~~~~~~~~~~~~

The IPE implements the return statement within a procedure
with the limitation that if it appears, then it must be the
final statement of the procedure.  This implies that there can
be only one return statement in a procedure and that a return
cannot appear within an if statement or a while statement.



Output functions
~~~~~~~~~~~~~~~~

There is a small set of output functions.  There is a procedure
print() which takes one argument of type bool, int, real, or c_string
and prints the type and the value e.g.::

      1 > var x = 30;
      2 > print(x);
          int      30

There is a restricted form of writeln().  The first version takes a
single c_string and prints the string and then a newline e.g.::

      1> writeln('Hello world');
         Hello world

The remaining versions take two arguments.  The first argument must be
a c_string, and the second argument must be a bool, int, or real.  This
prints the c_string followed by a representation of the second argument e.g.::

      1 > var x = 42;
      2 > writeln('The Answer to the Ultimate Question of Life, the Universe, and Everything is ', x);
          The Answer to the Ultimate Question of Life, the Universe, and Everything is    42





.. _Future directions:

Future directions
-----------------

We expect the IPE to mature rapidly during the development for release 1.12.  In the
immediate term we plan to

   * Stabilize the implementation so that a small variety of simple sequential
     programs can be executed

   * Support applications with multiple tasks by implementing the **begin** statement

   * Support multi-locale applications by implementing the **on** statement.

i.e. to go deep before we go wide.  Once we have developed this core functionality we can
turn our attention to, in no well-defined order,

   * Generic functions

   * Arrays and domains

   * Iterators

   * Classes and Records

   * Generic types

   * For loops, forall loops, and so on

It is not currently clear how much of this will be present in the 1.12 release but
we hope you share our excitement about the possibilities!
