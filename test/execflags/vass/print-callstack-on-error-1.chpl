// generate a call stack as the context for an error message

test1("test1");

def test1(x) {
  test2(x);
}

def test2(y) {
  test3(y);
}

def test3(z) {
  compilerError("error from test3, depth=0", 0);
}
