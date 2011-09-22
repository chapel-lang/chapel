// this code should generate "deprecated" warnings during proc/iter transition

def c11a() { writeln("in c11a"); }

def c11b() { writeln("in c11b"); return; }

def c11c() { return "11c"; }

def c12a() { yield "12a"; }

def c12b() { yield "12b"; return; }

def c13() {
  yield "13a.";
  return "13b.";
}

extern def c14();

c11a();
c11b();
writeln(c11c());
for i in c12a() do write(i); writeln();
for i in c12b() do write(i); writeln();
for i in c13() do write(i); writeln();
