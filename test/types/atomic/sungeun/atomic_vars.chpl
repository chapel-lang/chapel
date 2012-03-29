config type myType = uint(8);

var ax: atomic myType;
var x = ax.read();

writeln(x);
writeln(numBits(x.type));

on Locales[numLocales-1] do ax.write(min(myType));
writeln(ax.read());

ax.exchange(max(myType));
writeln(ax.read());

ax.write(0:myType);
coforall i in 1..22 do on Locales[i%numLocales] { // 22 is max for uint(8)
  ax.fetchAdd(i:myType);
}
x = ax.read();
if x != 253:myType then
  writeln("Error with fetchAdd(): ", x);
else
  writeln(x);

const D = [7..39];
var aA: [D] atomic myType;

for i in D do aA[i].write(i:myType);
var A = [a in aA] a.read();
writeln(A);

for i in D do aA[i].write((i+1):myType);
A = aA.read();
writeln(A);

