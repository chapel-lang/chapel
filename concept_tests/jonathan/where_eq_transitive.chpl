proc fn(type a, type b, type c, x:a) : c where (a==b, b==c) checked {
  return x;
}
