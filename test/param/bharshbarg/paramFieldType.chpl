record Data {
  type pt;
  param p: pt;
}

record DataArray {
  type pt;
  param p: pt;
  
  var data: [1..10] Data(pt, p);
}

proc print(type I, type T) {
  if I != T.p.type then compilerWarning("expecting '" + I:string + "', got '" + T.p.type:string + "'");
}

proc dosize(param w : int) {
  print(int(w), Data(int(w), 42));
  print(uint(w), Data(uint(w), 42));

  if w == 32 || w == 64 {
    print(real(w), Data(real(w), 42.42));
  }

  var temp : DataArray(int(w), 42);
  print(int(w), temp.data[1].type);

  var utemp : DataArray(uint(w), 42);
  print(uint(w), utemp.data[1].type);
}

proc main() {
  for param i in 3..6 {
    dosize(1 << i);
  }
}
