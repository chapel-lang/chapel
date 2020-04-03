pragma "suppress lvalue error" // or error!
proc intfn(const ref arg: int) {
  arg = 0;
}
proc tupfn(arg: 1*int) {
  arg(0) = 0;
}


proc main() {
  var i = 1;
  var tup = (1,);

  intfn(i);
  tupfn(tup);

  writeln(i);
  writeln(tup(0));
}
