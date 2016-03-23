record RR {
  var D = {1..3};
  var A: [D] int;
}

record R {
  var DD = {1..3};
//var B: [1..3] RR;
}

var r: R;
writeln(r);
