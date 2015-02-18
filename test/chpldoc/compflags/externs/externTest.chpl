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