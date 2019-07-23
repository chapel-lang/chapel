record R { type t; }

class MyClass { }

class MyGenericClass { type t; }

proc check(param arg:bool) {
  if !arg then
    compilerError("check failed");
}

check(R == R);
check(R <= R);
check(isSubtype(R, R));

check(borrowed MyClass == borrowed MyClass);
check(borrowed MyClass <= borrowed MyClass);
check(borrowed MyClass != unmanaged MyClass);
check(borrowed MyClass != owned MyClass);
check(isSubtype(borrowed MyClass, borrowed MyClass));

check(MyClass == MyClass);
check(MyClass <= MyClass);
check(isSubtype(MyClass, MyClass));

check(borrowed MyClass != MyClass);
check(borrowed MyClass <= MyClass);
check(isSubtype(borrowed MyClass, MyClass));

check(owned MyClass != MyClass);
check(owned MyClass <= MyClass);
check(isSubtype(owned MyClass, MyClass));

check(borrowed MyGenericClass == borrowed MyGenericClass);
check(borrowed MyGenericClass <= borrowed MyGenericClass);
check(isSubtype(borrowed MyGenericClass, borrowed MyGenericClass));

check(MyGenericClass == MyGenericClass);
check(MyGenericClass <= MyGenericClass);
check(isSubtype(MyGenericClass, MyGenericClass));

check(borrowed MyGenericClass != MyGenericClass);
check(borrowed MyGenericClass <= MyGenericClass);
check(isSubtype(borrowed MyGenericClass, MyGenericClass));
