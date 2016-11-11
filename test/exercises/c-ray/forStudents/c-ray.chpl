/* c-ray.chpl - a simple raytracing filter.
 *
 * Based on c-ray-f.c, developed by John Tsiombikas
 * Ported to Chapel and parallelized by Brad Chamberlain,
 *   September/November 2016
 *
 * ----------------------------------------------------------------------------
 * Usage:
 *   compile:  chpl c-ray.chpl -o c-ray  # add --fast for performance runs
 *   run:      ./c-ray  # reads from 'scene', writes to 'image.ppm' by default
 *     or:     ./c-ray --scene myscene --image myimage.ppm  # override defaults
 *     or:     cat scene | ./c-ray --scene stdin --image stdout > out.ppm
 *     or:     ./c-ray --usage  # for further options
 *   enjoy:    emacs foo.ppm (with a modern non-terminal version of emacs)
 * ----------------------------------------------------------------------------
 * Scene file format:
 *   # sphere (many)
 *   s  x y z  rad   r g b   shininess   reflectivity
 *   # light (many)
 *   l  x y z
 *   # camera (one)
 *   c  x y z  fov   tx ty tz
 * ----------------------------------------------------------------------------
 */

//
// STUDENTS: Browse through the code below to your heart's content.
// When you're ready to start coding, jump down to main() and read
// through the comments labeled STEP 0, STEP 1, STEP 2, etc.  These
// will lead you through a number of coding steps and transformations
// that will exercise Chapel data parallel and base language features.
//

//
// Configuration params/types
// (Override defaults on compiler line using -s<cfg>=<val>)
//
config type pixelType = int;

config param bitsPerColor = 8;

//
// Configuration constants
// (Override defaults on executable line using --<cfg> <val> or --<cfg>=<val>)
//
config const size = "800x600",            // size of output image
             samples = 1,                 // number of rays sampled per pixel
             scene = "scene",             // input scene filename, or stdin
             image = "image.ppm",         // output image filename, or stdout
             usage = false,                // print usage message?

             fieldOfView = quarter_pi,    // field of view in radians
             maxRayDepth = 5,             // raytrace recurion limit
             rayMagnitude = 1000.0,       // trace rays of this magnitude
             errorMargin = 1e-6,          // margin to avoid surface acne

             noTiming = false,            // print rendering times?

             useCRand = false,            // use C rand() (vs. Chapel PCG)?
             seed = 0;                    // if non-zero, use as the RNG 'seed'

//
// Define config-dependent types and params.
//
param colorMask = (0x1 << bitsPerColor) - 1;

type colorType = uint(bitsPerColor);

//
// Process config-dependent constants.
//
if usage then printUsage();

const ssize = size.partition("x");        // split size string into components

if (ssize.size != 3 || ssize(2) != "x") then
  halt("--s option requires argument to be in WxH format");

const xres = ssize(1):int,         // x- and y-resolutions of the image
      yres = ssize(3):int;

const rcpSamples = 1.0 / samples;  // the reciprocal of the # of samples

//
// the input and output file channels
//
const infile = if scene == "stdin" then stdin
                                   else open(scene, iomode.r).reader(),
      outfile = if image == "stdout" then stdout
                                     else open(image, iomode.cw).writer();

const halfFieldOfView = fieldOfView / 2;  // compute half the field-of-view

//
// set params for colors and dimensions
//

param red = 1,        // names for referring to colors
      green = 2,
      blue = 3,
      numColors = 3;

param X = 1,          // names for accessing vec3 elements
      Y = 2,
      Z = 3,
      numdims = 3;

//
// Verify that config-specified pixelType is appropriate for storing colors
//
if (isIntegral(pixelType)) {
  if (numColors*bitsPerColor > numBits(pixelType)) then
    compilerError("pixelType '" + pixelType:string +
                  "' isn't big enough to store " +
                  bitsPerColor + " bits per color");
} else {
  compilerError("pixelType must be an integral type");
}



