class C { var x: int; }

var x: sync owned C?;

x.writeEF(new owned C?(1));

x.writeFF(new owned C?(2));
