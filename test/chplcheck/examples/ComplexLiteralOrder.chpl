/*
  Complex literals should be written with the real part first, such as
  ``2 + 10i`` rather than ``10i + 2``.
*/

// This complex literal has the imaginary part first, which violates the rule.
var wrongOrder = 10i + 2;

// This complex literal has the real part first, which is correct.
var rightOrder = 2 + 10i;
