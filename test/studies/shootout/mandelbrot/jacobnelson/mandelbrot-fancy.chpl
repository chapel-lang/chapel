
use BlockDist;

config const size : uint = 200;

// for binary output
extern proc putchar(i : int) : int;

proc main()
{
  const bytesRequired : uint = (size + 7) / 8;
  const byteRange = 0..#bytesRequired:int(64);
  const sizeRange = 0..#size:int(64);
  const myLocales = [loc in 0..#(bytesRequired:int)] Locales[loc % numLocales];

  var ByteDist = new dmap(new Block(rank=2,idxType=int(64),
                                    targetLocales=reshape(myLocales, {1..#(bytesRequired:int), 1..1}),
                                    boundingBox={sizeRange, byteRange}));

  var ByteDom: domain(2, int(64)) dmapped ByteDist = {sizeRange, byteRange};
  var ByteArr : [ByteDom] uint(8);


  const limit : real = 2.0;
  const maxIter : int = 50;


  forall (byte,(y,bytex)) in zip(ByteArr,ByteArr.domain) do
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
            while (iterNo > 0 && (Tr+Ti <= limit**2))
              {
                Zi = 2.0*Zr*Zi + Ci;
                Zr = Tr - Ti + Cr;
                Tr = Zr * Zr;
                Ti = Zi * Zi;
                iterNo -= 1;
              }
          }

          byte_acc <<= 1;
          if(Tr+Ti <= limit**2) then
            byte_acc |= 0x01;
        }

      byte = byte_acc;
    }



  writeln("P4");
  writeln(size," ",size);

  for (x,y) in ByteArr.domain do
    {
      putchar(ByteArr(x,y));
    }

}

