


var s = "Some string...";

export proc badReturnIntentRef() ref {
  return s;
}

export proc badReturnIntentConstRef() const ref {
  return s;
}
