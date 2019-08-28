class C { }
var global = new owned C() : owned C?;
iter I(): owned C  { yield global; }
for i in I() do;
