
config const size : uint = 200;

// for binary output
extern proc putchar(i : int) : int;

proc main()
{
  const bytesRequired : uint = (size + 7) / 8;
  const byteRange = 0..#bytesRequired:int(64);
  const sizeRange = 0..#size:int(64);

  var byteArr : [sizeRange, byteRange] uint(8);

  const limit : real = 4.0;
  const maxIter : int = 50;


  forall (byte,(y,bytex)) in zip(byteArr,byteArr.domain) do
    {
      var byte_acc : uint(8) = 0;

      for x in (bytex*8)..#8 do
        {
          var Zr : real = 0.0;
          var Zi : real = 0.0;
          var Tr : real = 0.0;
          var Ti : real = 0.0;
          const Cr : real = (2.0*x/size - 1.5);
          const Ci : real = (2.0*y/size - 1.0);

          {
            var iterNo : int = maxIter;
            while (iterNo > 0 && (Tr+Ti <= limit))
              {
                Zi = 2.0*Zr*Zi + Ci;
                Zr = Tr - Ti + Cr;
                Tr = Zr * Zr;
                Ti = Zi * Zi;
                iterNo -= 1;
              }
          }

          byte_acc <<= 1;
          if(Tr+Ti <= limit) then
            byte_acc |= 0x01;
        }

      byte = byte_acc;
    }



  writeln("P4");
  writeln(size," ",size);

  for (x,y) in byteArr.domain do
    {
      putchar(byteArr(x,y));
    }

}

