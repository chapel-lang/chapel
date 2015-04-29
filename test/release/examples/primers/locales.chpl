/*
 * Locales primer
 *
 * This example showcases Chapel's locale types.  To run this example
 * using multiple locales set up your envionment as described in
 * README.multilocale and execute it using the "-nl #" flag to specify
 * the number of locales.  For example, to run on 2 locales, run
 * "./a.out -nl 2".
 *
 */

use Memory;

//
// In Chapel, the 'locale' type refers to a unit of the machine
// resources on which your program is running.  Locales have the
// capability to store variables (i.e., they have memory) and to run
// Chapel tasks (i.e., they have processors).  The specific definition
// of 'locale' for a given target architecture is determined by the
// Chapel compiler and architecture.  In practice for a standard
// distributed memory architecture a single multicore/SMP node is
// typically considered a locale.  Shared memory architectures are
// typically considered a single locale.  See the notes in
// $CHPL_HOME/doc/platforms/ for platform-specific notes that might
// relate to locales on a given architecture.
//
// As mentioned above, the number of locales on which a Chapel program
// should be run is specified on the executable command line using the
// -nl <numLocales> (or --numLocales=<numLocales>) flag.  This will
// cause the Chapel's launcher to execute the program on that many
// locales.
//

//
// Within a Chapel program, the number of locales can be referred
// to symbolically using the built-in numLocales variable.  For
// example:
//
writeln("This program is running on ", numLocales, " locales");

//
// Chapel programs always start executing from locale #0.  The
// locale on which the current piece of code is executing can
// always be queried in a Chapel program using the builtin 'here'
// variable which has locale type.  All locale values query an
// 'id' query that returns its unique value from 0 to numLocales-1.
// Here we verify that this program began running on locale 0:
//

writeln("It began running on locale #", here.id);
writeln();

//
// Chapel also provides a built-in Locales array that represents the
// set of locale values corresponding to the machine resources on
// which the program is running.  This Locales array is the main
// way for bootstrapping distributed memory computations, whether
// explicitly (using on-clauses) or implicitly (using distributions).
//
// As an example, the following for loop iterates over the Locales
// array and then uses an on-clause to specify that each iteration
// should execute on that specific locale.  Within the body of the
// loop we use 'here' and the '.id' query to indicate what locale
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
// parallelism across distributed memory machine resources, but in
// this example we focus only on the locality issues, not parallelism.
//

//
// Locale values can be assigned and stored like any other value.
// For example, the user can create their own view of the Locales
// array.  The following statement creates a 1..10 array of Locales
// in which we'll store our numLocales unique locale values
// redundantly if numLocales is < 10:
//
var MyLocaleArray: [1..10] locale;

for i in 1..10 do
  MyLocaleArray[i] = Locales[(i-1)%numLocales];

for i in 1..10 do
  on MyLocaleArray[i] do
    writeln("MyLocaleArray[", i, "] is really locale ", here.id);
writeln();

//
// Similarly, the user could arrange the locales into a
// multidimensional virtual locale grid by storing the locale
// values into a higher-dimensional array of locales (the array
// reshape() method can be particularly useful for this purpose).
//

//
// In addition to the .id query mentioned above, locales support a
// number of other queries about their properties.  For example:
//
//   locale.name     : returns a string indicating the locale's name
//   locale.numCores : returns the number of processor cores on the locale
//   locale.physicalMemory() : returns the amount of memory on the locale
//   locale.maxTaskPar : returns the likely maximum parallelism available
//                       on the locale
//

config const printLocaleInfo = true;  // permit testing to turn this off

if printLocaleInfo then
  for loc in Locales do
    on loc {
      writeln("locale #", here.id, "...");
      writeln("  ...is named: ", here.name);
      writeln("  ...has ", here.numCores, " processor cores");
      writeln("  ...has ", here.physicalMemory(unit=MemUnits.GB, retType=real), 
              " GB of memory");
      writeln("  ...has ", here.maxTaskPar, " maximum parallelism");
    }
writeln();


//
// Chapel variables are stored using the memory of the locale
// executing the task that encounters the variable declaration.
// Thus, in the following code, x is declared on locale 0 and
// y is declared on locale 1 if numLocales > 1 (0 otherwise).
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
// what is known as a global namespace (or global address space)
// quality in a language.  The fact that locality within a Chapel
// program can be semantically reasoned about by the programmer (i.e.,
// "x is on Locale[0], y is on Locale[1]") makes it a PGAS or
// Partitioned Global Address Space language (though we prefer the
// less broadly-used term "Partitioned Global Namespace").
//
// Not only can a programmer reason about a variable or task's
// location abstractly, they can also query these values directly
// in the language.  In particular, all Chapel variables support a
// .locale query which returns the locale value that is storing that
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
// with storage associated with it.  "on x" effectively says "execute
// this on whichever locale owns x" or equivalently "on x.locale".
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
// Next, let's create a simple distributed data structure using the
// principles above.  In particular, we'll create a linked list that
// spans the locales with a list node per locale.
//

//
// First, define a standard Node class for linked lists.  Note that
// even though the 'next' fields will span locales, they don't need
// to be declared specially in any way, again thanks to the [P]GAS
// nature of the language.
//
class Node {
  var data: real;
  var next: Node;
}


//
// Next, we'll iterate through the locales, allocating a node on each.
// It's worth reviewing that a class variable (like the 'next' field
// above) is just a reference in Chapel and the object it points to
// is a separate thing allocated by a call to 'new'.  So in general,
// the reference and object can live on different locales (as they
// will here.
//
// So, in the following loop, 'current' will always point at the last
// node we created while our on-clause will move the task ahead to
// the next locale.  That way, our execution of 'new' will create
// the new object on that "next" locale.
//
var head = new Node(0);
var current = head;
for i in 1..numLocales-1 do
  on Locales[i] {
    current.next = new Node(i);
    current = current.next;
  }

//
// Loop over the linked list and output each node and the locale on
// which it exists using .locale.id to determine the locale.  This
// computation takes place entirely on Locales(0) and accesses remote
// memory as necessary.
//
current = head;
while current {
  writeln("node with data = ", current.data, " on locale ", current.locale.id);
  current = current.next;
}
writeln();

//
// Alternatively, we could use a data-driven execution to walk the
// linked list.  Note that on-clauses, when applied to class
// variables, resolve to the locale of the object, not the reference.
//
current = head;
while current {
  on current {
    writeln("node with data = ", current.data, " on locale ", here.id);
    current = current.next;
  }
}

//
// Give back the memory allocated by the node list.
//
delete head;

//
// For more information about Locales, refer to the Locales chapter of
// the language spec.
//
