proc fact(i : int) :int{
  if(i == 0) then halt();
  else return i*fact(i-1);
}

fact(2);
