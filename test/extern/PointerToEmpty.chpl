use CTypes;
var a: [0..1] int;

var locDom: domain(1) = {2..1};

writeln(a[locDom]);          
writeln(c_ptrTo(a[locDom]));
