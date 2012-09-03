config param doit = false;
if doit {

  record R {
    var r: int;
  }

  on rootLocale.getLocales()(0) do {
    local {
      var r:R;
      r.r = 5;
    }
  }
}

writeln(doit);
