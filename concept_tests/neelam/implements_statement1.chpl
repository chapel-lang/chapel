interface LessThan {
  proc LT(x:self,y:self):bool;
}

int implements LessThan {
  proc LT(x:int, y:int):bool {
    return x < y;
  }
}

proc min(type T, x:T, y:T): int where T implements LessThan {
  if(LT(x,y))
	return x;
  else
	return y;
}

min(int,3,4);
