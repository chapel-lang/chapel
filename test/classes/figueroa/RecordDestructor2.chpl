record R {
  def ~R () {writeln("Inside ~R");}
}

def foo {
  var r: R;
}

foo;
writeln("Bye bye!");
