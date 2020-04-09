use IO;

var ff = openwriter("test.dat");
var m,x,y,z,vx,vy,vz : real;
m=0;
x=0;y=0;z=0;
vx=0; vy=0; vz=0;
for ii in 1.. #10000 {
  ff.writef("%12.6er %12.6er %12.6er %12.6er %12.6er %12.6er %12.6er\n",m,x,y,z,vx,vy,vz);
}
ff.close();

