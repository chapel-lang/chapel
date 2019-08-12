enum color {red, green, blue};

const s = " hi ";
const i = 1;
const u = 0: uint;
const r = 1.2;
const im = 3.4i;
const cmp = 5.6 + 7.8i;
const e = color.green;
const b = true;

writeln(s + i);
writeln(s + u);
writeln(s + r);
writeln(s + im);
writeln(s + cmp);
writeln(s + e);
writeln(s + b);

writeln(i + s);
writeln(u + s);
writeln(r + s);
writeln(im + s);
writeln(cmp + s);
writeln(e + s);
writeln(b + s);

writeln(" hi " + 1);
writeln(" hi " + 0:uint);
writeln(" hi " + color.green);
writeln(" hi " + true);

writeln(1 + " hi ");
writeln(0:uint + " hi ");
writeln(color.green + " hi ");
writeln(true + " hi ");

