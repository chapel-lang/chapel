class C {
  var x: int;
}

proc doSomethingOnLocale(loc: locale, in thunk: _thunkRecord) {
  var temp: thunkToReturnType(thunk.type);
  on loc {
    var result = __primitive("force thunk", thunk);
    temp = result;
  }
  return temp;
}

var result = doSomethingOnLocale(Locales.last, __primitive("create thunk", new C?(42)));
writeln(result);
writeln(result.locale == Locales.last);
