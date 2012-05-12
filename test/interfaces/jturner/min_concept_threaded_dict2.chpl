interface LessThan {
	proc LT(x:self, y:self):bool;
}

proc LT(x:int, y:int):bool {
	return x<y;
}

int implements LessThan;

proc DoLT(x:?T, y:T):T where T implements LessThan {  
  if(LT(x,y)) {
    return x;
  }
  else {
    return y;
  }
}

proc minFn(x:?T,y :T):T where T implements LessThan {
  return DoLT(x, y);
}

writeln(minFn(8,1));
