record Point {
    var x: real;
      var y: real;
}

proc =(ref arr: [?d], ref r: Point) {
    arr[d.first] = r.x;
      arr[d.first+1] = r.y;
}

var arr: [0..1] real;
var p = new Point(10,20);

//arr[0] = 0;  // uncomment this and it works

arr = p;

writeln(arr);
