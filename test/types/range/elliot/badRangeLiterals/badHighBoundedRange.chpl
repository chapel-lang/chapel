// test error message for an illegal high bounded range
config param anon = false;
if anon then for (..1.0) {} else var badHighBounded = (..1.0);
