// Tests C typedefs and structs

//NOTE: use C is necessary for Chapel to find the struct's
//  type constructor. Whether or not this is correct or
//  desirable behavior, it is consistent with how Chapel
//  already handles classes within modules. For example,
//  
//  module X {
//    class My_Class {
//      var foo:int;
//    }
//  }
//
//  within the current file would require a 
//    use X; 
//  statement in order to create a new instance of X.

module C { extern {
  #include "types.h" 

  struct my_struct {
    int foo;
    const char* bar;
  };

  typedef int my_int;
} }

use C;

//NOTE: either C.my_struct or my_struct will work with the use C; statement.
var strct: C.my_struct = new my_struct(42, "bar".c_str());
writeln(strct.foo);
writeln(strct.bar:string);

//NOTE: due to an issue with the way Chapel implements type aliases,
//
//  var x = new aliased_type_name(args)
//
//  results in an "invalid use of 'new'" error, so each member
//  must be assigned individually.
var td_strct: td_struct;
td_strct.d = 7.5;

var td_strct2: y;
td_strct2.x = 8;

var b: bar;
b.c = "Hello".c_str();
b.foo = 42;

var q:my_int;
q = 789;

write("q is ");
write(q);
writeln();

printf("%.1f, %d\n".c_str(), td_strct.d, td_strct2.x);
printf("%s, %d\n".c_str(), b.c, b.foo);

