interface IFace{ }

int implements IFace;

proc foo(x) {
  return x;
}

proc foo(x:int) {
  return x + 100;
}

proc ifaced(x:?T):T where T implements IFace {
  return foo(x);
}

writeln(ifaced(1));
