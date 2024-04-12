record myManager: contextManager {
  var x: int = 0;

  proc ref enterContext() ref: int {
    writeln('x is: ', x);
    return x;
  }

  proc exitContext(in err: owned Error?) {
    if err then halt(err!.message());
    writeln('x is: ', x);
  }
}
manage myManager as x {
  x = 1;
}
