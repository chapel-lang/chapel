config var n : int = 4;

var d1: domain(1) = {1..n};
writeln("domain(1): \t\t\t", d1);
writeln("translate(2): \t\t\t", d1.translate(2));
writeln("translate(0): \t\t\t", d1.translate(0));                      
writeln("translate(-1): \t\t\t", d1.translate(-1));
writeln();

var d2 : domain(2) = {1..n, 1..n};
writeln("domain(2): \t\t\t", d2);
writeln("translate(1, 2): \t\t", d2.translate(1, 2));
writeln("translate(-1, -2): \t\t", d2.translate(-1, -2));
writeln("translate(0, 0): \t\t", d2.translate(0, 0));
writeln("translate(0, 3): \t\t", d2.translate(0, 3));
writeln("translate(0, -1): \t\t", d2.translate(0, -1));
writeln("translate(4, 0): \t\t", d2.translate(4, 0));
writeln("translate(-4, 0): \t\t", d2.translate(-4, 0));
writeln("translate(-5, 1): \t\t", d2.translate(-5, 1));
writeln("translate(1, -5): \t\t", d2.translate(1, -5));
writeln();

var d3 : domain(3) = {1..n, 1..n, 1..n};
writeln("domain(3): \t\t\t", d3);
writeln("translate(1, 2, 3): \t\t", d3.translate(1, 2, 3));
writeln("translate(-1, -2, -3): \t\t", d3.translate(-1, -2, -3));
writeln("translate(0, 0, 0): \t\t", d3.translate(0, 0, 0));
writeln("translate(0, 2, 0): \t\t", d3.translate(0, 2, 0));
writeln("translate(1, 0, 0): \t\t", d3.translate(1, 0, 0));
writeln("translate(0, 0, -1): \t\t", d3.translate(0, 0, -1));
writeln("translate(-1, 0, 1): \t\t", d3.translate(-1, 0, 1));
writeln("translate(4, -3, 2): \t\t", d3.translate(4, -3, 2));
