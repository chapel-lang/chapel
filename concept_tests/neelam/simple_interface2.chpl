interface LessThan {
  proc LT(x:int, y:int) : bool;
}

proc LT(x:int(64), y:int) : bool {
  return x < y;
}

int implements LessThan;