//
// establish types
//

type vec3 = numdims*real;   // a 3-tuple for positions, vectors

record ray {
  var orig,           // origin
      dir: vec3;      // direction
}

record material {
  var color: vec3,    // color
      spow,           // specular power
      refl: real;     // reflection intensity
}

class sphere {
  var pos: vec3,      // position
      rad: real,      // radius
      mat: material;  // material
}

record spoint {     // a surface point
  var pos,            // position
      normal,         // normal
      vref: vec3,     // view reflection
      dist: real;     // parametric distance of intersection along the ray
}

record camera {
  var pos,            // position
      targ: vec3,     // target
      fov: real;      // field-of-view
}

//
// variables used to store the scene
//
var objects: [1..0] sphere,  // the scene's spheres; start with an empty array
    lights: [1..0] vec3,     // the scene's lights;  "
    cam: camera;             // camera (there will be only one)

//
// arrays for storing random numbers
//
param nran = 1024;

var urand: [0..#nran] vec3,
    irand: [0..#nran] int;

//
// The program's entry point
//
proc main() {

  //
  // STEP 0: Compile and run the code as it is.  You should get a
  // small black square image in the resulting image.ppm file.  View
  // the image file in gimp, or your favorite image viewer, or a
  // modern version of emacs running with its own window.
  //
  // STEP 1: Declare an array (and optionally a domain for it) here to
  // describe the image of pixels to render.  The array should store
  // 'yres' x 'xres' pixel elements of type 'pixelType' (a
  // configurable type alias defined above).
  //
  // STEP 2: Pass your array into the writeImage() call towards the
  // end of this routine, replacing the 'dummy' array (which is
  // provided here just to make the code compile out of the box).
  //

  loadScene();
  initRands();

  use Time;       // Bring in timers to measure the rendering time
  var t: Timer;
  t.start();

  //
  // STEP 3: Within these timer calls, fill in your array's values via
  // calls to 'computePixel()' (defined below).  Start by trying a
  // serial loop.  Do you get a reasonable image?  Try the 'sphfract'
  // scene file as well...
  //
  // Step 4: Try experimenting with setting other configuration
  // options on the command-line to see if things work as expected.
  // See the list of options by searching on 'config' above, or by
  // running the program with the --usage flag.
  // 
  // Step 5: Time how long the rendering takes.  Recompile with the
  // --fast flag (intended for performance runs, once a program is
  // working) and retime.  Note these timings for future reference.
  //
  // STEP 6: Now try switching to a parallel loop and make sure your
  // code still produces the right image.  What kind of speed
  // improvements do you see over the serial loop?  With or without
  // --fast?
  //
  // STEP 7 (optional): As a challenge, can you write this computation
  // with promotion?  Does it affect the speed at all?
  //
  // STEP 8 (optional): Ray tracing can be notoriously poorly load
  // balanced since some pixels result in far more pixel bounces than
  // others.  Can you achieve a speed improvement by applying the
  // dynamic() iterator from the DynamicIters module:
  // http://chapel.cray.com/docs/latest/modules/standard/DynamicIters.html
  // Do you need to create a more load-imbalanced scene (or increase
  // the value of 'maxRayDepth' in order to see a noticeable
  // difference?
  //
  // STEP 9 (intended for the afternoon session): Make your array a
  // distributed array (if you implemented STEP 8, note that the
  // dynamic iterators don't yet work well with distributed
  // domains/arrays).  Do you see overhead relative to your previous
  // timings due to the additional complexity of distributed arrays?
  // Run using multiple locales using the provided Cray accounts.  Do
  // you see speedups as you increase the number of locales?
  //

  const rendTime = t.elapsed();

  if !noTiming then
    stderr.writef("Rendering took: %r seconds (%r milliseconds)\n",
                  rendTime, rendTime*1000);

  var dummy: [1..100, 1..100] int;
  writeImage(dummy);

  for obj in objects do
    delete obj;
}

//
// print usage information
//
proc printUsage() {
  writeln("Usage: c-ray [options]");
  writeln("  Reads a scene file, writes a ray-traced image file, " +
          "prints timing to stderr.");
  writeln();
  writeln("Primary Options:");
  writeln("  --size <w>x<h>        plot an image of 'w'idth x 'h'eight pixels");
  writeln("  --samples <rays>      antialias using 'rays' samples per pixel");
  writeln("  --scene <file>        read scene from 'file' (can be 'stdin')");
  writeln("  --image <file>        write image to 'file' (can be 'stdout')");
  writeln("  --usage               print this usage information");
  writeln();
  writeln("Other options:");
  writeln("  --fieldOfView <fov>   use a field-of-view of 'fov' radians");
  writeln("  --maxRayDepth <max>   rays will reflect no more than 'max' times");
  writeln("  --rayMagnitude <mag>  trace rays with magnitude 'mag'");
  writeln("  --errorMargin <err>   avoid surface acne via error margin 'err'");
  writeln("  --noTiming            don't print timing information");
  writeln("  --useCRand            use C's rand() rather than Chapel's RNG");
  writeln("  --seed                a seed for the RNG, 0 == 'don't care'");

  exit(0);
}

//
// Load the scene from an extremely simple scene description file
//
proc loadScene() {
  const expectedArgs = ['l'=>4, 'c'=>8, 's'=>10];

  for (rawLine, lineno) in zip(infile.readlines(), 1..) {
    // drop any comments (text following '#')
    const linePlusComment = rawLine.split('#', maxsplit=1, ignoreEmpty=false),
          line = linePlusComment[1];

    // split the line into its whitespace-separated strings
    const columns = line.split();
    if columns.size == 0 then continue;

    // grab the input type
    const inType = columns[1];

    // handle error conditions
    if !expectedArgs.domain.member(inType) then
      inputError("unexpected input type: " + inType);
    else if columns.size < expectedArgs(inType) then
      inputError("not enough arguments for input of type '" + inType + "'");
    else if columns.size > expectedArgs(inType) then
      inputError("too many arguments for input of type '" + inType + "'");

    // grab the position columns
    const pos = (columns[2]:real, columns[3]:real, columns[4]:real);

    // if this is a light, store it as such
    if inType == 'l' {
      lights.push_back(pos);
      continue;
    }

    // grab the radius/field-of-view and color/target columnsx
    const rad = columns[5]:real,
          col = (columns[6]:real, columns[7]:real, columns[8]:real);

    // if this is the camera, store it
    if inType == 'c' {
      cam.pos = pos;
      cam.targ = col;
      cam.fov = rad;
      continue;
    }

    // grab the shininess and reflectivity columns
    const spow = columns[9]: real,
          refl = columns[10]: real;

    // this must be a sphere, so store it
    objects.push_back(new sphere(pos, rad, new material(col, spow, refl)));

    // helper routine for printing errors in the input file
    proc inputError(msg) {
      stderr.writeln(scene, ":", lineno, ": ", msg);
      exit(1);
    }
  }
}

//
// initialize the random number tables for the jitter using either C
// rand() (because the reference version does) or Chapel rand (because
// its results are portable, and it can optionally be used in parallel).
//
proc initRands() {
  if useCRand {
    // extern declarations of C's random number generators.
    extern const RAND_MAX: c_int;
    extern proc rand(): c_int;
    extern proc srand(seed: c_uint);

    if seed then
      srand(seed.safeCast(c_uint));
    for u in urand do
      u(X) = rand():real / RAND_MAX - 0.5;
    for u in urand do
      u(Y) = rand():real / RAND_MAX - 0.5;
    for r in irand do
      r = (nran * (rand():real / RAND_MAX)): int;
  } else {
    use Random;

    var rng = new RandomStream(seed=(if seed then seed
                                             else SeedGenerator.currentTime),
                               eltType=real);
    for u in urand do
      u(X) = rng.getNext() - 0.5;
    for u in urand do
      u(Y) = rng.getNext() - 0.5;
    for r in irand do
      r = (nran * rng.getNext()): int;
  }
}

//
// Given the (y, x) coordinates of a pixel and return a pixel color
// value as 'pixelType'.  For each subpixel, trace a ray through the
// scene, accumulate the colors of the subpixels of each pixel, then
// pack the color and put it into the framebuffer.
//
proc computePixel(y: int, x: int): pixelType {
  var rgb: vec3;

  for s in 0..#samples do
    rgb += trace(getPrimaryRay((x,y), s));

  rgb *= rcpSamples;

  return realColorToInt(red) | realColorToInt(green) | realColorToInt(blue);

  inline proc realColorToInt(param color) {
    const colorAsInt = colorMask & ((min(rgb(color), 1.0) * 255.0): pixelType);
    return colorAsInt << colorOffset(color);
  }
}

proc computePixel(yx: 2*int): pixelType {
  return computePixel((...yx));  // expand the tuple 'yx'
}

//
// write the image to the output file
//
proc writeImage(pixels) {
  outfile.writeln("P6");
  outfile.writeln(pixels.domain.dim(1).size, " ", pixels.domain.dim(2).size);
  outfile.writeln(255);
  for p in pixels do
    for param c in 1..numColors do
      outfile.writef("%|1i", ((p >> colorOffset(c)) & colorMask));
}

//
// how far to shift a color component when packing into a pixelType
//
inline proc colorOffset(param color) param {
  return (color - 1) * bitsPerColor;
}

//
// determine the primary ray corresponding to the specified pixel xy
//
proc getPrimaryRay(xy, sample) {
  var k = cam.targ - cam.pos;
  normalize(k);
  const i = crossProduct((0.0, 1.0, 0.0), k),
        j = crossProduct(k, i);

  const m: [1..numdims] vec3 = [i, j, k];

  var pRay = new ray();
  (pRay.dir(X), pRay.dir(Y)) = getSamplePos(xy, sample);
  pRay.dir(Z) = 1.0 / halfFieldOfView;
  pRay.dir *= rayMagnitude;

  const dir = pRay.dir + pRay.orig;

  pRay.orig = dot(pRay.orig, m) + cam.pos;
  pRay.dir = dot(dir, m) + pRay.orig;

  return pRay;
}

//
// trace a ray throught the scene recursively (the recursion happens
// through shade() to calculate reflection rays if necessary).
//
proc trace(ray, depth=0): vec3 {
  // if we've reached the recursion limit, bail out
  if depth >= maxRayDepth then
    return (0.0, 0.0, 0.0);

  // find the nearest intersection...
  var nearestObj: sphere,
      nearestSp: spoint;

  for obj in objects {
    const (hit, sp) = raySphere(obj, ray);
    if hit && (nearestObj == nil || sp.dist < nearestSp.dist) {
      nearestObj = obj;
      nearestSp = sp;
    }
  }

  // and perform shading calculations as needed by calling shade()
  if nearestObj then
    return shade(nearestObj, nearestSp, depth);
  else
    return (0.0, 0.0, 0.0);
}

//
// Convert pixel coordinates 'xy' into a 2D point 'pt' in scene space
//
proc getSamplePos(xy, sample) {
  var pt = xy / (xres: real, yres: real);
  pt -= (0.5, 0.65);
  pt(Y) = -pt(Y);

  if sample {
    const sf = 2.0 / xres;
    pt += jitter(xy, sample) * sf;
  }

  const aspect = xres:real / yres;  // image aspect ratio
  pt(Y) /= aspect;

  return pt;
}

//
// compute jitter values for subsequent samples to the same pixel.
//
proc jitter((x, y), s) {
  param mask = nran - 1;

  return (urand[(x + (y << 2) + irand[(x + s) & mask]) & mask](X),
          urand[(y + (x << 2) + irand[(y + s) & mask]) & mask](Y));
}

//
// Calculate ray-sphere intersection, returning (hit, surface point)
// where 'hit' is true/false depending on whether or not there's a hit
// and 'surface point' is the point on the surface if there was a hit.
//
proc raySphere(sph, ray) {
  var sp: spoint;

  const a = ray.dir(X)**2 + ray.dir(Y)**2 + ray.dir(Z)**2,
        b = 2.0 * ray.dir(X) * (ray.orig(X) - sph.pos(X)) +
            2.0 * ray.dir(Y) * (ray.orig(Y) - sph.pos(Y)) +
            2.0 * ray.dir(Z) * (ray.orig(Z) - sph.pos(Z)),
        c = sph.pos(X)**2 + sph.pos(Y)**2 + sph.pos(Z)**2 +
            ray.orig(X)**2 + ray.orig(Y)**2 + ray.orig(Z)**2 +
            2.0 * (-sph.pos(X) * ray.orig(X)
                   -sph.pos(Y) * ray.orig(Y)
                   -sph.pos(Z) * ray.orig(Z))
            - sph.rad**2,
        d = b**2 - 4.0 * a * c;

  if (d < 0.0) then return (false, sp);

  const sqrtD = sqrt(d);
  var t1 = (-b + sqrtD) / (2.0 * a),
      t2 = (-b - sqrtD) / (2.0 * a);

  if (t1 < errorMargin && t2 < errorMargin) || (t1 > 1.0 && t2 > 1.0) then
    return (false, sp);

  if (t1 < errorMargin) then t1 = t2;
  if (t2 < errorMargin) then t2 = t1;

  sp.dist = min(t1, t2);
  sp.pos = ray.orig + ray.dir * sp.dist;
  sp.normal = (sp.pos - sph.pos) / sph.rad;
  sp.vref = reflect(ray.dir, sp.normal);
  normalize(sp.vref);

  return (true, sp);

  // calculate reflection vector
  proc reflect(v, n) {
    return -(2.0 * dot(v, n) * n - v);
  }
}


//
// Calculates direct illumination with the phong reflectance model.
// Also handles reflections by calling trace again, if necessary.
//
proc shade(obj, sp, depth) {
  var col: vec3;

  // for all lights...
  for l in lights {
    var ldir = l - sp.pos;
    const shadowRay = new ray(orig = sp.pos, dir = ldir);
    var inShadow = false;

    // shoot shadow rays to determine if we have a line of sight with
    // the light
    for obj in objects {
      const (hit, sp) = raySphere(obj, shadowRay);
      if (hit) {
        inShadow = true;
        break;
      }
    }

    // and if we're not in shadow, calculate direct illumination with
    // the phong model.
    if (!inShadow) {
      normalize(ldir);
      const idiff = max(dot(sp.normal, ldir), 0.0),
            ispec = if (obj.mat.spow > 0.0)
                      then max(dot(sp.vref, ldir), 0.0) ** obj.mat.spow
                      else 0.0;
      col += idiff * obj.mat.color + ispec;
    }
  }

  // Also, if the object is reflective, spawn a reflection ray, and
  // call trace() to calculate the light arriving from the mirror
  // direction.
  if obj.mat.refl > 0.0 {
    const rRay = new ray(orig = sp.pos, dir = sp.vref * rayMagnitude),
          rcol = trace(rRay, depth + 1);
    col += rcol * obj.mat.refl;
  }

  return col;
}

//
// Simple vector functions
//
inline proc normalize(ref a) {
  a /= sqrt(dot(a, a));
}

inline proc dot(a, b) {
  return a(X)*b(X) + a(Y)*b(Y) + a(Z)*b(Z);
}

inline proc crossProduct(v1, v2) {
  return (v1(Y)*v2(Z) - v1(Z)*v2(Y),
          v1(Z)*v2(X) - v1(X)*v2(Z),
          v1(X)*v2(Y) - v1(Y)*v2(X));
}

iter channel.readlines() {
  var line: string;

  while (infile.readline(line)) do
    yield line;
}
