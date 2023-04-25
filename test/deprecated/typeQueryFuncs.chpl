// empty space needed to match line numbers in .good







































// end empty space

// Type subtype relations (use named functions instead of operators).

writeln("Compare 1.type op int");
writeln("op <= returns ", 1.type <= int);
writeln("op <  returns ", 1.type <  int);
writeln("op >= returns ", 1.type >= int);
writeln("op >  returns ", 1.type >  int);

class BaseClass {}
class DerivedClass : BaseClass {}

writeln("Compare BaseClass op DerivedClass");
writeln("op <= returns ", BaseClass <= DerivedClass);
writeln("op <  returns ", BaseClass <  DerivedClass);
writeln("op >= returns ", BaseClass >= DerivedClass);
writeln("op >  returns ", BaseClass >  DerivedClass);

writeln("Compare DerivedClass.type op BaseClass.type");
writeln("op <= returns ", BaseClass <= DerivedClass);
writeln("op <  returns ", BaseClass <  DerivedClass);
writeln("op >= returns ", BaseClass >= DerivedClass);
writeln("op >  returns ", BaseClass >  DerivedClass);
