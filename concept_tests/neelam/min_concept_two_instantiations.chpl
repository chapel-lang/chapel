interface LessThan {
  proc <(x:self, y:self):bool;
}

int implements LessThan;
real implements LessThan;

proc min(type T, x:T, y:T) : T where T implements LessThan {
  if (y < x) {
    return y;
  } else {
    return x;
  }
}

min(real,3.0,4.0);
min(int,3, 4);
