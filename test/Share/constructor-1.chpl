record EndPoints {
  var start : integer;
  var end : integer;
}

constructor EndPoints(s, e) {
    start = s;
    end = e;
}

var x = EndPoints(1,2);
writeln("expect 3 ", x.start + x.end);
