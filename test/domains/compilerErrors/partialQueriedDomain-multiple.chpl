module M {
 config const r: int = 8;
 config const c: int = 9;

 var nbr: int = r;
 var nbc: int = c;
 var ndr: int = r+1;
 var ndc: int = c+1;

 class gs {

   var bd: [0..nbr-1, 0..nbc-1] uint(8);
   var sf: [ 0 .. 3 * ndr * ndc - 1, 0 .. 1 ] uint(8);

   proc init() {
     bd = 0;
   }
 };

 proc gs.c_l(
   ref ar: [?D1, ?D2] uint(8)) {   // <- source of the error

   return ar[0];
 }

 proc gs.ic() {
   c_l(sf);
 }

 proc main() {
   var b = new owned gs();
   b.ic();
 }
}
