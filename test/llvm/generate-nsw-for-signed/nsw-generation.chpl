config const aSigned = 100;
config const bSigned = 5;

config const aUnsigned : uint = 100;
config const bUnsigned : uint = 5;

var signedAdd   : int;
var signedMul   : int;
var signedSub   : int;

var unsignedAdd   : uint;
var unsignedMul   : uint;
var unsignedSub   : uint;

proc test()
{
  // CHECK: add nsw i64
  signedAdd   = aSigned + bSigned;
  // CHECK: mul nsw i64
  signedMul   = aSigned * bSigned;
  // CHECK: sub nsw i64
  signedSub   = aSigned - bSigned;
  
  // CHECK: add i64
  unsignedAdd   = aUnsigned +  bUnsigned;   
  // CHECK: mul i64
  unsignedMul   = aUnsigned *  bUnsigned;   
  // CHECK: sub i64
  unsignedSub   = aUnsigned -  bUnsigned;   
}

test();

writeln(signedAdd, signedMul, signedSub, 
    unsignedAdd, unsignedMul, unsignedSub);

