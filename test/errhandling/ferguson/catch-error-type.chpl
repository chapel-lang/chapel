proc test1() {
  try {
    throw new Error();
  } catch e {
    writeln(e.type:string);
  }
}
test1();

class MyError : Error {
}

proc test2() {
  try {
    throw new MyError();
  } catch e: MyError {
    writeln(e.type:string);
  } catch e {
    writeln("catchall");
  }
}
test2();
