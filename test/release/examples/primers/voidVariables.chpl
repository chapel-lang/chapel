// Void variables and fields

/*
   This primer demonstrates the use of the ``void`` type on variables and
   class/record fields. A variable or field that is of type ``void`` is removed
   by the compiler and doesn't exist during program execution.
*/

// This variable will be removed by the compiler. It is an error to assign a
// non-void value to it, or to use it where a non-void value is expected.
var voidVar: void;

// The value ``_void`` is the value version of the ``void`` type. It is
// expected that the name ``_void`` is not a permanent choice and will be
// changed in the future.
var voidVar2 = _void;

// The following (commented out) statements are compile time errors::
/*
// voidVar = 1;
// var x = 2 * voidVar;
*/

// A ``void`` variable can be passed to a generic function as long as the
// function doesn't do anything that expects it to be a value. The following
// call to ``writeln()`` will simply print a blank line, as though the
// argument was never there.

writeln(voidVar);

// A variable that will just be removed by the compiler doesn't seem very
// useful on its own. But with the addition of compile time folding of
// ``param`` values, it can be used to remove variables conditionally.

config param useMultiplier = false;

// If useMultiplier is ``false``, the ``multiplier`` variable will be removed
// by the compiler.  Since all uses of ``multiplier`` are guarded by the
// ``useMultiplier`` param, they will also be removed.
const multiplier = if useMultiplier then 3.5 else _void;
var value = 1.0;

if useMultiplier {
  value *= multiplier;
}

writeln(value);

// The ``void`` type is extra useful for class and record fields, where a
// large number of instances may be created, so removal of fields could save
// a substantial amount of memory or cache space. An example is a record
// containing two different implementations of its functionality, e.g. for
// two different platforms.
record voidRecord {
  param useImpl2: bool = false;
  var impl1Var1 = if useImpl2 then _void else 1;
  var impl1Var2 = if useImpl2 then _void else 2.0;

  var impl2Var1 = if useImpl2 then 3.0 else _void;
  var impl2Var2 = if useImpl2 then "4.0" else _void;

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
var vr1 = new voidRecord(useImpl2=false),
    vr2 = new voidRecord(useImpl2=true);

vr1.myProc();
vr2.myProc();

// By leaving out the unnecessary fields, the memory and cache footprint
// are significantly reduced, resulting in potential performance improvements
config const n = 1000;
var A: [1..n] voidRecord(useImpl2=false);
for vr in A {
  if vr.useImpl2 {
    vr.impl2Var1 = 1.1;
    vr.impl2Var2 = "hello world!";
  } else {
    vr.impl1Var1 = 42;
    vr.impl1Var2 = 3.14;
  }
}

// Although ``void`` variables don't seem useful at first glance, they can
// be used to conditionally remove unnecessary variables reducing memory
// and cache footprint. This can lead to less memory overhead and better
// performance.
