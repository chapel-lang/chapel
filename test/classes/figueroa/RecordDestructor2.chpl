record R {
  proc deinit () {writeln("Inside ~R");}
}

proc foo {
  var r: R;
}

foo;
writeln("Bye bye!");
