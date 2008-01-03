record R {
  var a, b, c: bool;
  var s: string;
}

def main() {
  var r: R;
  coforall i in 0..numLocales-1 {
    on Locales(i) {
      select i {
        when 0 {
          while (!r.a) {}
          r.s = "Hello from locale " + localeID();
        }
        when 1 {
          while (!r.b) {}
          _debugWriteln(localeID());
          r.c = true;
        }
        when 2 {
          _debugWriteln(localeID());
          r.b = true;
        }
        when 3 {
          while (!r.c) {}
          _debugWriteln(localeID());
          r.a = true;
        }
        otherwise {
        }
      }
    }
  }
  writeln(r.s);
}

