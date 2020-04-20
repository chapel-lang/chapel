// Locales

/*
   This example showcases Chapel's locale types.  To run this example
   using multiple locales set up your environment as described in
   :ref:`multilocale documentation <readme-multilocale>` and execute it
   using the ``-nl #`` flag to specify  the number of locales.
   For example, to run on 2 locales, run: ``./locales -nl 2``
*/

//
// In Chapel, the ``locale`` type refers to a unit of the machine
// resources on which your program is running.  Locales have the
// capability to store variables (i.e., they have memory) and to run
// Chapel tasks (i.e., they have processors).  The specific definition
// of ``locale`` for a given target architecture is determined by the
// Chapel compiler and architecture.  In practice for a standard
// distributed memory architecture a single multicore/SMP node is
// typically considered a locale.  Shared memory architectures are
// typically considered a single locale.  See
// :ref:`platform-specific documentation <platforms-index>`
// for notes that might relate to locales on a given architecture.
//
// As mentioned above, the number of locales on which a Chapel program
// should be run is specified on the executable command line using the
// ``-nl <numLocales>`` or ``--numLocales=<numLocales>`` flag.  This will
// cause the Chapel's launcher to execute the program on ``<numLocales>``
// locales.
//

//
// Within a Chapel program, the number of locales can be referred
// to symbolically using the built-in ``numLocales`` variable.
// For example:
//
writeln("This program is running on ", numLocales, " locales");

//
// Chapel programs always start executing from locale #0.  The
// locale on which the current piece of code is executing can
// always be queried in a Chapel program using the builtin ``here``
// variable, which has ``locale`` type.  All ``locale`` values implement
// the ``id`` query that returns a unique value from 0 to ``numLocales-1``.
// Here we verify that this program began running on locale 0:
//

writeln("It began running on locale #", here.id);
writeln();

//
// Chapel also provides a built-in ``Locales`` array that contains the
// set of locale values corresponding to the machine resources on
// which the program is running.  This ``Locales`` array is the main
// way for bootstrapping distributed memory computations, whether
// explicitly (using on-clauses) or implicitly (using
// :ref:`distributions <primers-distributions>`).
//
// As an example, the following for loop iterates over the ``Locales``
// array and then uses an on-clause to specify that each iteration
// should execute on that specific locale.  Within the body of the
// loop we use ``here`` and the ``.id`` query to indicate what locale
// each iteration of the loop body is executing on.
//
for loc in Locales do
  on loc do
    writeln("hello locale ", here.id);
writeln();

//
// Note that in Chapel, locality and parallelism are orthogonal
// concepts.  That is, nothing about the use of the on-clause above
// introduced parallelism into the program, it only moved the current
// task (conceptually) from locale #0 to another locale.  Task and
// data parallel concepts can be intermixed with on-clauses to get
// parallelism across distributed memory machine resources. In this example
// we focus on the locality issues and do not illustrate parallelism.
//

//
// Locale values can be assigned and stored like any other value.
// For example, the user can create their own view of the ``Locales``
// array.  The following statement creates a ``1..10`` array of locales
// in which we'll store our ``numLocales`` unique locale values
// redundantly if ``numLocales`` is less than 10:
//
var MyLocaleArray: [1..10] locale =
      for i in 1..10 do Locales[(i-1)%numLocales];

for i in 1..10 do
  on MyLocaleArray[i] do
    writeln("MyLocaleArray[", i, "] is really locale ", here.id);

writeln();

//
// Similarly, the user could arrange the locales into a
// multidimensional virtual locale grid by storing the locale
// values into a higher-dimensional array of locales. The array
// :proc:`~ChapelArray.reshape()` function can be particularly useful
// for this purpose.
//

/*
 In addition to the ``.id`` query mentioned above, locales support a
 number of other queries about their properties.  For example:

 * ``locale.name``       : returns a string indicating the locale's name
 * ``locale.hostname``   : returns a string indicating the locale's hostname
 * ``locale.numPUs()``   : returns the number of processor cores on the locale
 * ``locale.physicalMemory()`` : returns the amount of memory on the locale
 * ``locale.maxTaskPar`` : returns the likely maximum parallelism available
   on the locale
*/

