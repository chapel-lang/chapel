record iteratorInitEquals {
  iter init=(x) {}
}

proc main() {
  var i1 = new iteratorInitEquals();
  var i2 = i1;
  writeln(i2);
}
