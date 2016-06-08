use checkType;

const sl = "bye";
const s: string;

// s should be string for these
{
  proc f(s = "hi") {
    checkType(s.type);
  }

  f();
  f("bye");
  f("bye":string);
  f(s="bye");
  f(s="bye":string);
  f(sl);
  f(s=sl);
  f(s);
  f(s=s);
}

{
  proc f(type gtype, g, s = "hi") {
    checkType(gtype, g.type);
    checkType(s.type);
  }

  f(int, 1);
  f(int, 3, "bye");
  f(int, 7, "bye":string);
  f(s="bye", int, 5);
  f(s="bye":string, int, 9);
  f(int, 11, sl);
  f(s=sl, int, 13);
  f(int, 15, s);
  f(s=s, int, 17);
}

{
  proc f(s = "hi", type gtype, g = 3.14) {
    checkType(gtype, g.type);
    checkType(s.type);
  }

  f(gtype=real);
  f("bye", real);
  f("bye":string, real);
  f(real, s="bye");
  f(real, s="bye":string);
  f(sl, real);
  f(real, s=sl);
  f(s, real);
  f(real, s=s);
}

{
  proc f(param s = "hi") {
    checkType(string, s.type);
  }

  f();
  f("bye");
  f(s="bye");
}

{
  proc f(type gtype, g, param s = "hi") {
    checkType(gtype, g.type);
    checkType(string, s.type);
  }

  f(int, 1);
  f(int, 3, "bye");
  f(s="bye", int, 5);
}

{
  proc f(param s = "hi", type gtype, g = 3.14) {
    checkType(gtype, g.type);
    checkType(string, s.type);
  }

  f(gtype=real);
  f("bye", real);
  f(real, s="bye");
}

// s should be c_string for these
{
  proc f(s: c_string = "hi") {
    checkType(c_string, s.type);
  }

  f();
  f("bye");
  f(s="bye");
}

{
  proc f(type gtype, g, s: c_string = "hi") {
    checkType(gtype, g.type);
    checkType(c_string, s.type);
  }

  f(int, 1);
  f(int, 3, "bye");
  f(s="bye", int, 5);
}

{
  proc f(s: c_string = "hi", type gtype, g = 3.14) {
    checkType(gtype, g.type);
    checkType(c_string, s.type);
  }

  f(gtype=real);
  f("bye", real);
  f(real, s="bye");
}
