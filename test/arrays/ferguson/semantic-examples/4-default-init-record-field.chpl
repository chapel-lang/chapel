use myrecord; 
record RecordStoringArray{
  var field:[1..2] R;
}
proc f() {
  return new RecordStoringArray();
}

proc run() {
  var x = f();
}

run();

