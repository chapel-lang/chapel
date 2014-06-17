record R {
  proc ~R () {writeln("Inside ~R");}
}

proc foo {
  var r: R;
}

foo;
writeln("Bye bye!");
