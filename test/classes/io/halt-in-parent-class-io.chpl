class parent : writeSerializable {
  var p: int;
  override proc serialize(writer, ref serializer) throws { halt("halt"); }
}
class child: parent { var c: int; }
writeln(new child());
writeln(new parent());

