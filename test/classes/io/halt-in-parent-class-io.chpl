class parent {
  var p: int;
  override proc writeThis(f) throws { halt("halt"); }
}
class child: parent { var c: int; }
writeln(new child());
writeln(new parent());

