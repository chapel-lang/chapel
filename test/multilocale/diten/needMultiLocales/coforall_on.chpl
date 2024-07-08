record R {
  var a, b, c: sync bool;
  var s: string;
  proc init() {}
  proc init=(other: R) {
    this.s = other.s;
    init this;

    if other.a.isFull
      then this.a.writeEF(other.a.readFF());
    if other.b.isFull
      then this.b.writeEF(other.b.readFF());
    if other.c.isFull
      then this.c.writeEF(other.c.readFF());
  }
}

proc main() {
  var r: R;
  coforall i in 0..numLocales-1 with (ref r) {
    on Locales(i) {
      select i {
        when 0 {
          r.a.readFF();
          r.s = "Hello from locale " + here.id:string;
        }
        when 1 {
          r.b.readFF();
          writeln(here.id);
          r.c.writeEF(true);
        }
        when 2 {
          writeln(here.id);
          r.b.writeEF(true);
        }
        when 3 {
          r.c.readFF();
          writeln(here.id);
          r.a.writeEF(true);
        }
        otherwise {
        }
      }
    }
  }
  writeln(r.s);
}

