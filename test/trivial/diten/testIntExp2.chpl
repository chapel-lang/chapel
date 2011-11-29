param a = 0;
param b = 0;
param c = 1;
param d = 2;
param e = 8;

param f = a ** b;
param g = a ** c;
param h = e ** b;
param i = d ** e;

writeln((f,g,h,i)); // (1, 0, 1, 256)
