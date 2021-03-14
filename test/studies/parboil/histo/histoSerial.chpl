use IO;

config var numIterations = 1;
config const printTimers = 0;

proc main(args: [] string) {
  use Time;
  var inputFilename: string;
  var outputFilename: string;
  var inputTime, outputTime, computeTime, totalTime: Timer;
  if args.size == 4 {
    inputFilename = args[1];
    outputFilename = args[2];
    numIterations = args[3]:int;
  } else {
    writeln("num args: ", args.size);
    writeln(args);
    halt("Expected 4 args: a.out infile, outfile, numIterations");
  }

  var imgWidth, imgHeight, histoWidth, histoHeight: uint(32);
  totalTime.start();
  inputTime.start();
  var f = open(inputFilename, iomode.r);
  var r = f.reader(kind=ionative);

  r.read(imgWidth);
  r.read(imgHeight);
  r.read(histoWidth);
  r.read(histoHeight);

  var img: [0..#imgWidth*imgHeight] uint(32);
  var histo: [0..#histoWidth*histoHeight] uint(8);

  r.read(img);
  r.close();
  f.close();
  inputTime.stop();

  computeTime.start();
  for it in 0..#numIterations {
    histo = 0;
    const maxUint8 = max(uint(8));
    for val in img {
      if histo[val] < maxUint8 then histo[val] += 1;
    }
  }
  computeTime.stop();

  outputTime.start();
  dumpHisto(histo, histoHeight, histoWidth, outputFilename);
  outputTime.stop();
  totalTime.stop();
  if printTimers >= 3 {
    writeln("IO: ", inputTime.elapsed() + outputTime.elapsed());
    writeln(" -Input: ", inputTime.elapsed());
    writeln(" -Output: ", outputTime.elapsed());
  }
  if printTimers >= 2 then writeln("Compute: ", computeTime.elapsed());
  if printTimers >= 1 then writeln("Timer Wall Time: ", totalTime.elapsed());
}

record RGB {
  var B, G, R: uint(8);
}

proc HSVtoRGB(h: real, s: real, v: real) {
  var value: RGB;
  
  if s == 0 {
    return value;
  }

  var hh = h / 60;
  var i:int = floor(hh):int;
  var f = hh - i;
  var p = v*(1-s);
  var q = v*(1-s*f);
  var t = v*(1-s*(1-f));
  var r,g,b: real;
  select i {
    when 0 { r = v; g = t; b = p; }
    when 1 { r = q; g = v; b = p; }
    when 2 { r = p; g = v; b = t; }
    when 3 { r = p; g = q; b = v; }
    when 4 { r = t; g = p; b = v; }
    otherwise { r = v; g = p; b = q; }
  }
  value.R = (r*255):uint(8);
  value.G = (g*255):uint(8);
  value.B = (b*255):uint(8);

  return value;
}

proc dumpHisto(histo: [] uint(8), height: uint(32), width: uint(32), outFilename: string) {
  var pixMap: [0..#height*width] RGB;
  for y in 0..#height {
    for x in 0..#width {
      const pos = y*width + x;
      var value = histo[pos];
      if value == 0 then
        (pixMap[pos].R, pixMap[pos].G, pixMap[pos].B) = (0:uint(8),0:uint(8),0:uint(8));
      else
        pixMap[pos] = HSVtoRGB(0.0, 1.0, cbrt(1+63.0*value/max(uint(8)))/4);
    }
  }
  createBMP(pixMap, height, width, outFilename);
}

record bmpFileHeader {
  var filesz: uint(32);
  var creator1: uint(16);
  var creator2: uint(16);
  var bmpOffset: uint(32);
}

record bmpDibHeader {
  var headerSz: uint(32);
  var width: uint(32);
  var height: uint(32);
  var nplanes: uint(16);
  var bitspp: uint(16);
  var compressType: uint(32);
  var bmpByteSz: uint(32);
  var hres: uint(32);
  var vres: uint(32);
  var ncolors: uint(32);
  var nimpcolors: uint(32);
}

record bmpImage {
  var magic: 2*uint(8);
  var fileHeader: bmpFileHeader;
  var dibHeader: bmpDibHeader;
}

extern proc sizeof(type T): uint(32);

proc createBMP(bitmap:[] RGB, height: uint(32), width: uint(32), filename: string) {
  var image: bmpImage;
  var paddedWidth = 4*(((width*24)+31)/32) - width*sizeof(RGB);
  var pad: [0..#paddedWidth] uint(8);

  image.magic(0) = 'B'.toByte();
  image.magic(1) = 'M'.toByte();

  image.fileHeader.filesz = 2 + sizeof(bmpFileHeader) + sizeof(bmpDibHeader) + height*width*sizeof(RGB);
  image.fileHeader.creator1 = 0;
  image.fileHeader.creator2 = 0;
  image.fileHeader.bmpOffset = 2 + sizeof(bmpFileHeader) + sizeof(bmpDibHeader);

  image.dibHeader.headerSz = sizeof(bmpDibHeader);
  image.dibHeader.width = width;
  image.dibHeader.height = height;
  image.dibHeader.nplanes = 1;
  image.dibHeader.bitspp = 24;
  image.dibHeader.compressType = 0;
  image.dibHeader.bmpByteSz = width*height*sizeof(RGB);
  image.dibHeader.hres = 0;
  image.dibHeader.vres = 0;
  image.dibHeader.ncolors = 0;
  image.dibHeader.nimpcolors = 0;

  var f = open(filename, iomode.cw);
  var w = f.writer(kind=ionative);

  w.write(image.magic(0));
  w.write(image.magic(1));
  w.write(image.fileHeader);
  w.write(image.dibHeader);

  for h in 0..(height-1):int by -1 {
    w.write(bitmap[h*width..#width]);
    w.write(pad);
  }

  w.close();
  f.close();
}
