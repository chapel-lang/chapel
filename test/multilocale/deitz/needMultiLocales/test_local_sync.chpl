extern proc printf(fmt: c_string, x...);

proc main {
  var s: sync int;

  s = 2;

  on Locales(1) {
    var i = s;
    printf("%s\n", (here.id:string + " " + i:string).c_str());
  }

  s = 3;

  on Locales(1) {
    var i = s;
    printf("%s\n", (here.id:string + " " + i:string).c_str());
  }
}
