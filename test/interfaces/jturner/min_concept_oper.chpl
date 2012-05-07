interface LessThan {
  proc <(x:self, y:self):bool;
}

int implements LessThan;

proc minFn(x:?T, y:T):T where T implements LessThan {
  if (y < x) {
    return y;
  }
  else {
    return x;
  }
}

writeln(minFn(5, 4));

