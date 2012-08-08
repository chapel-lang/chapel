// This tests whether atomic variables get default values at declaration.

config const n = 10;
config const par = 1000;

for 1..n {

  var toti: atomic int;

  const toti1 = toti.read();
  if toti1 != 0 then writeln("error 1: toti=", toti1);

  forall 1..par {
    var ai32: atomic int(32);
    var au32: atomic uint(32);
    var ai64: atomic int(64);
    var au64: atomic uint(64);
    var booo: atomic bool;

    if ai32.read() != 0 then toti.add(1);
    if au32.read() != 0 then toti.add(1);
    if ai64.read() != 0 then toti.add(1);
    if au64.read() != 0 then toti.add(1);
    if booo.read() then toti.add(1);

    ai32.add(3);
    au32.add(4);
    ai64.add(5);
    au64.add(6);

    if ai32.read() != 3 then toti.add(1);
    if au32.read() != 4 then toti.add(1);
    if ai64.read() != 5 then toti.add(1);
    if au64.read() != 6 then toti.add(1);
  }  // forall

  const toti2 = toti.read();
  if toti2 != 0 then writeln("error 2: toti=", toti2);

}  // for 1..n
