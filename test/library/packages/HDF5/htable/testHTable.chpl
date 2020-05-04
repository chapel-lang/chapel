use HTable;

record Particle {
  var pos : c_array(real(32),3);
  var ndx : c_array(int,2);
  var mass : real;
  var pid : int;

  // A default initializer is necessary
  proc init() {};

  proc init(i : int) {
    this.complete();
    forall ip in 0..2 do this.pos[ip] = (3.14*i+ip):real(32);
    ndx[0] = 1;
    ndx[1] = i;
    mass = (i+1)*5.0;
    pid = i;
  }
}

record ParticleSmall {
  var pos : c_array(real(32),3);
  var mass : real;
  var pid : int;
}

var meta = new H5MetaTable(Particle);
var file_id = H5Fcreate( "ex_table_01.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );

var arr : [1..5] Particle;
forall ii in 1..5 do arr[ii] = new Particle(ii);

// Create an empty array
createH5Table(Particle, file_id, "points");
// Create an array with particles
createH5Table(arr, file_id, "particles");

// Append to table
appendH5Table(arr, file_id, "points");
appendH5Table(arr, file_id, "particles");

// Now read the entire point table in
var outarr = readH5Table(Particle, file_id, "points");
for iarr in outarr do writeln(iarr);

// Now read back a subset of another table
var outarr2 = readH5Table(ParticleSmall, file_id, "particles");
for iarr in outarr2 do writeln(iarr);


H5Fclose(file_id);

