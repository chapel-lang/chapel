use Reflection;

config type testType = string;
config param testNum = 1;


if testNum == 1 {
  getNumFields(testType);
}
else if testNum == 2 {
  getFieldName(testType, 0);
}
else if testNum == 3 {
  var v: testType;
  getField(v, 0);
}
else if testNum == 4 {
  var v: testType;
  getField(v, "dummy");
}
else if testNum == 5 {
  var v: testType;
  getFieldRef(v, 0);
}
else if testNum == 6 {
  var v: testType;
  getFieldRef(v, "dummy");
}
else if testNum == 7 {
  getFieldIndex(testType, "dummy");
}
else if testNum == 8 {
  hasField(testType, "dummy");
}
else if testNum == 9 {
  isFieldBound(testType, 0);
}
else if testNum == 10 {
  isFieldBound(testType, "dummy");
}
