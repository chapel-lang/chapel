use CPtr;
proc f(ptr:c_ptr(real)) {
  writeln(ptr.deref());
}

proc main() {
  var a: c_array(int, 2);

  f(a);
}
