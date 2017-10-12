// test error message for an illegal low bounded counted range
config param anon = false;
if anon then for (0.0..#1) {} else var badLowBounded = (0.0..#1);
