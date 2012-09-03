iter ii() {
  local {
    local {
      for i in 1..10 {
        local {
          yield i;
        }
      }
    }
  }
}

class C {
  var i: int = -1;
}

var c = new C();

on rootLocale.getLocales()(1) {
  for i in ii() {
    writeln(c.i);
  }
}
