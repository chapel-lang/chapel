use Set;

class C { var x = 0; }

config const numElems = 2**20;

proc test1() {
  writeln('T1');
  writeln('Adding ' + numElems:string + ' elems');
  var st = new set(shared C);
  for i in 0..<numElems do st.add(new shared C(i));
  writeln(st.size);
}
test1();
  
