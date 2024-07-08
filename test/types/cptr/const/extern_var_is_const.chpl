use CTypes;

extern {
  static const int myconstint = 3;
  static const int* myconstptr = &myconstint;
}

myconstptr.deref();     // ok
myconstptr.deref() = 4; // should fail
