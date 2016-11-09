/* c-ray.chpl - a simple raytracing filter.
 *
 * Based on c-ray-f.c, developed by John Tsiombikas
 * Ported to Chapel by Brad Chamberlain, September/November 2016
 *
 * -----------------------------------------------------------------------
 * Usage:
 *   compile:  chpl c-ray.chpl -o c-ray  # add --fast for performance runs
 *   run:      ./c-ray         # reads from 'scene', writes to 'image.ppm'
 *     or:     ./c-ray --scene myscene --image myimage.ppm
 *     or:     cat scene | ./c-ray --scene stdin --image stdout > out.ppm
 *     or:     ./c-ray --help            # for further options
 *   enjoy:    emacs foo.ppm (with a modern non-terminal version of emacs)
 * -----------------------------------------------------------------------
 * Scene file format:
 *   # sphere (many)
 *   s  x y z  rad   r g b   shininess   reflectivity
 *   # light (many)
 *   l  x y z
 *   # camera (one)
 *   c  x y z  fov   tx ty tz
 * -----------------------------------------------------------------------
 */

use Time;  // Bring in timers to measure the rendering time


//
// Configuration params
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

             fieldOfView = quarter_pi,    // field of view in radians
             maxRayDepth = 5,             // raytrace recurion limit
             rayMagnitude = 1000.0,       // trace rays of this magnitude
             errorMargin = 1e-6,          // avoids surface acne

             noTiming = false;        // print rendering times?

//
// Define config-dependent types and params.
//
param colorMask = (0x1 << bitsPerColor) - 1;

type colorType = uint(bitsPerColor);


//
// Compute config-dependent constants.
//
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

// TODO: Should be able to use an enum for both of the above:
//   enum colors {red=0, green, blue};
//   use colors;
//   param numColors = colors.size;

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
//
// TODO: Making this an array [0..3] doesn't work -- why?
//

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
proc main(args: [] string) {
  processArgs(args);

  loadScene();
  initRands();

  var t: Timer;
  t.start();

  var pixels: [0..#yres, 0..#xres] pixelType;

  /* render a frame of xsz x ysz dimensions into the provided framebuffer */
  forall (y, x) in pixels.domain do
    pixels[y, x] = computePixel(y, x);

  const rendTime = t.elapsed();

  if !noTiming then
    stderr.writef("Rendering took: %r seconds (%r milliseconds)\n",
                  rendTime, rendTime*1000);

  writeImage(pixels);

  for obj in objects do
    delete obj;
}


//
// process any arguments
//
proc processArgs(args) {
  for i in 1..args.size-1 {
    if (args[i] == "--help" || args[i] == "-h") {
      printUsage();
    } else {
      stderr.writeln("error: unrecognized argument: '", args[i], "'");
      stderr.writeln();
      printUsage(error=true);
    }
  }

  proc printUsage(error=false) {
    const outChannel = if error then stderr else stdout;

    usage("Usage: " + args[0] + " [options]");
    usage("  Reads a scene file, writes a ray-traced image file, " +
          "prints timing to stderr.");
    usage();
    usage("Primary Options:");
    usage("  --size <w>x<h>        plot an image of 'w'idth x 'h'eight pixels");
    usage("  --samples <rays>      antialias using 'rays' samples per pixel");
    usage("  --scene <file>        read scene from 'file' (can be 'stdin')");
    usage("  --image <file>        write image to 'file' (can be 'stdout')");
    usage("  --help / -h           print this help screen");
    usage();
    usage("Other options:");
    usage("  --fieldOfView <fov>   use a field-of-view of 'fov' radians");
    usage("  --maxRayDepth <max>   rays will reflect no more than 'max' times");
    usage("  --rayMagnitude <mag>  trace rays with magnitude 'mag'");
    usage("  --errorMargin <err>   avoid surface acne via error margin 'err'");
    usage("  --noTiming            don't print timing information");

    exit(error);

    proc usage(str = "") {
      outChannel.writeln(str);
    }
  }
}

//
// Read the input scene file
//
proc loadScene() {
  const inputs = {'l', 'c', 's'},
        expectedArgs: [inputs] int = ['l'=>4, 'c'=>8, 's'=>10];

  for (rawLine, lineno) in zip(infile.readlines(), 1..) {
    //
    // drop any comments (text following '#')
    // TODO: string library experts, is there a better way to do this?
    //
    const linePlusComment = rawLine.split('#', maxsplit=1, ignoreEmpty=false),
          line = linePlusComment[1];

    //
    // split the line into its whitespace-separated strings
    //
    const columns = line.split();
    if columns.size == 0 then continue;

    //
    // grab the input type
    //
    const inType = columns[1];

    //
    // handle error conditions
    //
    if !inputs.member(inType) then
      inputError("unexpected input type: " + inType);
    else if columns.size < expectedArgs(inType) then
      inputError("not enough arguments for input of type '" + inType + "'");
    else if columns.size > expectedArgs(inType) then
      inputError("too many arguments for input of type '" + inType + "'");

    //
    // grab the position columns
    //
    const pos = (columns[2]:real, columns[3]:real, columns[4]:real);

    //
    // if this is a light, store it as such
    //
    if inType == 'l' {
      lights.push_back(pos);
      continue;
    }

    //
    // grab the radius/field-of-view and color/target columnsx
    //
    const rad = columns[5]:real,
          col = (columns[6]:real, columns[7]:real, columns[8]:real);

    //
    // if this is the camera, store it
    //
    if inType == 'c' {
      cam.pos = pos;
      cam.targ = col;
      cam.fov = rad;
      continue;
    }

    //
    // grab the shininess and reflectivity columns
    //
    const spow = columns[9]: real,
          refl = columns[10]: real;

    //
    // this must be a sphere, so store it
    //
    objects.push_back(new sphere(pos, rad, new material(col, spow, refl)));

    //
    // helper routine for printing errors in the input file
    //
    proc inputError(msg) {
      stderr.writeln(scene, ":", lineno, ": ", msg);
      exit(1);
    }
  }
}


//
// initialize the random number tables for the jitter
//
proc initRands() {
  //
  // extern declarations of C's random number generators.
  //
  // TODO: The following could be changed to use Chapel's 'Random'
  //   module and to generate random numbers in parallel if the
  //   program is not particularly sensitive to the specific random
  //   number generator used.
  //
  extern const RAND_MAX: c_int;
  extern proc rand(): c_int;

  for u in urand do
    u(X) = rand():real / RAND_MAX - 0.5;
  for u in urand do
    u(Y) = rand():real / RAND_MAX - 0.5;
  for r in irand do
    r = (nran * (rand():real / RAND_MAX)): int;
}


//
// Take in the (y, x) coordinates of a pixel and return a pixel value
//
proc computePixel(y: int, x: int): pixelType {
  var rgb: vec3;

  for s in 0..#samples do
    rgb += trace(getPrimaryRay((x, y), s));

  rgb *= rcpSamples;

  return realColorToInt(red) | realColorToInt(green) | realColorToInt(blue);

  inline proc realColorToInt(param color) {
    const colorAsInt = colorMask & ((min(rgb(color), 1.0) * 255.0): pixelType);
    return colorAsInt << colorOffset(color);
  }
}


//
// write the image to the output file
//
proc writeImage(pixels) {
  outfile.writeln("P6");
  outfile.writeln(xres, " ", yres);
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
  var nearestObj: sphere;
  var nearestSp: spoint;

  /* if we reached the recursion limit, bail out */
  if depth > maxRayDepth then
    return (0.0, 0.0, 0.0);

  /* find the nearest intersection ... */
  //
  // TODO: minloc reduction?
  for obj in objects {
    const (hit, sp) = raySphere(obj, ray);
    if hit {
      if (nearestObj == nil || sp.dist < nearestSp.dist) {
        nearestObj = obj;
        nearestSp = sp;
      }
    }
  }

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
    const sf = 2.0 / xres; // TODO: This really wants to be a local static
    pt += jitter(xy, sample) * sf;
  }

  const aspect = xres:real / yres;  // image aspect ratio; TODO: local static
  pt(Y) /= aspect;

  return pt;
}


