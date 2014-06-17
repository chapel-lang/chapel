record EndPoints {
  var start : int;
  var end : int;
}

constructor EndPoints(s, e) {
    start = s;
    end = e;
}

var x = new EndPoints(1,2);
writeln("expect 3 ", x.start + x.end);
