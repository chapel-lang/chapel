record T {
  param paramField;
  param otherParamField;
}

type T1 = T(paramField=1, ?);
var x: T1(paramField=2, otherParamField=5); // Should complain
