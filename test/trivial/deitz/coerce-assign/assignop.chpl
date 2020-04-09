var x = 0b0100;
x &= 0b1100;
writeln(x);
x |= 0b0011;
writeln(x);
x ^= 0b0110;
writeln(x);

x = 12;
x /= 3;
writeln(x);
x **= 2;
writeln(x);
x -= 8;
writeln(x);
x += 16;
writeln(x);
x %= 20;
writeln(x);
x *= 5;
writeln(x);
x <<= 1;
writeln(x);
x >>= 2;
writeln(x);

var y = true;

y &&= false;
writeln(y);
y ||= true;
writeln(y);

