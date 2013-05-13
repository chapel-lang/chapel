record R {
  proc ~R () {writeln("Inside ~R");}
}

proc foo() {
  var r: R;
  sync {
    begin {
      var myR2: R = r;
    }
  }
  writeln("Bye bye!");
}

foo();
