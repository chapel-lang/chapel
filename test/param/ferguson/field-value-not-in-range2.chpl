use Reflection;

record R {
  var x:int;
}

var r:R;
var i = getFieldRef(r, 1);

