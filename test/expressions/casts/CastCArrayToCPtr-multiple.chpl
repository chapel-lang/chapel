/*
This test checks error reporting
for casts and argument conversions c_array --> c_ptr[Const]
with the following variations:

* source: var or const
* target: c_ptr or c_ptrConst
* source and target element types match
* the target element type is 'void'
* source and target element types do not match

See also:
  test/expressions/casts/CastCArrayToCPtr-correct.chpl
  test/extern/ferguson/c-array/c-array-coercions.chpl
*/

// case = 0 covers all cases that should compile correctly
config param case = 0;

use CTypes;

var   avar: c_array(int, 9);
const acon: c_array(int, 9);

proc p(arg) { }  // casts as statements are currently illegal syntax

proc qiv(arg: c_ptr(int))       { }
proc qic(arg: c_ptrConst(int))  { }
proc qrv(arg: c_ptr(real))      { }
proc qrc(arg: c_ptrConst(real)) { }

/// argument conversions ///////////////////////////////////////////////////

if case ==  0 then qiv(avar);
if case ==  0 then qic(avar);
if case == 13 then qrv(avar);  // error: unresolved -- int->real
if case == 14 then qrc(avar);  // error: unresolved -- int->real

if case == 21 then qiv(acon);  // error: const actual to ':'
if case ==  0 then qic(acon);
if case == 23 then qrv(acon);  // error: unresolved -- int->real
if case == 24 then qrc(acon);  // error: unresolved -- int->real

/// casts //////////////////////////////////////////////////////////////////

/////////// SAME source and target types ///////////

if case ==  0 then p(avar : c_ptr(int));
if case == 32 then p(acon : c_ptr(int));   // error: const actual to ref formal
if case ==  0 then p(avar : c_ptrConst(int));
if case ==  0 then p(acon : c_ptrConst(int));

/////////// VOID target type ///////////

if case ==  0 then p(avar : c_ptr(void));
if case == 42 then p(acon : c_ptr(void));  // error: const actual to ref formal
if case ==  0 then p(avar : c_ptrConst(void));
if case ==  0 then p(acon : c_ptrConst(void));

/////////// MISMATCH of source and target types ///////////

if case == 51 then p(avar : c_ptr(real));      // error -- int->real
if case == 52 then p(acon : c_ptr(real));      // error -- int->real
if case == 53 then p(avar : c_ptrConst(real)); // error -- int->real
if case == 54 then p(acon : c_ptrConst(real)); // error -- int->real
