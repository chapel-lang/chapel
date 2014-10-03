// Check some compile-time computations and aliasing.

TEST_CLASS(true);
TEST_CLASS(555);
TEST_CLASS(int);
TEST_CLASS([1..3] string);
TEST_RECORD(true);
TEST_RECORD(555);
TEST_RECORD(int);
TEST_RECORD([1..3] string);
compilerError("done"); // no need to execute

/////////////////////////////////

class CLASS_PARAM {
  param FIELD_PARAM;
  proc METHOD_TYPE type return complex;
}

proc TEST_CLASS(param ARG_PARAM) {
  compilerWarning("TEST_CLASS(param)");
  compilerWarning(ARG_PARAM: c_string);

  param LOC_ALIAS_PARAM = ARG_PARAM;
  compilerWarning(LOC_ALIAS_PARAM: c_string);

/*  this does not work - see param-formal-in-ctor.chpl
  type LOC_TYPE = CLASS_PARAM(ARG_PARAM);
  compilerWarning(typeToString(LOC_TYPE));
*/
  var LOC_VAR1: CLASS_PARAM(ARG_PARAM);
  compilerWarning(typeToString(LOC_VAR1.type));
  compilerWarning(LOC_VAR1.FIELD_PARAM: c_string);
  compilerWarning(typeToString(LOC_VAR1.METHOD_TYPE));
  param LOC_ALIAS_PARAM1 = LOC_VAR1.FIELD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM1: c_string);
  type LOC_ALIAS_TYPE1 = LOC_VAR1.METHOD_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE1));
/*
  var LOC_VAR2: LOC_TYPE;
  compilerWarning(typeToString(LOC_VAR2.type));
  compilerWarning(LOC_VAR2.FIELD_PARAM: c_string);
  compilerWarning(typeToString(LOC_VAR2.METHOD_TYPE));
  param LOC_ALIAS_PARAM2 = LOC_VAR2.FIELD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM2: c_string);
  type LOC_ALIAS_TYPE2 = LOC_VAR2.METHOD_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE2));
*/
}

/////////////////////////////////

record RECORD_PARAM {
  param FIELD_PARAM;
  proc METHOD_TYPE type return complex;
}

proc TEST_RECORD(param ARG_PARAM) {
  compilerWarning("TEST_RECORD(param)");
  compilerWarning(ARG_PARAM: c_string);

  param LOC_ALIAS_PARAM = ARG_PARAM;
  compilerWarning(LOC_ALIAS_PARAM: c_string);

/*  this does not work - see param-formal-in-ctor.chpl
  type LOC_TYPE = RECORD_PARAM(ARG_PARAM);
  compilerWarning(typeToString(LOC_TYPE));
*/
  var LOC_VAR1: RECORD_PARAM(ARG_PARAM);
  compilerWarning(typeToString(LOC_VAR1.type));
  compilerWarning(LOC_VAR1.FIELD_PARAM: c_string);
  compilerWarning(typeToString(LOC_VAR1.METHOD_TYPE));
  param LOC_ALIAS_PARAM1 = LOC_VAR1.FIELD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM1: c_string);
  type LOC_ALIAS_TYPE1 = LOC_VAR1.METHOD_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE1));
/*
  var LOC_VAR2: LOC_TYPE;
  compilerWarning(typeToString(LOC_VAR2.type));
  compilerWarning(LOC_VAR2.FIELD_PARAM: c_string);
  compilerWarning(typeToString(LOC_VAR2.METHOD_TYPE));
  param LOC_ALIAS_PARAM2 = LOC_VAR2.FIELD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM2: c_string);
  type LOC_ALIAS_TYPE2 = LOC_VAR2.METHOD_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE2));
*/
}

/////////////////////////////////

class CLASS_TYPE {
  type FIELD_TYPE;
  proc METHOD_PARAM param return 666;
}

proc TEST_CLASS(type ARG_TYPE) {
  compilerWarning("TEST_CLASS(type)");
  compilerWarning(typeToString(ARG_TYPE));

  type LOC_ALIAS_TYPE = ARG_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE));

  type LOC_TYPE = CLASS_TYPE(ARG_TYPE);
  compilerWarning(typeToString(LOC_TYPE));

  var LOC_VAR1: CLASS_TYPE(ARG_TYPE);
  compilerWarning(typeToString(LOC_VAR1.type));
  compilerWarning(typeToString(LOC_VAR1.FIELD_TYPE));
  compilerWarning(LOC_VAR1.METHOD_PARAM: c_string);
  type LOC_ALIAS_TYPE1 = LOC_VAR1.FIELD_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE1));
  param LOC_ALIAS_PARAM1 = LOC_VAR1.METHOD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM1: c_string);

  var LOC_VAR2: LOC_TYPE;
  compilerWarning(typeToString(LOC_VAR2.type));
  compilerWarning(typeToString(LOC_VAR2.FIELD_TYPE));
  compilerWarning(LOC_VAR2.METHOD_PARAM: c_string);
  type LOC_ALIAS_TYPE2 = LOC_VAR2.FIELD_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE2));
  param LOC_ALIAS_PARAM2 = LOC_VAR2.METHOD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM2: c_string);
}

/////////////////////////////////

class RECORD_TYPE {
  type FIELD_TYPE;
  proc METHOD_PARAM param return 666;
}

proc TEST_RECORD(type ARG_TYPE) {
  compilerWarning("TEST_RECORD(type)");
  compilerWarning(typeToString(ARG_TYPE));

  type LOC_ALIAS_TYPE = ARG_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE));

  type LOC_TYPE = RECORD_TYPE(ARG_TYPE);
  compilerWarning(typeToString(LOC_TYPE));

  var LOC_VAR1: RECORD_TYPE(ARG_TYPE);
  compilerWarning(typeToString(LOC_VAR1.type));
  compilerWarning(typeToString(LOC_VAR1.FIELD_TYPE));
  compilerWarning(LOC_VAR1.METHOD_PARAM: c_string);
  type LOC_ALIAS_TYPE1 = LOC_VAR1.FIELD_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE1));
  param LOC_ALIAS_PARAM1 = LOC_VAR1.METHOD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM1: c_string);

  var LOC_VAR2: LOC_TYPE;
  compilerWarning(typeToString(LOC_VAR2.type));
  compilerWarning(typeToString(LOC_VAR2.FIELD_TYPE));
  compilerWarning(LOC_VAR2.METHOD_PARAM: c_string);
  type LOC_ALIAS_TYPE2 = LOC_VAR2.FIELD_TYPE;
  compilerWarning(typeToString(LOC_ALIAS_TYPE2));
  param LOC_ALIAS_PARAM2 = LOC_VAR2.METHOD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM2: c_string);
}
