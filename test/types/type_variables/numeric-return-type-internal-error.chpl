proc foo(): numeric {
  var y = true;
  if y {
    return 1;
  } else {
    return 1.0;
  }
}

proc main() {
  var z = foo();
  compilerWarning(z.type:string);
}
