config const inputfile = "blockLU-Mat.dat";
config const filterfile = "aafilter3x3.dat";

def AA(Img:[?D], Filter:[?FD], fx: int, fy: int, out outImg:[D]){
  for (NeighborDom,i,j) in aawindow(D.dim(1),D.dim(2),fx,fy){ 
    
    var newPixel : real = 0;
    var newPixelSum : real = 0;
    var f_i : int = 0; 
    
    for (ind,jnd) in NeighborDom do { //ind,jnd is the pixel around the pixel we're at
        //i is the pixel that we're on. ind is the pixel around this pixel
        //j is the pixel that we're on. jnd is the pixel around this pixel 
        if(ind > 0 && ind < D.dim(1).high && jnd > 0 && jnd < D.dim(2).high){
          newPixel += Img(ind,jnd) * Filter[f_i];
          newPixelSum += Filter[f_i];
        }
        f_i = f_i + 1;
    }
    outImg(i,j) = newPixel / newPixelSum;
  }     
}

def aawindow(W:range,H:range,filter_width:int,filter_height:int){
  for j in H { //which row
    for i in W { //which pixel
      
      var lo1 : int = i-filter_width/2; //previous pixel
      var lo2 : int = j-filter_height/2; //previous row
      
      var hi1 : int = i+filter_width/2; //next pixel
      var hi2 : int = j+filter_width/2; //next row
            
      yield ([lo1..hi1,lo2..hi2],i,j);
    }
  }
}

def main() {

  var infile = new file(inputfile,path='./',mode='r');
  infile.open();
  const m = infile.read(int),
        n = infile.read(int);

  const Width = 0..m-1, Height = 0..n-1, ImgDom = [Width, Height];
  var Img: [ImgDom] real;
  var outImg: [ImgDom] real;

  for ij in ImgDom do{
    infile.read(Img(ij));
    outImg(ij) = 0;
  }
  infile.close();
  
  var finfile = new file(filterfile,path='./',mode='r');
  finfile.open();
  
  const fx = finfile.read(int), fy = finfile.read(int); 
  const FilterDom = [0..((fx*fy)-1)];
  const Filter : [FilterDom] real;
  for i in FilterDom do{
    finfile.read(Filter(i));
  }
  finfile.close();

  writeln("Original Array:");
  writeln(Img);
  AA(Img, Filter, fx, fy, outImg); 
  writeln("Smoothed Array:");
  writeln(outImg); 
}
