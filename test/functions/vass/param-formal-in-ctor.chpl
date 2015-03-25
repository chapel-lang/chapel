// Once this works, delete this test and uncomment
// the corresponding pieces in ./compile-time-messages.chpl


TEST_CLASS(true);
TEST_CLASS(555);

class CLASS_PARAM {
  param FIELD_PARAM;
}

proc TEST_CLASS(param ARG_PARAM) {
  type LOC_TYPE = CLASS_PARAM(ARG_PARAM);
  compilerWarning(typeToString(LOC_TYPE));
}


TEST_RECORD(true);
TEST_RECORD(555);

class RECORD_PARAM {
  param FIELD_PARAM;
}

proc TEST_RECORD(param ARG_PARAM) {
  type LOC_TYPE = RECORD_PARAM(ARG_PARAM);
  compilerWarning(typeToString(LOC_TYPE));
}

compilerError("done"); // no need to execute
