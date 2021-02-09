interface LessThan(T) {
  proc LT(x:T, y:T):bool;
}

proc LT(x:int, y:int):bool {
  return x<y;
}

proc EQ(x:int, y:int):bool {
  return x == y;
}

implements LessThan(int);

proc DoLTE(x:?U, y:U):U {
  // Error: No function EQ(U, U) in scope
  return LT(x, y) || EQ(x, y);
}

proc minFn(x:?T,y :T):T where implements LessThan(T) {
  return DoLTE(x);
}

writeln(minFn(3,4));
