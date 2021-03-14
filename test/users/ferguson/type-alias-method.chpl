type my_type = c_string;

proc my_type.doit() {
  writeln(this);
}

var str = "bla":c_string;
str.doit();

