record R { var x: int; }

proc main() {
  var data: [1..1] R;
  data[1] = data[1];
  writeln(data);
}
