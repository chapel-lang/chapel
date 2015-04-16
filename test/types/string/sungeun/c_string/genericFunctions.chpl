use checkType;

param psl = "bye";
const cs: string;
var vs: string;

// s should be string for these
{
  proc g(s) {
    checkType(s.type);
  }

  g("bye");
  g("bye":string);
  g(s="bye");
  g(s="bye":string);
  g(psl);
  g(cs);
  g(vs);
  g(s=psl);
  g(s=cs);
  g(s=vs);
}

{
  proc f(type gtype, g, s) {
    checkType(gtype, g.type);
    checkType(s.type);
  }

  f(int, 3, "bye");
  f(int, 7, "bye":string);
  f(s="bye", int, 5);
  f(s="bye":string, int, 9);
  f(int, 11, psl);
  f(int, 15, cs);
  f(int, 11, vs);
  f(s=psl, int, 13);
  f(s=cs, int, 17);
  f(s=vs, int, 13);
}

{
  proc f(s, type gtype, g = 3.14) {
    checkType(gtype, g.type);
    checkType(s.type);
  }

  f("bye", real);
  f("bye":string, real);
  f(real, s="bye");
  f(real, s="bye":string);
  f(psl, real);
  f(cs, real);
  f(vs, real);
  f(real, s=psl);
  f(real, s=cs);
  f(real, s=vs);
}

// s should be c_string for these
{
  proc g(s) {
    checkType(c_string, s.type);
  }

  g(vs.c_str());
  g(cs.c_str());
  g(s=vs.c_str());
  g(s=cs.c_str());
}

{
  proc f(type gtype, g, s) {
    checkType(gtype, g.type);
    checkType(c_string, s.type);
  }

  f(int, 3, vs.c_str());
  f(int, 3, cs.c_str());
  f(s=vs.c_str(), int, 5);
  f(s=cs.c_str(), int, 5);
}

{
  proc f(s, type gtype, g = 3.14) {
    checkType(gtype, g.type);
    checkType(c_string, s.type);
  }

  f(vs.c_str(), real);
  f(cs.c_str(), real);
  f(real, s=vs.c_str());
  f(real, s=cs.c_str());
}

{
  proc g(param s) {
    checkType(c_string, s.type);
  }

  g("bye");
  g(s="bye");
}

{
  proc f(type gtype, g, param s) {
    checkType(gtype, g.type);
    checkType(c_string, s.type);
  }

  f(int, 3, "bye");
  f(s="bye", int, 5);
}

{
  proc f(param s, type gtype, g = 3.14) {
    checkType(gtype, g.type);
    checkType(c_string, s.type);
  }

  f("bye", real);
  f(real, s="bye");
}

