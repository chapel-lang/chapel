// Force fixups to occur by defining a type 'data' _after_ its uses. Fixups
// happen when we lower to the middle-end's IR because we may generate
// resolved mentions of a type before that type itself has been lowered.
proc main() {
  proc foo(fn, x: data) { return fn(x); }
  proc f(x) { return x; }
  var d = foo(proc(x: data) {
    writeln('Hello world!');
    return f(x); 
  }, new data());
}

record data {}
