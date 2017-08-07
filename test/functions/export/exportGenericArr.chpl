  export
  proc flip_bit(v: [?D] int) {
    v[0] = -v[0];
    return v;
  }
