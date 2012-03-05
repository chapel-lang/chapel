interface LessThan {
  proc LT(x: self, y:self) :bool;
}

proc min(type T, x:T, y:T) : T where T implements LessThan {
  if (LT(y,x)) {
    return y;
  } else {
    return x;
  }
}

module M1 {
  proc LT(x:int, y:int):bool {
    if(x < y) {
      return true;
    } else {
      return false;
    }
  }

  int implements LessThan;

  min(int,3,4);
}

module M2 {
  proc LT(x:int, y:int):bool {
    if(x > y) {
      return false;
    } else {
      return true;
    }
  }     

  int implements LessThan;

  min(int,3,4);
}