use Memory;  // for physicalMemory()
config const printLocaleInfo = true;  // permit testing to turn this off

if printLocaleInfo then
  for loc in Locales do
    on loc {
      writeln("locale #", here.id, "...");
      writeln("  ...is named: ", here.name);
      writeln("  ...has hostname: ", here.hostname);
      writeln("  ...has ", here.numPUs(), " processor cores");
      writeln("  ...has ", here.physicalMemory(unit=MemUnits.GB, retType=real),
              " GB of memory");
      writeln("  ...has ", here.maxTaskPar, " maximum parallelism");
    }

writeln();


//
// Chapel variables are stored using the memory of the locale
// executing the task that encounters the variable declaration.
// Thus, in the following code, ``x`` is declared on locale 0 and
// ``y`` is declared on locale 1 when there is more than one locale
// (on locale 0 otherwise).
//

{
  var x: int = 2;
  on Locales[1 % numLocales] {
    var y: int = 3;
    writeln("From locale ", here.id, ", x is: ", x, " and y is: ", y);
    on Locales[0] {
      writeln("From locale 0, x is: ", x, " and y is: ", y);
    }
  }
  writeln();
}

//
// Note in the code above that a task can refer to any
// lexically-visible variable in Chapel regardless of the locales on
// which the variable is stored and the task is executing.  This is
// what is known as a *global namespace* (or *global address space*)
// quality in a language.  The fact that locality within a Chapel
// program can be semantically reasoned about by the programmer (i.e.,
// "``x`` is on Locale 0, ``y`` is on Locale 1") makes it a PGAS or
// *Partitioned Global Address Space* language (though we prefer the
// less broadly-used term *Partitioned Global Namespace*).
//
// Not only can a programmer reason about a variable or task's
// location abstractly, they can also query these values directly
// in the language.  In particular, all Chapel variables support a
// ``.locale`` query which returns the locale value that is storing that
// variable.  Modifying the above example slightly:
//

{
  var x: int = 2;
  on Locales[1 % numLocales] {
    var y: int = 3;
    writeln("x is stored on locale ", x.locale.id, ", while y lives on ",
            y.locale.id);
  }
  writeln();
}

//
// Moreover, Chapel's on-clauses can be controlled by any expression
// with storage associated with it.  ``on x`` effectively says "execute
// this on whichever locale owns ``x``" or equivalently ``on x.locale``.
// Here's a variation of the example above using this idiom:
//

{
  var x: int = 2;

  on Locales[1 % numLocales] {
    var y: int = 3;

    on x do
      writeln("Using a data-driven on-clause, I'm now executing on locale ",
              here.id);
  }

  writeln();
}


//
// Reasoning about the locality of classes is complicated somewhat by
// the fact that class variables have two components: (1) the object
// that contains the class fields and (2) the variable storing a
// reference to the object (where such variables may be 'nil' when
// they don't refer to an object).  Classes are generally considered
// to be stored on the locale where the object is stored rather than
// the one where the reference is stored, and to that end, applying
// `.locale` to a class variable will typically reflect the object's
// location.  However, when the class variable is `nil`, it will
// evaluate to the locale where the reference is stored.
//
// Let's explore this idea by creating a class instance and
// querying its locality.
//

class Data {
  var x:int;
}

var myData: unmanaged Data?; // myData is a class pointer stored on locale 0 whose default value is `nil`

on Locales[1 % numLocales] {
  writeln("at start of 'on', myData is on locale ", myData.locale.id);
  myData = new unmanaged Data(1);
  // now myData points to something on Locales[1]
  writeln("at end of 'on', myData is on locale ", myData.locale.id);
}
writeln("after 'on', myData is on locale ", myData.locale.id);

on myData {
  writeln("Using 'on myData', I'm now executing on locale ", here.id);
}

//
// We can now deallocate myData to ensure no memory leaks.
//

delete myData;

//
// For more information about locales, refer to the Locales chapter of
// the :ref:`Chapel Language Specification <chapel-spec>`.
//

