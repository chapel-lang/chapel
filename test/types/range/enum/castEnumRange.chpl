enum color { red=1, green=2, blue=3 };
{
  var r = color.red..color.blue;
  var r2 = r: range(int);
  writeln(r2);
}
