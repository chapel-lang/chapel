record R {
  proc ~R () {writeln("Inside ~R");}
}

proc foo() {
  var r: R;
  writeln("Bye bye!");
}

foo();
