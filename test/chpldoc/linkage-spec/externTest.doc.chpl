module Other {
  /* Comment for method inside module */
  extern proc inside;

  extern proc commentless;

  /* Comment for method with an argument inside module */
  extern proc hasArg(val: int);

  extern proc hasArgCommentless(val: int);

  /* This method has a return type */
  extern proc hasReturnType(): int;
  
  extern proc hasReturnTypeCommentless(): int;
  
  /* This method has a return type and an argument */
  extern proc hasReturnTypeAndArg(val: int): int;
  
  extern proc hasReturnTypeAndArgCommentless(val: int): int;
}

/* Comment for method1 */
extern proc method1();

extern proc commentless();

/* Comment for method with an argument */
extern proc hasArg(val: int);

extern proc hasArgCommentless(val: int);

/* This comment is rather long
   And spans for
   Multiple lines */
extern proc longComment();

/* This method has a return type */
extern proc hasReturnType(): int;

extern proc hasReturnTypeCommentless(): int;

/* This method has a return type and an argument */
extern proc hasReturnTypeAndArg(val: int): int;

extern proc hasReturnTypeAndArgCommentless(val: int): int;

/* Commenting withExternCNameComment */
extern "test_name" proc withExternCNameComment();

extern "test_name" proc withExternCName();

/* test param method for withParamExternCName */
proc getName() param {
  return "name";
}

extern "test"+getName() proc withParamExternCName();

extern record externRecordNoComment { }

/* Commenting externRecordComment */
extern record externRecordComment {
  /* Commenting a field */
  var field:int;
}

extern "c_name1" record externRecordRenamedNoComment { }

/* Commenting externRecordRenamedComment */
extern "c_name2" record externRecordRenamedComment {
  /* Commenting a field */
  var field:int;
}

proc end() { }
