class C { }
var global = new borrowed C() : borrowed C?;
iter I(): borrowed C  { yield global; }
for i in I() do;
