class C { }
var global = (new owned C() : borrowed C?).borrow();
iter I(): borrowed C  { yield global; }
for i in I() do;
