class A {}
class Parent {}
class Child: Parent {}

{

  var ownedA = new A();
  var nilableOwnedA = new A?();
  var ownedChild = new Child();
  var nilableOwnedChild = new Child?();
  var ownedParent = new Parent();
  var nilableOwnedParent = new Parent?();

  var borrowedA = ownedA.borrow();
  var nilableBorrowedA = nilableOwnedA.borrow();
  var borrowedChild = ownedChild.borrow();
  var nilableBorrowedChild = nilableOwnedChild.borrow();
  var borrowedParent = ownedParent.borrow();
  var nilableBorrowedParent = nilableOwnedParent.borrow();

  var sharedA = new shared A();
  var nilableSharedA = new shared A?();
  var sharedChild = new shared Child();
  var nilableSharedChild = new shared Child?();
  var sharedParent = new shared Parent();
  var nilableSharedParent = new shared Parent?();

  compilerWarning("flat casting");
  { var x = ownedA:unmanaged A; }
  { var x = nilableOwnedA:unmanaged A; }
  { var x = ownedA:unmanaged A?; }
  { var x = nilableOwnedA:unmanaged A?; }

  compilerWarning("");

  // these are ok and should not produce warnings
  { var x = borrowedA:unmanaged A; }
  { var x = nilableBorrowedA:unmanaged A; }
  { var x = borrowedA:unmanaged A?; }
  { var x = nilableBorrowedA:unmanaged A?; }

  compilerWarning("");

  { var x = sharedA:unmanaged A; }
  { var x = nilableSharedA:unmanaged A; }
  { var x = sharedA:unmanaged A?; }
  { var x = nilableSharedA:unmanaged A?; }

  compilerWarning("");

  compilerWarning("upcasting");
  { var x = ownedChild:unmanaged Parent; }
  { var x = nilableOwnedChild:unmanaged Parent; }
  { var x = ownedChild:unmanaged Parent?; }
  { var x = nilableOwnedChild:unmanaged Parent?; }

  compilerWarning("");

  // these are ok and should not produce warnings
  { var x = borrowedChild:unmanaged Parent; }
  { var x = nilableBorrowedChild:unmanaged Parent; }
  { var x = borrowedChild:unmanaged Parent?; }
  { var x = nilableBorrowedChild:unmanaged Parent?; }

  compilerWarning("");

  { var x = sharedChild:unmanaged Parent; }
  { var x = nilableSharedChild:unmanaged Parent; }
  { var x = sharedChild:unmanaged Parent?; }
  { var x = nilableSharedChild:unmanaged Parent?; }

  compilerWarning("");

  compilerWarning("downcasting");
  // down casting - these are runtime errors, catch the ClassCastError
  try { var x = ownedParent:unmanaged Child; } catch ClassCastError {}
  try { var x = nilableOwnedParent:unmanaged Child; } catch ClassCastError {}
  { var x = ownedParent:unmanaged Child?; }
  try { var x = nilableOwnedParent:unmanaged Child?; } catch ClassCastError {}

  compilerWarning("");

  // these are ok and should not produce warnings
  try { var x = borrowedParent:unmanaged Child; } catch ClassCastError {}
  try { var x = nilableBorrowedParent:unmanaged Child; } catch ClassCastError {}
  { var x = borrowedParent:unmanaged Child?; }
  try { var x = nilableBorrowedParent:unmanaged Child?; } catch ClassCastError {}

  compilerWarning("");

  try { var x = sharedParent:unmanaged Child; } catch ClassCastError {}
  try { var x = nilableSharedParent:unmanaged Child; } catch ClassCastError {}
  { var x = sharedParent:unmanaged Child?; }
  try { var x = nilableSharedParent:unmanaged Child?; } catch ClassCastError {}

}
