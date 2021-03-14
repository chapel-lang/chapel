record R {
  proc deinit () {writeln("Inside ~R");}
}

var r: R;
writeln("Bye bye!");
