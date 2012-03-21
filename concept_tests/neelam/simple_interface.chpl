interface LessThan {
  proc LT(x:int, y:int) : bool;
}

proc LT(x:int, y:int) : bool {
  return x < y;
}

int implements LessThan;
