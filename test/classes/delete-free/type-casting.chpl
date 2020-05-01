config param DuplicateManagement = false;

class MyClass { }

proc makeBorrowed1(type t) type {
  return borrowed t;
}
proc makeBorrowed2(type t) type {
  return t:borrowed;
}

proc makeBorrowedBang1(type t) type {
  return borrowed (t :borrowed class);
}
proc makeBorrowedBang2(type t) type {
  return t:borrowed class;
}

proc makeBorrowedQ1(type t) type {
  return borrowed t?;
}
proc makeBorrowedQ2(type t) type {
  return t:borrowed class?;
}

proc check(param arg:bool) {
  if !arg then
    compilerError("check failed");
}

proc main() {
  check(makeBorrowed1(MyClass) == borrowed MyClass);
  if DuplicateManagement {
    check(makeBorrowed1(borrowed MyClass) == borrowed MyClass);
    check(makeBorrowed1(unmanaged MyClass) == borrowed MyClass);
    check(makeBorrowed1(owned MyClass) == borrowed MyClass);
  }
  check(makeBorrowed1(MyClass?) == borrowed MyClass?);
  if DuplicateManagement {
    check(makeBorrowed1(borrowed MyClass?) == borrowed MyClass?);
    check(makeBorrowed1(unmanaged MyClass?) == borrowed MyClass?);
    check(makeBorrowed1(owned MyClass?) == borrowed MyClass?);
  }

  check(makeBorrowed2(MyClass) == borrowed MyClass);
  check(makeBorrowed2(borrowed MyClass) == borrowed MyClass);
  check(makeBorrowed2(unmanaged MyClass) == borrowed MyClass);
  check(makeBorrowed2(owned MyClass) == borrowed MyClass);
  check(makeBorrowed2(MyClass?) == borrowed MyClass?);
  check(makeBorrowed2(borrowed MyClass?) == borrowed MyClass?);
  check(makeBorrowed2(unmanaged MyClass?) == borrowed MyClass?);
  check(makeBorrowed2(owned MyClass?) == borrowed MyClass?);

  if DuplicateManagement {
    check(makeBorrowedBang1(MyClass) == borrowed MyClass);
    check(makeBorrowedBang1(borrowed MyClass) == borrowed MyClass);
    check(makeBorrowedBang1(unmanaged MyClass) == borrowed MyClass);
    check(makeBorrowedBang1(owned MyClass) == borrowed MyClass);
  }
  if DuplicateManagement {
    check(makeBorrowedBang1(MyClass?) == borrowed MyClass);
    check(makeBorrowedBang1(borrowed MyClass?) == borrowed MyClass);
    check(makeBorrowedBang1(unmanaged MyClass?) == borrowed MyClass);
    check(makeBorrowedBang1(owned MyClass?) == borrowed MyClass);
  }

  check(makeBorrowedBang2(MyClass) == borrowed MyClass);
  check(makeBorrowedBang2(borrowed MyClass) == borrowed MyClass);
  check(makeBorrowedBang2(unmanaged MyClass) == borrowed MyClass);
  check(makeBorrowedBang2(owned MyClass) == borrowed MyClass);
  check(makeBorrowedBang2(MyClass?) == borrowed MyClass);
  check(makeBorrowedBang2(borrowed MyClass?) == borrowed MyClass);
  check(makeBorrowedBang2(unmanaged MyClass?) == borrowed MyClass);
  check(makeBorrowedBang2(owned MyClass?) == borrowed MyClass);

  check(makeBorrowedQ1(MyClass) == borrowed MyClass?);
  if DuplicateManagement {
    check(makeBorrowedQ1(borrowed MyClass) == borrowed MyClass?);
    check(makeBorrowedQ1(unmanaged MyClass) == borrowed MyClass?);
    check(makeBorrowedQ1(owned MyClass) == borrowed MyClass?);
  }
  check(makeBorrowedQ1(MyClass?) == borrowed MyClass?);
  if DuplicateManagement {
    check(makeBorrowedQ1(borrowed MyClass?) == borrowed MyClass?);
    check(makeBorrowedQ1(unmanaged MyClass?) == borrowed MyClass?);
    check(makeBorrowedQ1(owned MyClass?) == borrowed MyClass?);
  }

  check(makeBorrowedQ2(MyClass) == borrowed MyClass?);
  check(makeBorrowedQ2(borrowed MyClass) == borrowed MyClass?);
  check(makeBorrowedQ2(unmanaged MyClass) == borrowed MyClass?);
  check(makeBorrowedQ2(owned MyClass) == borrowed MyClass?);
  check(makeBorrowedQ2(MyClass?) == borrowed MyClass?);
  check(makeBorrowedQ2(borrowed MyClass?) == borrowed MyClass?);
  check(makeBorrowedQ2(unmanaged MyClass?) == borrowed MyClass?);
  check(makeBorrowedQ2(owned MyClass?) == borrowed MyClass?);
}
