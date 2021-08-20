enum color { red=100, green=200, blue=300 };
{
  var r = color.red..color.blue;
  var r2 = r: range(int, stridable=false);
  writeln(r2);
}
