interface LessThan {
  proc LT(x:self,y:self):bool;
}

int implements LessThan {
  LT = MyLT;
}

proc MyLT( x: int, y:int):bool {
  return x < y;
}

proc min(type T, x:T, y:T): bool where T implements LessThan {
  if( LT(x,y))
    return x;
  else
    return y;
}

min(int,3,4);
