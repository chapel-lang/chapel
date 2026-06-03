use Reflection;
record R {
  var a: int;
  var b: string;
  var c: real;
}

writeln(getFieldName(R, 4));
