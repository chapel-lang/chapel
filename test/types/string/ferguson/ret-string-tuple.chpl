proc delared_return():1*string {
  var x = "hi";
  return (x + " there declared",);
}

proc inferred_return() {
  var x = "hi";
  return (x + " there inferred",);
}

proc inferred_return_copy_initcopy() {
  var x = "hi";
  var ret = (x + " there inferred initcopy",);
  return ret;
}

proc inferred_return_copy_noinit_assign() {
  var x = "hi";
  var ret:1*string = noinit;
  ret = (x + " there inferred copy noinit assign",);
  return ret;
}

proc inferred_return_copy_defaultinit_assign() {
  var x = "hi";
  var ret:1*string;
  ret = (x + " there inferred copy defaultinit assign",);
  return ret;
}



proc main() {

  var y = delared_return();
  writeln(y);

  var x = inferred_return_copy_noinit_assign();
  writeln(x);

  // this one seems to work.
  var x2 = inferred_return_copy_initcopy();
  writeln(x2);

  // this one seems to work.
  var x3 = inferred_return_copy_defaultinit_assign();
  writeln(x3);



  // this one seems to work.
  var z = inferred_return();
  writeln(z);

}
