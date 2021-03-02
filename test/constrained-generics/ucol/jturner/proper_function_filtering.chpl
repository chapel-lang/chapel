interface IFace(T){ }

implements IFace(int);

proc foo(x) {
  return x;
}

proc foo(x:int) {
  return x + 100;
}

proc ifaced(x:?T):T where implements IFace(T) {
  return foo(x);
}

writeln(ifaced(1));
