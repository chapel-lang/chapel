proc id(i:int):int {
  return i;
}

interface F {
  proc id(i:self):self;
}

int implements F;
