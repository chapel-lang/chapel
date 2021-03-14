use Reflection;

record R {
  var x:int;
}

var r:R;
var s = getFieldRef(r, "y");

