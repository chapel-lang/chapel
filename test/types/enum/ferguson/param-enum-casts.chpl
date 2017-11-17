enum myenum { x=1, y=2 };

param i1 = myenum.x:int;
param x1 = 1:myenum;
param x2 = "x":myenum;
param y1 = 2:myenum;
param y2 = "y":myenum;


writeln(i1.type:string, " ", i1);
writeln(x1.type:string, " ", x1);
writeln(x2.type:string, " ", x2);
writeln(y1.type:string, " ", y1);
writeln(y2.type:string, " ", y2);
