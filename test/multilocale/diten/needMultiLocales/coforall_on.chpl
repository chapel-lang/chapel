record R {
  var a, b, c: single bool;
  var s: string;
}

proc main() {
  var r: R;
  coforall i in 0..numLocales-1 with (ref r) {
    on Locales(i) {
      select i {
        when 0 {
          r.a;
          r.s = "Hello from locale " + here.id:string;
        }
        when 1 {
          r.b;
          writeln(here.id);
          r.c = true;
        }
        when 2 {
          writeln(here.id);
          r.b = true;
        }
        when 3 {
          r.c;
          writeln(here.id);
          r.a = true;
        }
        otherwise {
        }
      }
    }
  }
  writeln(r.s);
}

