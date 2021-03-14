record R {
  proc deinit () {writeln("Inside ~R");}
}

proc foo() {
  var r: R;
  writeln("Bye bye!");
}

foo();
