interface LessThan {
  proc LT(x:self, y:self):bool;
}

proc LT(x:int, y:int):bool {
  if (x < y) {
    return true;
  } else {
    return false;
  }
}

int implements LessThan;

proc min(type T, x:T, y:T) : T where T implements LessThan {
  if (LT(y,x)) {
    return y;
  } else {
    return x;
  }
}

min(int,3, 4);
