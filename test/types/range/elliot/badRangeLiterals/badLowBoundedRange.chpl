// test error message for an illegal low bounded range
config param anon = false;
if anon then for (0.0..) {} else var badLowBounded = (0.0..);
