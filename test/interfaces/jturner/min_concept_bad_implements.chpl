interface LessThan {
  proc LT(x:self, y:self):bool;
}

int implements LessThan;

proc minFn(x:?T, y:T):T where T implements LessThan {
  if (LT(y, x)) {
    return y;
  }
  else {
    return x;
  }
}

writeln(minFn(3, 4));
