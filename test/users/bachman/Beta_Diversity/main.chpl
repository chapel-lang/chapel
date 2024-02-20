//use png;
use distance_mask;
use dissimilarity;
//use IO_Module;
use IO;
use BlockDist;
use Time;
use AutoMath;
use Math;
use LinearAlgebra;
use IO.FormattedIO;

/* Command line arguments. */
config const in_name : string;
config const map_type : string;
config const window_size : real(32);
config const dx : real(32) = 5.0;
config const printReduce = true;

proc convolve_and_calculate(Image: [] int(8),
                            centerPoints : ?,
                            LeftMaskDomain : ?,
                            CenterMaskDomain : ?,
                            RightMaskDomain : ?,
                            dissimilarity : [] real(64),
                            ref Output: [] real(64),
                            d_size : int,
                            Mask_Size : int,
                            t: stopwatch) : [] {

  // This 'eps' makes sure that we differentiate between land points (zero) and ocean points (nonzero), even
  // if the beta diversity at the ocean point is zero.
  param eps = 0.00001;

  var first_point = centerPoints.first[1];
  var last_point = centerPoints.last[1];

  forall center in centerPoints[..,first_point] with (ref Output) {

      // Calculate masks and beta diversity for leftmost point in subdomain
      var B_left: [0..(d_size-1)] real(64) = 0;
      var B_center: [0..(d_size-1)] real(64) = 0;
      var B_right: [0..(d_size-1)] real(64) = 0;

      for m in LeftMaskDomain do {
        var tmp = Image[(center,first_point) + m];
        B_left[tmp] = B_left[tmp] + 1;
      }
      for m in CenterMaskDomain do {
        var tmp = Image[(center,first_point) + m];
        B_center[tmp] = B_center[tmp] + 1;
      }
      for m in RightMaskDomain do {
        var tmp = Image[(center,first_point) + m];
        B_right[tmp] = B_right[tmp] + 1;
      }

      var B = B_left + B_center + B_right;
      var B_old = B;

      // If we are over land, return zero
      if (Image[center,first_point] == 0) {
        Output[center,first_point] = 0.0;
      }
      // If we are over deep water, return a different number so we can color it differently
      else if (Image[center, first_point] == (d_size-1)) {
        Output[center,first_point] = -999.0;
      }
      // If we are on a reef point, calculate beta diversity
      else {
        var num_habitat_pixels = (+ reduce B[1..(d_size-2)]) : real(64);
        var habitat_frac = num_habitat_pixels / Mask_Size;

        var P = B / num_habitat_pixels;

        var beta = + reduce (dissimilarity * outer(P,P));
        Output[center,first_point] = (habitat_frac * beta) : real(64);
      }

      for point in (first_point+1)..last_point do {

        B_right = 0;
        for m in RightMaskDomain do {
          var tmp = Image[(center,point) + m];
          B_right[tmp] = B_right[tmp] + 1;
        }
        B = B_old + B_right - B_left;
        B_old = B;
        B_left = 0;

        // Update B_left
        for m in LeftMaskDomain do {
          var tmp = Image[(center,point) + m];
          B_left[tmp] = B_left[tmp] + 1;
        }

        // If we are over land, return zero
        if (Image[center,point] == 0) {
          Output[center,point] = 0.0;
        }
        // If we are over deep water, return a different number so we can color it differently
        else if (Image[center,point] == (d_size-1)) {
          Output[center,point] = -999.0;
        }
        // If we are on a reef point, calculate beta diversity
        else {
          var num_habitat_pixels = (+ reduce B[1..(d_size-2)]) : real(64);
          var habitat_frac = num_habitat_pixels / Mask_Size;

          var P = B / num_habitat_pixels;

          var beta = + reduce (dissimilarity * outer(P,P));
          Output[center,point] = (habitat_frac * beta + eps) : real(64);
        }
      }
  }

}


proc main(args: [] string) {

  var t : stopwatch;
  t.start();

  // Gather input variables from command line
  const radius = (sqrt(window_size) / 2) : int;
  const nx = (radius / dx) : int;
  writeln("Distance circle has a radius of ", nx, " points.");

  // Open the text file that contains the number of rows and columns for the image.
  var infile = open(in_name + "_" + map_type + ".txt", ioMode.r);
  var reader = infile.reader(locking=false);
  // Read the number of rows and columns in the array in from the file.
  var rows = reader.read(int);
  var cols = reader.read(int);

  const ImageSpace = {0..<rows, 0..<cols};
  var Image : [ImageSpace] int(8);

  // Read in array
  //var f = open(in_array, ioMode.r);
  var f = open(in_name + "_" + map_type + ".bin", ioMode.r);
  var r = f.reader(deserializer=new binaryDeserializer(), locking=false);

  // Read in dissimilarity coefficients
  var dissimilarity_file = map_type + ".txt";
  var (dissimilarity, d_size) = ReadArray(dissimilarity_file);

  // Shift the domain so that it starts at 0
  var d_domain = dissimilarity.domain.translate(-1);

  // Create distance mask
  var (LeftMask, CenterMask, RightMask, Mask_Size) = create_distance_mask(radius, dx, nx);

  // Create Block distribution of interior of PNG
  const offset = nx; // maybe needs to be +1 to account for truncation?
  const Inner = ImageSpace.expand(-offset);
  const myTargetLocales = reshape(Locales, {1..Locales.size, 1..1});
  const D = blockDist.createDomain(Inner, targetLocales=myTargetLocales);
  var OutputArray : [D] real(64);

  // Create NetCDF
  var varid : int;
  var Ha = window_size / 10000 / (4.0 / pi);
  var zero_pad = 3 - (Ha : string).size;
  var nonzero : string = "%3.1dr".format(Ha);
  var Ha_string : string = (zero_pad*"0") + nonzero;

  var out_file = in_name + "_" + map_type + "_" + Ha_string + "Ha.nc";
//  CreateNetCDF(out_file, ImageSpace, varid);

  writeln("Starting coforall loop.");

//////////////////////////////////////////////////////////////////////////

  coforall loc in Locales with (ref OutputArray) do on loc {

    const loc_d_size = d_size;
    const loc_Mask_Size = Mask_Size;

    var locD = D.localSubdomain();
    var locD_plus = locD.expand(offset);
    var locImage : [locD_plus] int(8);

    // Read in array
    var f = open(in_name + "_" + map_type + ".bin", ioMode.r);
    var first_point = locD_plus.first[0]*locD_plus.shape[1] + locD_plus.first[1];
    var r = f.reader(deserializer=new binaryDeserializer(), region=first_point.., locking=false);

    for i in locD_plus.first[0]..locD_plus.last[0] {
      for j in locD_plus.first[1]..locD_plus.last[1] {
        var tmp : int(8);
        r.readBinary(tmp);
        locImage[i,j] = tmp;
      }
    }
    r.close();

    const locLeftMaskDomain = LeftMask.domain;
    const locCenterMaskDomain = CenterMask.domain;
    const locRightMaskDomain = RightMask.domain;

    const locDissDomain = d_domain;
    const locDiss : [locDissDomain] dissimilarity.eltType = dissimilarity;

    convolve_and_calculate(locImage, D.localSubdomain(), locLeftMaskDomain, locCenterMaskDomain, locRightMaskDomain, locDiss, OutputArray, loc_d_size, loc_Mask_Size, t);

  }

//  WriteOutput(out_file, OutputArray, varid);

  if printReduce then
    writeln("Sum reduce of OutputArray: ", (+ reduce OutputArray));
}
