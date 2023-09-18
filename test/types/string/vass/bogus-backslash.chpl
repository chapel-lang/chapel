
var x: owned RootClass?;
param p1 = x.type:string;
param p2 = "owned RootClass?";
compilerWarning(p1);
compilerWarning(p2);
compilerError((p1 == p2) :string);  // should print 'true'
