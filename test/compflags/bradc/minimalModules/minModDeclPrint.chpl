var xyz = 1: int(8);
extern proc printf(f:c_string, x...);
printf("%d\n", xyz);

  pragma "tuple" record _tuple {
    param size : int;
  }
  
  pragma "build tuple"
  inline proc _build_tuple_always(x ...?size)
    return x;
  
