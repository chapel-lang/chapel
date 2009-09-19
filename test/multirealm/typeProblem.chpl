def main {

  var b: bool = true;
  var x: int(64) = 9223372036854775800;
  var sm1: int(16) = 32760;
  var sm2: int(8) = 120;
  var ux: uint(64) = 18446744073709551610;
  var usm1: uint(16) = 65530;
  var usm2: uint(8) = 250;
  var re: real(64) = .0123;
  var im: imag(64) = .4567i;
  var cp: complex(128) = .8 + .9i;
  var s: string = "abcdefghijklmnopqrstuvwxyz";
  var rm: realm;
  var lc: locale;
  enum e { allston, brighton, charleston, dorchester, east, fenway };
  class c {
    var x2: int(32) = 2147483640;
  }
  record rd {
    var ux2: uint(32);
  }
  var class1 = new c();
  var record1: rd;
  // Seems I can't make an unsigned 32-bit int be between 
  // the values 2147483647 and 4294967295.
  record1.ux2 = 2147483640;
  // Unions don't work with genCommunicatedStructures yet.
  //union u {
  //  var s2: string;
  //  var re2: real(32);
  //}
  //var union1: u;
  //union1.re2 = .98:real(32);
  var t: (imag(32), complex(64));
  t = (.76i:imag(32), (.54 + .32i):complex(64));
  type ty = int;
  var rg: range = 1..10;
  var d: domain(1) = [rg];
  var domain1: [d] ty;
  forall i in d do
    domain1(i) = i;
  coforall r in Realms {
    on r {
      coforall loc in r.Locales {
        on loc {

          var myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " bool: " + b;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " int(64): " + x;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " int(16): " + sm1;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " int(8): " + sm2;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " uint(64): " + ux;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " uint(16): " + usm1;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " uint(8): " + usm2;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " real(64): " + re;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " imag(64): " + im;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " complex(64): " + cp;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " string: " + s;
          writeln(myMessage);
          rm = Realms(0);
          lc = rm.Locales(0);
          on lc {
            myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " Hello!";
            writeln(myMessage);
          }
          var hood = e.dorchester;
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " enum: " + hood;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " class, int(32): " + class1.x2;
          writeln(myMessage);
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " record, uint(32): " + record1.ux2;
          writeln(myMessage);
          // Unions don't work with genCommunicatedStructures yet.
          //myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " union, real(32): " + union1.re2;
          //writeln(myMessage);
          //myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " tuple, imag(32), complex(64): ";
          writeln(t);
          //myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " domain, range, int, type: " + domain1;
          writeln(domain1);
          var v: ty = 4;
          myMessage = "realm: " + thisRealm.id + " locale: " + here.id + " type, int: " + v;
          writeln(myMessage);
        }
      }
    }
  }
}