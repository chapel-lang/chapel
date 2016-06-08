use decls;

for param i in 1..4 do writeln("8":uint(4<<i));
for param i in 1..4 do writeln("8":int(4<<i));
writeln("8":real(32));
writeln("8":real(64));
writeln("8i":imag(32));
writeln("8i":imag(64));
writeln("8+8i":complex(64));
writeln("8+8i":complex(128));
writeln("three":E);
writeln("true":bool);

for param i in 1..4 do writeln(cstr:uint(4<<i));
for param i in 1..4 do writeln(cstr:int(4<<i));
writeln(cstr:real(32));
writeln(cstr:real(64));
writeln(cstri:imag(32));
writeln(cstri:imag(64));
writeln(cstrc:complex(64));
writeln(cstrc:complex(128));
writeln(cstrE:E);
writeln(cstrB:bool);

for param i in 1..4 do writeln(vcstr:uint(4<<i));
for param i in 1..4 do writeln(vcstr:int(4<<i));
writeln(vcstr:real(32));
writeln(vcstr:real(64));
writeln(vcstri:imag(32));
writeln(vcstri:imag(64));
writeln(vcstrc:complex(64));
writeln(vcstrc:complex(128));
writeln(vcstrE:E);
writeln(vcstrB:bool);

for param i in 1..4 do writeln(str:uint(4<<i));
for param i in 1..4 do writeln(str:int(4<<i));
writeln(str:real(32));
writeln(str:real(64));
writeln(stri:imag(32));
writeln(stri:imag(64));
writeln(strc:complex(64));
writeln(strc:complex(128));
writeln(strE:E);
writeln(strB:bool);

writeln(n:string);
writeln(r:string);
writeln(i:string);
writeln(c:string);
writeln(e:string);
writeln(b:string);
