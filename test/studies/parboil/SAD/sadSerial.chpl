use IO;

config param useConfigFiles = true;
config param printTimers = 0;

config var filename1 = "image1.img";
config var filename2 = "image2.img";
config var outfilename = "output.bin";

param MAX_POS = 1089;
param MAX_POS_PADDED = 1092;
param SEARCH_RANGE = 16;

record image {
  var width, height: uint(16);
  var Dom: domain(1);
  var data: [Dom] uint(16);
}

proc main(args:[] string) {
  use Time;
  var IOTimer, ComputeTimer, WallTimer: Timer;
  WallTimer.start();
  if args.size == 3 {
    filename1 = args[1];
    filename2 = args[2];
    outfilename = args[3];
  } else if !useConfigFiles {
    halt("Expecting two input filenames and one output filename");
  }
  IOTimer.start();
  var refImage = loadImage(filename1);
  var curImage = loadImage(filename2);
  IOTimer.stop();
  ComputeTimer.start();

  if refImage.width != curImage.width || refImage.height != curImage.height {
    halt("Input images must be the same size");
  }

  if refImage.width % 16 != 0 || refImage.height % 16 != 0 {
    halt("Input image size must be an integral multiple of 16");
  }

  const imageSizeBytes: int = refImage.width * refImage.height * 2;
  const imageWidthMacroblocks: int = refImage.width / 16;
  const imageHeightMacroblocks: int = refImage.height / 16;
  const imageSizeMacroblocks: int = imageWidthMacroblocks * imageHeightMacroblocks;

  var sadsComputed: [0..#(41 * MAX_POS_PADDED * imageSizeMacroblocks)] uint(16);

  sad4CPU(sadsComputed, curImage.data, refImage.data, refImage.width/16, refImage.height/16);
  largerSads(sadsComputed, imageSizeMacroblocks);
  ComputeTimer.stop();
  IOTimer.start();
  writeSads(outfilename, imageWidthMacroblocks, imageHeightMacroblocks, sadsComputed);
  IOTimer.stop();

  if printTimers >= 3 then writeln("IO : ", IOTimer.elapsed());
  if printTimers >= 2 then writeln("Compute : ", ComputeTimer.elapsed());
  if printTimers >= 1 then writeln("Timer Wall Time: ", WallTimer.elapsed());
}


proc loadImage(filename: string) {
  var f = open(filename, iomode.r);
  var r = f.reader(kind=ionative);
  var width, height: uint(16);

  r.read(width);
  r.read(height);

  var img = new image(width, height, {0..#width*height});

  r.read(img.data);
  r.close();
  f.close();
  return img;
}

param SAD_TYPE_1_CT = 1;
param SAD_TYPE_2_CT = 2;
param SAD_TYPE_3_CT = 2;
param SAD_TYPE_4_CT = 4;
param SAD_TYPE_5_CT = 8;
param SAD_TYPE_6_CT = 8;
param SAD_TYPE_7_CT = 16;

proc SAD_TYPE_CT(n) {
  select n {
    when 1 do return SAD_TYPE_1_CT;
    when 2 do return SAD_TYPE_2_CT;
    when 3 do return SAD_TYPE_3_CT;
    when 4 do return SAD_TYPE_4_CT;
    when 5 do return SAD_TYPE_5_CT;
    when 6 do return SAD_TYPE_6_CT;
    when 7 do return SAD_TYPE_7_CT;
    otherwise halt("bad number in SAD_TYPE_CT");
  }
}

proc SAD_TYPE_1_IX(imageSize) return 0;
proc SAD_TYPE_2_IX(imageSize) return imageSize * MAX_POS_PADDED;
proc SAD_TYPE_3_IX(imageSize) return imageSize * (3 * MAX_POS_PADDED);
proc SAD_TYPE_4_IX(imageSize) return imageSize * (5 * MAX_POS_PADDED);
proc SAD_TYPE_5_IX(imageSize) return imageSize * (9 * MAX_POS_PADDED);
proc SAD_TYPE_6_IX(imageSize) return imageSize * (17 * MAX_POS_PADDED);
proc SAD_TYPE_7_IX(imageSize) return imageSize * (25 * MAX_POS_PADDED);

proc SAD_TYPE_IX(n, imageSize) {
  select n {
    when 1 do return SAD_TYPE_1_IX(imageSize);
    when 2 do return SAD_TYPE_2_IX(imageSize);
    when 3 do return SAD_TYPE_3_IX(imageSize);
    when 4 do return SAD_TYPE_4_IX(imageSize);
    when 5 do return SAD_TYPE_5_IX(imageSize);
    when 6 do return SAD_TYPE_6_IX(imageSize);
    when 7 do return SAD_TYPE_7_IX(imageSize);
    otherwise halt("bad number in SAD_TYPE_IX");
  }
}


proc write16u(w, i: uint(16)) {
  w.write(i);
/*
  var tmp: uint(8);
  tmp =        (i & 0xff): uint(8); w.write(tmp);
  tmp = ((i >> 8) & 0xff): uint(8); w.write(tmp);
*/
}

proc write32u(w, i: uint(32)) {
  w.write(i);
/*
  var tmp: uint(8);
  tmp =         (i & 0xff): uint(8); w.write(tmp);
  tmp = ( (i >> 8) & 0xff): uint(8); w.write(tmp);
  tmp = ((i >> 16) & 0xff): uint(8); w.write(tmp);
  tmp = ((i >> 24) & 0xff): uint(8); w.write(tmp);
*/
}

proc write_subblocks(w, subblockArray: [] uint(16),
                     macroblock: int, count: int) {
  const sLow = subblockArray.domain.low;
  for block in 0..#count {
    ref vec = subblockArray[sLow + (block + macroblock*count)*MAX_POS_PADDED..#MAX_POS];
    for elt in vec {
      write16u(w, elt);
    }
  }
}

proc writeSads(filename: string, width: int, height: int, sads: [] uint(16)) {
  var f = open(filename, iomode.cw);
  var w = f.writer(kind=ionative);
  const mbs = width*height;
  const rowInds = [0, 1, height/2 - 1, height/2, height - 2, height - 1];
  write32u(w, (width*6):uint(32));
  write32u(w, 0:uint(32));
  for row in rowInds {
    for block in width*row..(width*(row+1)-1) {
      for blocktype in 1..7 {
        write_subblocks(w, sads[SAD_TYPE_IX(blocktype, mbs)..], block,
                        SAD_TYPE_CT(blocktype));
      }
    }
  }
  w.close();
  f.close();
}

proc sad4CPU(blkSad: [] uint(16), frame: [] uint(16), reference: [] uint(16), width: int, height: int) {
  for (mby, yoff) in zip(0..#height, 0.. by 256*width) {
    for mbx in 0..#width {
      ref blkOff = blkSad[(SAD_TYPE_7_IX(width*height) + (mby*width + mbx) * (SAD_TYPE_7_CT * MAX_POS_PADDED))..];
      ref frameOff = frame[(yoff + mbx*16)..];
      sad4OneMacroblock(blkOff, frameOff, reference, mby*16, mbx*16, width, height);
    }
  }
}

proc sad4OneMacroblock(macroblockSad: [] uint(16), frame: [] uint(16), reference: [] uint(16), framey: int, framex: int, mbWidth: int, mbHeight: int) {
  var pos = 0;
  var width = mbWidth * 16;
  var height = mbHeight * 16;
  const mlow = macroblockSad.domain.low;
  const flow = frame.domain.low;
  const rlow = reference.domain.low;
  for posy in -SEARCH_RANGE..SEARCH_RANGE {
    for posx in -SEARCH_RANGE..SEARCH_RANGE {
      for blky in 0..#4 {
        for blkx in 0..#4 {
          var sad: uint(16);
          for y in 0..#4 {
            for x in 0..#4 {
              var refx = min(width - 1, max(0, framex + posx + blkx*4 + x));
              var refy = min(height - 1, max(0, framey + posy + blky*4 + y));
              var b = reference[rlow + refy*width + refx];
              var a = frame[flow + (blky*4 + y) * width + (blkx*4 + x)];
              sad += abs(a:int - b:int):uint(16); // danger, need these casts
            }
          }
          macroblockSad[mlow + MAX_POS_PADDED*(4*blky + blkx) + pos] = sad;
        }
      }
      pos += 1;
    }
  }
}

