proc main() {
  var myBool: bool = true;

  writeln("iterating over param range variable");
  var r1 = 0.. # true;
  for k in r1 {
    writeln("k = ", k, " : ", k.type:string);
  }

  writeln("iterating over range variable");
  var r2 = 0.. # myBool;
  for k in r2 {
    writeln("k = ", k, " : ", k.type:string);
  }

  writeln("iterating over param range literal");
  for k in 0.. # true {
    writeln("k = ", k, " : ", k.type:string);
  }

  writeln("iterating over range literal");
  for k in 0.. # myBool {
    writeln("k = ", k, " : ", k.type:string);
  }
}
