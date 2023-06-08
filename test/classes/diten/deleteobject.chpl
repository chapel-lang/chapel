proc main {
  var o:unmanaged RootClass = new unmanaged RootClass();
  writeln(o);
  delete o;
}