proc largerSads(sads: [] uint(16), mbs: int) {
  param MPP = MAX_POS_PADDED;
  for macroblock in 0..#mbs {
    /* Block type 6 */
    for blky in 0..#2 {
      for blkx in 0..#4 {
        const xoff = SAD_TYPE_7_IX(mbs) + macroblock*SAD_TYPE_7_CT*MPP + (8*blky + blkx)*MPP;
        ref x = sads[xoff..#MAX_POS];
        ref y = sads[xoff + 4*MPP..#MAX_POS];
        ref z = sads[SAD_TYPE_6_IX(mbs) + macroblock*SAD_TYPE_6_CT*MPP + (4*blky + blkx)*MPP..#MAX_POS];
        //z = x + y;
        for (zz, xx, yy) in zip(z, x, y) do zz = xx + yy;
      }
    }
    /* Block type 5 */
    for blky in 0..#4 {
      for blkx in 0..#2 {
        const xoff = SAD_TYPE_7_IX(mbs) + macroblock*SAD_TYPE_7_CT * MPP + (4*blky + 2*blkx) * MPP;
        ref x = sads[xoff..#MAX_POS];
        ref y = sads[xoff+MPP..#MAX_POS];
        ref z = sads[SAD_TYPE_5_IX(mbs) + macroblock*SAD_TYPE_6_CT * MPP + (2*blky+blkx)*MPP..#MAX_POS];
        //z = x + y;
        for (zz, xx, yy) in zip(z, x, y) do zz = xx + yy;
      }
    }
    /* Block type 4 */
    for blky in 0..#2 {
      for blkx in 0..#2 {
        const xoff = SAD_TYPE_5_IX(mbs) + macroblock*SAD_TYPE_5_CT * MPP + (4*blky+blkx)*MPP;
        ref x = sads[xoff..#MAX_POS];
        ref y = sads[xoff + 2*MPP..#MAX_POS];
        ref z = sads[SAD_TYPE_4_IX(mbs) + macroblock*SAD_TYPE_4_CT*MPP + (2*blky + blkx)*MPP..#MAX_POS];
        //z = x + y;
        for (zz, xx, yy) in zip(z, x, y) do zz = xx + yy;
      }
    }
    /* Block type 3 */
    {
      const xoff = SAD_TYPE_4_IX(mbs) + macroblock * SAD_TYPE_4_CT * MPP;
      ref x = sads[xoff..#MAX_POS];
      ref y = sads[xoff+2*MPP..#MAX_POS];
      ref z = sads[SAD_TYPE_3_IX(mbs) + macroblock * SAD_TYPE_3_CT * MPP..#MAX_POS];
      //z = x + y;
      for (zz, xx, yy) in zip(z, x, y) do zz = xx + yy;
    }
    {
      const xoff = SAD_TYPE_4_IX(mbs) + macroblock * SAD_TYPE_4_CT * MPP + MPP;
      ref x = sads[xoff..#MAX_POS];
      ref y = sads[xoff+2*MPP..#MAX_POS];
      ref z = sads[SAD_TYPE_3_IX(mbs) + macroblock * SAD_TYPE_3_CT * MPP + MPP..#MAX_POS];
      //z = x + y;
      for (zz, xx, yy) in zip(z, x, y) do zz = xx + yy;
    }
    /* Block type 2 */
    {
      const xoff = SAD_TYPE_4_IX(mbs) + macroblock * SAD_TYPE_4_CT * MPP;
      ref x = sads[xoff..#MAX_POS];
      ref y = sads[xoff + MPP..#MAX_POS];
      ref z = sads[SAD_TYPE_2_IX(mbs) + macroblock * SAD_TYPE_2_CT * MPP..#MAX_POS];
      //z = x + y;
      for (zz, xx, yy) in zip(z, x, y) do zz = xx + yy;
    }
    {
      const xoff = SAD_TYPE_4_IX(mbs) + macroblock * SAD_TYPE_4_CT * MPP + 2*MPP;
      ref x = sads[xoff..#MAX_POS];
      ref y = sads[xoff + MPP..#MAX_POS];
      ref z = sads[SAD_TYPE_2_IX(mbs) + macroblock * SAD_TYPE_2_CT * MPP + MPP..#MAX_POS];
      //z = x + y;
      for (zz, xx, yy) in zip(z, x, y) do zz = xx + yy;
    }
    /* Block type 1 */
    {
      const xoff = SAD_TYPE_2_IX(mbs) + macroblock * SAD_TYPE_2_CT * MPP;
      ref x = sads[xoff..#MAX_POS];
      ref y = sads[xoff+MPP..#MAX_POS];
      ref z = sads[SAD_TYPE_1_IX(mbs) + macroblock * SAD_TYPE_1_CT * MPP..#MAX_POS];
      //z = x + y;
      for (zz, xx, yy) in zip(z, x, y) do zz = xx + yy;
    }
  }
}
