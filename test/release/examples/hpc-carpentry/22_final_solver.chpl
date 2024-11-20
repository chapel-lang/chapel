use BlockDist;
config const n = 8;
const mesh: domain(2) = {1..n,1..n};
const largerMesh: domain(2) dmapped Block(boundingBox=mesh) = {0..n+1,0..n+1};
var T, Tnew: [largerMesh] real;
  var x = ((i:real)-0.5)/(n:real);
  var y = ((j:real)-0.5)/(n:real);
  Tnew[i,j]=exp(-((x-0.5)**2 + (y-0.5)**2) / 0.01);
}
for step in 1..5 {
  forall (i,j) in Tnew.domain[1..n,1..n] {
    Tnew[i,j] = (T[i-1,j]+T[i+1,j]+T[i,j-1]+T[i,j+1])/4.0;
  }
  T = Tnew;
  writeln((step,T[n/2,n/2],T[1,1]));
  // Compute total energy
  var total: real = 0;
  forall (i,j) in mesh with (+ reduce total) do
    total += T[i,j];
  writeln("Total energy = ", total);
}

// Write out data, NetCDF or HDF5 integration would be nice
var myFile = open("output.dat", iomode.cw); // open the file for writing
var myWritingChannel = myFile.writer();     // create a writing channel
                                            // starting at file offset 0
myWritingChannel.write(T);                  // write the array
myWritingChannel.close();                   // close the channel
