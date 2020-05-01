
class CC { var x: int; }

// The following creations are NOT allowed:
//
//  owned <- shared     test/classes/delete-free/owned/owned-from-shared.chpl
//  owned <- owned      test/classes/delete-free/owned/owned-from-owned.chpl
//                      - when the source is non-nilable, non-expiring
//  owned <- borrowed   test/classes/delete-free/owned/owned-from-borrow.chpl
//
//  shared <- owned     test/classes/delete-free/shared/shared-from-owned.chpl
//                      - when the source is non-nilable, non-expiring
//  shared <- borrowed  test/classes/delete-free/shared/shared-from-borrow.chpl

proc main {
  // os/od: owned source/owned destination

  var os1 = new owned CC(101)?;
  var od1 = owned.create(os1);
  compilerWarning(od1.type:string, 0);
  writeln(od1!.x);

  var os2 = new owned CC(102);
  var od2 = owned.create(os2); // OK because 'os2' is expiring
  compilerWarning(od2.type:string, 0);
  writeln(od2.x);

  var os5 = new unmanaged CC(105)?;
  var od5 = owned.create(os5);
  compilerWarning(od5.type:string, 0);
  writeln(od5!.x);

  var os6 = new unmanaged CC(106);
  var od6 = owned.create(os6);
  compilerWarning(od6.type:string, 0);
  writeln(od6.x);

  // ss/sd: shared source/shared destination

  var ss1 = new owned CC(201)?;
  var sd1 = shared.create(ss1);
  compilerWarning(sd1.type:string, 0);
  writeln(sd1!.x);

  var ss2 = new owned CC(202);
  var sd2 = shared.create(ss2); // OK because 'ss2' is expiring
  compilerWarning(sd2.type:string, 0);
  writeln(sd2.x);

  var ss3 = new shared CC(203)?;
  var sd3 = shared.create(ss3);
  compilerWarning(sd3.type:string, 0);
  writeln(sd3!.x);

  var ss4 = new shared CC(204);
  var sd4 = shared.create(ss4);
  compilerWarning(sd4.type:string, 0);
  writeln(sd4.x);

  var ss5 = new unmanaged CC(205)?;
  var sd5 = shared.create(ss5);
  compilerWarning(sd5.type:string, 0);
  writeln(sd5!.x);

  var ss6 = new unmanaged CC(206);
  var sd6 = shared.create(ss6);
  compilerWarning(sd6.type:string, 0);
  writeln(sd6.x);
}
