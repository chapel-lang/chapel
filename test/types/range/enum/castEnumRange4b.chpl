
enum color {red=42, orange=56};

writeln((42..56): range(color)); // unstable warning
writeln((42..42): range(color)); // unstable warning

// a small reproducer

outer(1); // warning: 1
outer(2); // warning: 2
outer(2); // currently, "warning: 2" is not generated

proc outer(param p) {
  inner(p);
}

proc inner(param p) {
  compilerWarning(p:string);
//cf: compilerWarning(p:string, 2);
}

/*
The difference between the reproducer and the range-cast warning is that
the former is in the user code and the latter in internal modules.

One could argue that "warning: 2" should be produced just once,
the first time outer() is instantiated with 2, given that this happens
in user code. OTOH one could also argue that "warning: 2" should be produced
for each call path that starts at a generic function.

Whereas for the range-cast warning it is clear that it needs to be given twice.
*/
