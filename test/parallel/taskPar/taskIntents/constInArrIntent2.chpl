config param modify = false;
var flag: atomic int, flag2: sync int;
proc main()
{
  var xs : [0 ..# 3] int = [1, 2, 3];
  begin modifyArr(xs);
  coforall loc in Locales with (const in xs) do on loc {
    flag.add(1);
    flag2.readFF();
    writeln(xs);
    if modify then
      xs += 1;
  }
}

proc modifyArr(arr) {
  flag.waitFor(numLocales);
  arr += 1;
  flag2.writeEF(2);
}
