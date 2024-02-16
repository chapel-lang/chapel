class A {}
class Parent {}
class Child: Parent {}

{

  var ownedA = new A();
  var nilableOwnedA = new A?();
  var ownedChild = new Child();
  var nilableOwnedChild = new Child?();
  var ownedParent: Parent = new Child();
  var nilableOwnedParent: Parent? = new Child?();

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
  var sharedParent: Parent = new shared Child();
  var nilableSharedParent: Parent? = new shared Child?();

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
  { var x = ownedParent:unmanaged Child; }
  { var x = nilableOwnedParent:unmanaged Child; }
  { var x = ownedParent:unmanaged Child?; }
  { var x = nilableOwnedParent:unmanaged Child?; }

  compilerWarning("");

  // these are ok and should not produce warnings
  { var x = borrowedParent:unmanaged Child; }
  { var x = nilableBorrowedParent:unmanaged Child; }
  { var x = borrowedParent:unmanaged Child?; }
  { var x = nilableBorrowedParent:unmanaged Child?; }

  compilerWarning("");

  { var x = sharedParent:unmanaged Child; }
  { var x = nilableSharedParent:unmanaged Child; }
  { var x = sharedParent:unmanaged Child?; }
  { var x = nilableSharedParent:unmanaged Child?; }

}
