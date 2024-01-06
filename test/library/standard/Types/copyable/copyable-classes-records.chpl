use Types;

use Copyable;

class C { var x: int; }
record R { var y: int; }
enum MyEnum { val }

record GenericRecord { type xType; var x: xType; }

record GenericRecordTypeless { var x; }

record GenericRecordDefault { type t = int; var z: t; }

proc main() {

  var ownedC = new owned C();
  var ownedCQ = new owned C?();
  var sharedC = new shared C();
  var sharedCQ = new shared C?();
  var unmanagedC = new unmanaged C();
  var unmanagedCQ = new unmanaged C?();

  var genericR = new GenericRecord(int, 1);
  var genericRT = new GenericRecordTypeless(2);
  var genericRD: GenericRecordDefault;

  checkNormal(int, 1);
  checkNormal(real, 1.0);
  checkNormal(bool, true);
  checkNormal(MyEnum, MyEnum.val);
  checkNormal(R, new R(1));
  checkNormalNoDefault(borrowed C, ownedC.borrow());
  checkNormal(borrowed C?, ownedCQ.borrow());
  checkNormalNoDefault(unmanaged C, unmanagedC);
  checkNormal(unmanaged C?, unmanagedCQ);
  checkNormalNoDefault(shared C, sharedC);
  checkNormal(shared C?, sharedCQ);

  // nilable owned can generally be copied / assigned but only from a ref
  checkMutable(owned C?, ownedCQ);

  // non-nilable owned cannot generally be copied / assigned 
  checkNo(owned C, ownedC);

  var tup = ("a", "b", "c");
  checkNormal((string, string, string), tup);

  checkNormal(GenericRecord(int), genericR);
  checkNormalNoDefault(GenericRecordTypeless(int), genericRT);
  checkNormal(GenericRecordDefault, genericRD);

  delete unmanagedCQ;
  delete unmanagedC;
}
