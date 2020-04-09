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
  compilerWarning(ARG_PARAM: string);

  param LOC_ALIAS_PARAM = ARG_PARAM;
  compilerWarning(LOC_ALIAS_PARAM: string);

  type LOC_TYPE = CLASS_PARAM(ARG_PARAM);
  compilerWarning(LOC_TYPE:string);

  var LOC_VAR1: borrowed CLASS_PARAM(ARG_PARAM) =
    new borrowed CLASS_PARAM(ARG_PARAM);
  compilerWarning(LOC_VAR1.type:string);
  compilerWarning(LOC_VAR1.FIELD_PARAM: string);
  compilerWarning(LOC_VAR1.METHOD_TYPE:string);
  param LOC_ALIAS_PARAM1 = LOC_VAR1.FIELD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM1: string);
  type LOC_ALIAS_TYPE1 = LOC_VAR1.METHOD_TYPE;
  compilerWarning(LOC_ALIAS_TYPE1:string);

/*
  var LOC_VAR2: LOC_TYPE;
  compilerWarning(LOC_VAR2.type:string);
  compilerWarning(LOC_VAR2.FIELD_PARAM: string);
  compilerWarning(LOC_VAR2.METHOD_TYPE:string);
  param LOC_ALIAS_PARAM2 = LOC_VAR2.FIELD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM2: string);
  type LOC_ALIAS_TYPE2 = LOC_VAR2.METHOD_TYPE;
  compilerWarning(LOC_ALIAS_TYPE2:string);
*/
}

/////////////////////////////////

record RECORD_PARAM {
  param FIELD_PARAM;
  proc METHOD_TYPE type return complex;
}

proc TEST_RECORD(param ARG_PARAM) {
  compilerWarning("TEST_RECORD(param)");
  compilerWarning(ARG_PARAM: string);

  param LOC_ALIAS_PARAM = ARG_PARAM;
  compilerWarning(LOC_ALIAS_PARAM: string);

/*  this does not work - see param-formal-in-ctor.chpl
  type LOC_TYPE = RECORD_PARAM(ARG_PARAM);
  compilerWarning(LOC_TYPE:string);
*/
  var LOC_VAR1: RECORD_PARAM(ARG_PARAM);
  compilerWarning(LOC_VAR1.type:string);
  compilerWarning(LOC_VAR1.FIELD_PARAM: string);
  compilerWarning(LOC_VAR1.METHOD_TYPE:string);
  param LOC_ALIAS_PARAM1 = LOC_VAR1.FIELD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM1: string);
  type LOC_ALIAS_TYPE1 = LOC_VAR1.METHOD_TYPE;
  compilerWarning(LOC_ALIAS_TYPE1:string);
/*
  var LOC_VAR2: LOC_TYPE;
  compilerWarning(LOC_VAR2.type:string);
  compilerWarning(LOC_VAR2.FIELD_PARAM: string);
  compilerWarning(LOC_VAR2.METHOD_TYPE:string);
  param LOC_ALIAS_PARAM2 = LOC_VAR2.FIELD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM2: string);
  type LOC_ALIAS_TYPE2 = LOC_VAR2.METHOD_TYPE;
  compilerWarning(LOC_ALIAS_TYPE2:string);
*/
}

/////////////////////////////////

class CLASS_TYPE {
  type FIELD_TYPE;
  proc METHOD_PARAM param return 666;
}

proc TEST_CLASS(type ARG_TYPE) {
  compilerWarning("TEST_CLASS(type)");
  compilerWarning(ARG_TYPE:string);

  type LOC_ALIAS_TYPE = ARG_TYPE;
  compilerWarning(LOC_ALIAS_TYPE:string);

  type LOC_TYPE = CLASS_TYPE(ARG_TYPE);
  compilerWarning(LOC_TYPE:string);

  var LOC_VAR1: borrowed CLASS_TYPE(ARG_TYPE) =
    new borrowed CLASS_TYPE(ARG_TYPE);
  compilerWarning(LOC_VAR1.type:string);
  compilerWarning(LOC_VAR1.FIELD_TYPE:string);
  compilerWarning(LOC_VAR1.METHOD_PARAM: string);
  type LOC_ALIAS_TYPE1 = LOC_VAR1.FIELD_TYPE;
  compilerWarning(LOC_ALIAS_TYPE1:string);
  param LOC_ALIAS_PARAM1 = LOC_VAR1.METHOD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM1: string);

  var LOC_VAR2: borrowed LOC_TYPE = new borrowed LOC_TYPE();
  compilerWarning(LOC_VAR2.type:string);
  compilerWarning(LOC_VAR2.FIELD_TYPE:string);
  compilerWarning(LOC_VAR2.METHOD_PARAM: string);
  type LOC_ALIAS_TYPE2 = LOC_VAR2.FIELD_TYPE;
  compilerWarning(LOC_ALIAS_TYPE2:string);
  param LOC_ALIAS_PARAM2 = LOC_VAR2.METHOD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM2: string);
}

/////////////////////////////////

class RECORD_TYPE {
  type FIELD_TYPE;
  proc METHOD_PARAM param return 666;
}

proc TEST_RECORD(type ARG_TYPE) {
  compilerWarning("TEST_RECORD(type)");
  compilerWarning(ARG_TYPE:string);

  type LOC_ALIAS_TYPE = ARG_TYPE;
  compilerWarning(LOC_ALIAS_TYPE:string);

  type LOC_TYPE = RECORD_TYPE(ARG_TYPE);
  compilerWarning(LOC_TYPE:string);

  var LOC_VAR1: borrowed RECORD_TYPE(ARG_TYPE) =
    new borrowed RECORD_TYPE(ARG_TYPE);
  compilerWarning(LOC_VAR1.type:string);
  compilerWarning(LOC_VAR1.FIELD_TYPE:string);
  compilerWarning(LOC_VAR1.METHOD_PARAM: string);
  type LOC_ALIAS_TYPE1 = LOC_VAR1.FIELD_TYPE;
  compilerWarning(LOC_ALIAS_TYPE1:string);
  param LOC_ALIAS_PARAM1 = LOC_VAR1.METHOD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM1: string);

  var LOC_VAR2: borrowed LOC_TYPE = new borrowed LOC_TYPE();
  compilerWarning(LOC_VAR2.type:string);
  compilerWarning(LOC_VAR2.FIELD_TYPE:string);
  compilerWarning(LOC_VAR2.METHOD_PARAM: string);
  type LOC_ALIAS_TYPE2 = LOC_VAR2.FIELD_TYPE;
  compilerWarning(LOC_ALIAS_TYPE2:string);
  param LOC_ALIAS_PARAM2 = LOC_VAR2.METHOD_PARAM;
  compilerWarning(LOC_ALIAS_PARAM2: string);
}
