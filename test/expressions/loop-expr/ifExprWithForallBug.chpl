config const branch = true;


proc main() {
  var array: [1..10] int;
  var b = if branch
          then forall a in array do a
          else array;

  writeln(b[1].type:string, " ", b);
}

