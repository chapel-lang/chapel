union u {
  var x: int;
  var y: int;
  var z: real;
}

var u0, u1, u2, u3, u4: u;

u2.x = 45;
u3.y = 78;
u4.z = 33.3;

select u0 {
  when u2 do writeln("Matches u2!");
  when u3 do writeln("Matches u3!");
  when u4 do writeln("Matches u4!");
  otherwise do writeln("Got: ", u0);
}

testit();

u0.x = 33;
testit();

u0.x = 45;
testit();

u0.y = 33;
testit();

u0.y = 78;
testit();

u0.z = 33.0;
testit();

u0.z = 33.3;
testit();

proc testit() {
  select u0 {
    when u1 do writeln("Matches u1!");
    when u2 do writeln("Matches u2!");
    when u3 do writeln("Matches u3!");
    when u4 do writeln("Matches u4!");
    otherwise do writeln("Got: ", u0);
  }
}
