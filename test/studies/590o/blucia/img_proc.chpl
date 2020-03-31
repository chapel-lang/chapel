use IO;

config const inputfile = "blockLU-Mat.dat";
config const filterfile = "aafilter3x3.dat";

proc AA(Img:[?D], Filter:[?FD], fx: int, fy: int, out outImg:[D] real){
  for (NeighborDom,i,j) in aawindow(D.dim(0),D.dim(1),fx,fy){ 
    
    var newPixel : real = 0;
    var newPixelSum : real = 0;
    var f_i : int = 0; 
    
    for (ind,jnd) in NeighborDom do { //ind,jnd is the pixel around the pixel we're at
        //i is the pixel that we're on. ind is the pixel around this pixel
        //j is the pixel that we're on. jnd is the pixel around this pixel 
        if(ind > 0 && ind < D.dim(0).high && jnd > 0 && jnd < D.dim(1).high){
          newPixel += Img(ind,jnd) * Filter[f_i];
          newPixelSum += Filter[f_i];
        }
        f_i = f_i + 1;
    }
    outImg(i,j) = newPixel / newPixelSum;
  }     
}

iter aawindow(W:range,H:range,filter_width:int,filter_height:int){
  for j in H { //which row
    for i in W { //which pixel
      
      var lo1 : int = i-filter_width/2; //previous pixel
      var lo2 : int = j-filter_height/2; //previous row
      
      var hi1 : int = i+filter_width/2; //next pixel
      var hi2 : int = j+filter_width/2; //next row
            
      yield ({lo1..hi1,lo2..hi2},i,j);
    }
  }
}

proc main() {

  var infile = open(inputfile, iomode.r);
  var reader = infile.reader();
  const m = reader.read(int),
        n = reader.read(int);

  const Width = 0..m-1, Height = 0..n-1, ImgDom = {Width, Height};
  var Img: [ImgDom] real;
  var outImg: [ImgDom] real;

  for ij in ImgDom do{
    reader.read(Img(ij));
    outImg(ij) = 0;
  }
  reader.close();
  infile.close();
  
  var finfile = open(filterfile, iomode.r);
  var freader = finfile.reader();
  
  const fx = freader.read(int), fy = freader.read(int); 
  const FilterDom = {0..((fx*fy)-1)};
  var Filter : [FilterDom] real;
  for i in FilterDom do{
    freader.read(Filter(i));
  }
  freader.close();
  finfile.close();

  writeln("Original Array:");
  writeln(Img);
  AA(Img, Filter, fx, fy, outImg); 
  writeln("Smoothed Array:");
  writeln(outImg); 
}
