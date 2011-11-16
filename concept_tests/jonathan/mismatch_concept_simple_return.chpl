proc id(i:int):int {
  return i;
}

interface F {
  proc id(i:int):string;
}

int implements F;
