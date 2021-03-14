use Reflection;

record R {
  var x:int;
}

var r:R;
var i = getField(r, 1);

