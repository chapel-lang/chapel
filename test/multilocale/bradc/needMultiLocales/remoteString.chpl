var s1: string;
var flag1: sync bool;
var done: sync bool;

extern proc printf(fmt:c_string, x...);

proc main() {
  s1 = "foo";
  on Locales(1) do begin with (ref s1) {
    const tmp = flag1.readFE();
    printf("%s\n", ("s1 is: " + s1).c_str());
    done.writeEF(true);
  }
  s1 = "boohoo";
  flag1.writeEF(true);
  done.readFE();

  var s2: string;
  var flag2: sync bool;

  sync {
    s2 = "foo";
    on Locales(1) do begin with (ref s2) {
        const tmp = flag2.readFE();
        printf("%s\n", ("s2 is: " + s2).c_str());
      }
    s2 = "boohoo";
    flag2.writeEF(true);
  }
}
