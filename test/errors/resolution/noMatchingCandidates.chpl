{ // expects split-init
  proc fn(const arg) {
    arg;
  }  
  var x;
  fn(x);
  x=5;  
}
{ // type mismatch
  proc fn(arg:string) {
    arg;
  }  
  var x=5;
  fn(x);  
}
// Tests needed:
// call was parenful but method is parenless

// call was parenless but method was parenful

// bad where clause

// bad vararg count

// star tuple mismatches

// tuple size mismatches

// The 'ref' intent requires the formal and actual types to match exactly

// bad subtype

// incompatible manager

// incompatible nilability
