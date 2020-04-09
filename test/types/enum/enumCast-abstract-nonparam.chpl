enum color {red, green, blue};

config const c: color;
config param enumToInt = true;

if enumToInt {
  var x = c: int;
} else {
  var c2 = 1: color;
}
