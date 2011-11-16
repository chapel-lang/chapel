interface F {
  proc id(i:int):int;
}

proc id(i:int):int {
  return i;
}

int implements F;
