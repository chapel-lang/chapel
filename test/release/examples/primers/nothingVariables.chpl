// The 'nothing' type

/*
   This primer demonstrates the use of the ``nothing`` type on variables and
   class/record fields. A variable or field that is of type ``nothing`` is
   removed by the compiler and doesn't exist during program execution.

   The value ``none`` has type ``nothing`` and can be assigned to a
   ``nothing`` variable, or returned from a function with ``nothing``
   return type.

   .. note:: 

     The ``nothing`` type is not to be confused with the ``void`` type, which
     is the type of a function that does not return any value.
*/

// Nothing variables
// -----------------

// This variable will be removed by the compiler. It is an error to assign a
// non-nothing value to it, or to use it where a non-nothing value is expected.
var nothingVar: nothing;

// The value ``none`` is the only value of the ``nothing`` type.
var nothingVar2 = none;

// The following (commented out) statements are compile time errors::
/*
// nothingVar = 1;
// var x = 2 * nothingVar;
*/

// A ``nothing`` variable can be passed to a generic function as long as the
// function doesn't do anything that expects it to be a value. The following
// call to ``writeln()`` will simply print a blank line, as though the
// argument was never there.

writeln(nothingVar);

// Conditional variables
// ---------------------

// A variable that will just be removed by the compiler doesn't seem very
// useful on its own. But with the addition of compile time folding of
// ``param`` values, it can be used to remove variables conditionally.

config param useMultiplier = false;

// If useMultiplier is ``false``, the ``multiplier`` variable will be removed
// by the compiler.  Since all uses of ``multiplier`` are guarded by the
// ``useMultiplier`` param, they will also be removed.
const multiplier = if useMultiplier then 3.5 else none;
var value = 1.0;

if useMultiplier {
  value *= multiplier;
}

writeln(value);

// Conditional fields
// ------------------

// The ``nothing`` type is extra useful for class and record fields, where a
// large number of instances may be created, so removal of fields could save
// a substantial amount of memory or cache space. An example is a record
// containing two different implementations of its functionality, e.g. for
// two different platforms.
record nothingRecord {
  param useImpl2: bool = false;
  var impl1Var1 = if useImpl2 then none else 1;
  var impl1Var2 = if useImpl2 then none else 2.0;

  var impl2Var1 = if useImpl2 then 3.0 else none;
  var impl2Var2 = if useImpl2 then "4.0" else none;

  proc myProc() {
    if useImpl2 {
      writeln((impl2Var1, impl2Var2));
    } else {
      writeln((impl1Var1, impl1Var2));
    }
  }
}

// - ``vr1`` doesn't contain the fields ``impl2Var1`` or ``impl2Var2``
// - ``vr2`` doesn't contain the fields ``impl1Var1`` or ``impl1Var2``
var vr1 = new nothingRecord(useImpl2=false),
    vr2 = new nothingRecord(useImpl2=true);

vr1.myProc();
vr2.myProc();

// By leaving out the unnecessary fields, the memory and cache footprint
// are significantly reduced, resulting in potential performance improvements
config const n = 1000;
var A: [1..n] nothingRecord(useImpl2=false);
for vr in A {
  if vr.useImpl2 {
    vr.impl2Var1 = 1.1;
    vr.impl2Var2 = "hello world!";
  } else {
    vr.impl1Var1 = 42;
    vr.impl1Var2 = 3.14;
  }
}

// Although ``nothing`` variables don't seem useful at first glance, they can
// be used to conditionally remove unnecessary variables reducing memory
// and cache footprint. This can lead to less memory overhead and better
// performance.
