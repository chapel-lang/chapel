// test error message for an illegal fully bounded range
config param anon = false;
if anon then for 0.0..1.0 {} else var badBounded = 0.0..1.0;
