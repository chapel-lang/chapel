config const mode = 0;

config var zero = 0;
config var neg = -10;
on here.gpus[0] {
  select mode {
    when 0 {
      @gpu.blockSize(0)
        foreach 1..1500 { }
    }
    when 1 {
      @gpu.blockSize(-10)
        foreach 1..1500 { }
    }
    when 2 {
      @gpu.blockSize(zero)
        foreach 1..1500 { }
    }
    when 3 {
      @gpu.blockSize(neg)
        foreach 1..1500 { }
    }
  }
}