//
// compute jitter values for subsequent samples to the same pixel
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

  // TODO: simplify this
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

  if (d < 0.0) then return (0, sp);

  const sqrtD = sqrt(d);
  var t1 = (-b + sqrtD) / (2.0 * a),
      t2 = (-b - sqrtD) / (2.0 * a);

  // TODO: simplify?
  if (t1 < errorMargin && t2 < errorMargin) || (t1 > 1.0 && t2 > 1.0) then
    return (0, sp);

  if (t1 < errorMargin) then t1 = t2;
  if (t2 < errorMargin) then t2 = t1;

  sp.dist = min(t1, t2);
  sp.pos = ray.orig + ray.dir * sp.dist;
  sp.normal = (sp.pos - sph.pos) / sph.rad;
  sp.vref = reflect(ray.dir, sp.normal);
  normalize(sp.vref);

  return (1, sp);

  proc reflect(v, n) {
    return -(2.0 * dot(v, n) * n - v);
  }
}


proc shade(obj, sp, depth) {
  var col: vec3;  // TODO: reduction?

  for l in lights {
    var ldir = l - sp.pos;
    const shadowRay = new ray(orig = sp.pos, dir = ldir);
    var inShadow = false;
    //
    // TODO: use reduction
    //
    // shoot shadow rays to determine if we have a line of sight with
    // the light
    //
    for obj in objects {
      const (hit, sp) = raySphere(obj, shadowRay);
      if (hit) {
        inShadow = true;
        break;
      }
    }

    if (!inShadow) {
      normalize(ldir);
      const idiff = max(dot(sp.normal, ldir), 0.0),
            ispec = if (obj.mat.spow > 0.0)
                      then max(dot(sp.vref, ldir), 0.0) ** obj.mat.spow
                      else 0.0;
      col += idiff * obj.mat.color + ispec;
    }
  }

  /* Also, if the object is reflective, spawn a reflection ray, and
   * call trace() to calculate the light arriving from the
   * mirror direction.
   */
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


//
// TODO: Add this to the IO module
//
iter channel.readlines() {
  var line: string;

  while (infile.readline(line)) do
    yield line;
}
