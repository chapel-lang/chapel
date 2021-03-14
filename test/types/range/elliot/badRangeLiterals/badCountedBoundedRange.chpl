// test error message for an illegal fully bounded counted range
config param anon = false;
if anon then for 0..1 #1.0 {} else var badBounded = 0..1 #1.0;
