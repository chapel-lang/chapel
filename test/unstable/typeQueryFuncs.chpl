// Type subtype relations (operators now deprecated in favor of named
// functions)

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

