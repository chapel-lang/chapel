_extern record testrec {
  var x:int(64);
}

_extern proc init_testrec(inout t:testrec, x:int(32), y:int(32));
_extern proc debug_print_testrec(inout t:testrec);

record myrecord {
  var t:testrec;
}

record otherrecord {
  var i:int(64);
}

var r:myrecord;
init_testrec(r.t, 72, 24);
writeln("r is");
debug_print_testrec(r.t);

var copy:myrecord = r;

writeln("copy is");
debug_print_testrec(copy.t);
