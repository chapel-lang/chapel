class C { }
var global = new shared C() : shared C?;
iter I(): shared C  { yield global; }
for i in I() do;
