param a = 0;
param b = -1;
param c = 2;

param d = a ** c;
param e = c ** b;
param f = b ** c;
param g = a ** b; // compiler error

writeln((d,e,f,g));
