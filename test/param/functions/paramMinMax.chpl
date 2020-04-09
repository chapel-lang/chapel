config type t = int;

param one = 1: t;
param two = 2: t;

param small = min(one,two);
param big = max(one,two);

compilerError("small = " + small:string + ", big = " + big: string);
