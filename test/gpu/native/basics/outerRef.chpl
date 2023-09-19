use GPU;

{
  proc testValue(){
    var exp = 1234;

    on here.gpus[0]{
      var A: [0..0] int;
      @assertOnGpu
      foreach a in A do a = exp;

      writeln(A);
    }
    exp = exp + 1; // Without this line there's no error because exp will be
                   // RVFed, otherwise we used to get garbage because it is a
                   // ref
  }

  testValue();
}

{
  proc testValue(){
    var exp = 1234;

    ref rExp = exp;

    on here.gpus[0]{
      var A: [0..0] int;
      @assertOnGpu
      foreach a in A do a = rExp;

      writeln(A);
    }
    exp = exp + 1; // Without this line there's no error because exp will be
                   // RVFed, otherwise we used to get garbage because it is a
                   // ref
  }

  testValue();
}

{
  proc testValue(){
    var exp = 1234;


    on here.gpus[0]{
      var A: [0..0] int;
      ref rExp = exp;
      @assertOnGpu
      foreach a in A do a = rExp;

      writeln(A);
    }
    exp = exp + 1; // Without this line there's no error because exp will be
                   // RVFed, otherwise we used to get garbage because it is a
                   // ref
  }

  testValue();
}
