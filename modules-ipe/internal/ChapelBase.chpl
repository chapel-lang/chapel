// ChapelBase.chpl
//

module ChapelBase
{
  //
  // Assignment operators
  //
  inline proc =  (ref a : bool, b : bool) : void { __primitive("=", a, b); }
  inline proc =  (ref a : int,  b : int ) : void { __primitive("=", a, b); }
  inline proc =  (ref a : real, b : real) : void { __primitive("=", a, b); }

  //
  // Basic arithmetic operators
  //
  inline proc +  (    a : int,  b : int ) : int  return __primitive("+",  a, b);
  inline proc -  (    a : int,  b : int ) : int  return __primitive("-",  a, b);
  inline proc *  (    a : int,  b : int ) : int  return __primitive("*",  a, b);
  inline proc /  (    a : int,  b : int ) : int  return __primitive("/",  a, b);

  inline proc +  (    a : real, b : real) : real return __primitive("+",  a, b);
  inline proc -  (    a : real, b : real) : real return __primitive("+",  a, b);
  inline proc *  (    a : real, b : real) : real return __primitive("+",  a, b);
  inline proc /  (    a : real, b : real) : real return __primitive("+",  a, b);

  //
  // Equality comparison on primitive types
  //
  inline proc == (    a : bool, b : bool) : bool return __primitive("==", a, b);
  inline proc == (    a : int,  b : int ) : bool return __primitive("==", a, b);
  inline proc == (    a : real, b : real) : bool return __primitive("==", a, b);

  inline proc != (    a : bool, b : bool) : bool return __primitive("!=", a, b);
  inline proc != (    a : int,  b : int ) : bool return __primitive("!=", a, b);
  inline proc != (    a : real, b : real) : bool return __primitive("!=", a, b);

  //
  // Ordered comparison on primitive types
  //
  inline proc <  (    a : int,  b : int ) : bool return __primitive("<",  a, b);
  inline proc <  (    a : real, b : real) : bool return __primitive("<",  a, b);

  inline proc >  (    a : int,  b : int ) : bool return __primitive(">",  a, b);
  inline proc >  (    a : real, b : real) : bool return __primitive(">",  a, b);

  inline proc <= (    a : int,  b : int ) : bool return __primitive("<=", a, b);
  inline proc <= (    a : real, b : real) : bool return __primitive("<=", a, b);

  inline proc >= (    a : int,  b : int ) : bool return __primitive(">=", a, b);
  inline proc >= (    a : real, b : real) : bool return __primitive(">=", a, b);


  //
  // Support for implicit boolean conversion for conditional expressions
  //
  inline proc _cond_test(x : bool) : bool return x;
  inline proc _cond_test(x : int ) : bool return x != 0;
}
