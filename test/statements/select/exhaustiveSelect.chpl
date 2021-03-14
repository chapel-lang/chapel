enum myenum {one, two};
config const enumval = myenum.one;

proc myproc(e: myenum) {
  select e {
    when myenum.one do return 1;
    when myenum.two do return 2;
  }
}

writeln(myproc(enumval));
