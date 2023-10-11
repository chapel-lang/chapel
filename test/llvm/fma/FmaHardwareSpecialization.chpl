use Math;

// We use 'config const' as input so that the target compiler cannot
// statically optimize away the hardware FMA instruction we're
// looking for.
config const x = 2;
config const y = 2;
config const z = 2;

// Not used in the test proper, but upon inspection in the disassembly,
// reveals x*y+z is not optimized into a single instruction.
inline proc myfma(x, y, z) do return x*y+z;

proc main() {
  var n1 = fma(x, y, z);
  var n2 = myfma(x, y, z);
  assert(n1 == n2);
  writeln(n1);
}
