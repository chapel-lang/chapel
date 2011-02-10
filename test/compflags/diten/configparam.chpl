enum color {red, green, blue};

config param abool1 = false;
config param abool2: bool = false;
config param aint1 = 3;
config param aint2: int = 3;
config param areal1 = 3.14;
config param areal2: real = 3.14;
config param astring1 = "hi";
config param astring2: string = "hi";
//config param acomplex1 = 3.14+2.72i;
//config param acomplex2: complex = 3.14+2.72i;
config param aimag1 = 3.4i;
config param aimag2: imag = 3.4i;
config param acolor1 = color.blue;
config param acolor2: color = color.blue;

proc main() {
  writeln(abool1);
  writeln(abool2);

  writeln(aint1);
  writeln(aint2);

  writeln(areal1);
  writeln(areal2);

  writeln(astring1);
  writeln(astring2);

  //writeln(acomplex1);
  //writeln(acomplex2);

  writeln(aimag1);
  writeln(aimag2);

  writeln(acolor1);
  writeln(acolor2);
}

