interface LessThan {
  proc <(x:self, y:self):bool;
}

proc minFn(x:?T, y:T):T where T implements LessThan {
  if (y < x) {
    return y;
  }
  else {
    return x;
  }
}

writeln(minFn(3, 4));

