/* c-ray.chpl - a simple raytracing filter.
 *
 * Based on c-ray-f.c, developed by John Tsiombikas
 * Ported to Chapel and parallelized by Brad Chamberlain,
 *   September/November 2016
 * Updated October 2018 to support distributed execution better.
 *
 * ----------------------------------------------------------------------------
 * Usage:
 *   compile:  chpl c-ray.chpl -o c-ray  # add --fast for performance runs
 *   run:      ./c-ray  # reads from 'scene', writes to 'image.bmp' by default
 *     or:     ./c-ray --scene myscene --image myimage.ppm  # override defaults
 *     or:     cat scene | ./c-ray --scene stdin --image stdout > out.ppm
 *     or:     ./c-ray --usage  # for further options
 *   enjoy:    open image.bmp (with any standard image browser)
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

use Image;    // use helper module related to writing out images
use List;
use IO;       // allow use of stderr, stdin, iomode

//
// =================================================
// Module-level constant / param / type declarations
// =================================================
//

//
// Configuration constants
// (Override defaults on executable line using --<cfg> <val> or --<cfg>=<val>)
//
config const size = "800x600",            // size of output image
             samples = 1,                 // number of rays sampled per pixel
             scene = "built-in",          // input scene filename, or stdin
             image = "image.bmp",         // output image filename, or stdout
             imgType = extToFmt(image),   // the image file format
             usage = false,               // print usage message?

             fieldOfView = quarter_pi,    // field of view in radians
             maxRayDepth = 5,             // raytrace recursion limit
             rayMagnitude = 1000.0,       // trace rays of this magnitude
             errorMargin = 1e-6,          // margin to avoid surface acne

             noTiming = false,            // print rendering times?

             useCRand = false,            // use C rand() (vs. Chapel PCG)?
             seed = 0;                    // if non-zero, use as the RNG 'seed'

//
// Print the usage information, if requested.
//
if usage then printUsage();

//
// Establish config-dependent constants.
//
const (xres, yres) = computeDims(size);

const rcpSamples = 1.0 / samples,         // the reciprocal of the # of samples
      halfFieldOfView = fieldOfView / 2;  // compute half the field-of-view

//
// Set params that symbolically represent dimensions using 1-based indexing
//
param X = 0,
      Y = 1,
      Z = 2,
      numdims = 3;

//
// Declare types used within the program
//
type vec3 = numdims*real;       // a 3-tuple for positions, vectors

param nran = 1024;

record randNums {  // random numbers used for jittering
  var urand: [0..#nran] vec3,
      irand: [0..#nran] int;
}

record sceneType {
  var lights = new list(vec3), // a list of light positions

      camera: cameraType,       // the camera

      objects = new list(sphere); // a list of sphere objects
}

record cameraType {
  var pos,            // position
      targ: vec3,     // target
      fov: real;      // field-of-view
}

record sphere {
  var pos: vec3,      // position
      rad: real,      // radius
      mat: material;  // material
}

record material {
  var color: vec3,    // color
      spow,           // specular power
      refl: real;     // reflection intensity
}

record ray {
  var orig,           // origin
      dir: vec3;      // direction
}

record spoint {     // a surface point
  var pos,            // position
      normal,         // normal
      vref: vec3,     // view reflection
      dist: real;     // parametric distance of intersection along the ray
}

//
// =================================
// main(): The program's entry point
// =================================
//
proc main() {
  use BlockDist;

  //
  // A local domain, distributed domain, and array representing the image
  //
  const imageSize = {0..#yres, 0..#xres};
  const pixelPlane = imageSize dmapped Block(imageSize);
  var pixels: [pixelPlane] pixelType;

  //
  // Set up the random numbers and scene
  //
  var rands = initRands();
  var scene = loadScene();

  //
  // Timers to measure the rendering time
  //
  use Time;
  var t: Timer;
  t.start();

  //
  // The main loop that computes the image in parallel.
  //
  forall (y, x) in pixelPlane {
    pixels[y, x] = computePixel(y, x, scene, rands);
  }

  //
  // Check the timer
  //
  const rendTime = t.elapsed();
  
  //
  // Print the elapsed time and communications to 'stderr' (just in
  // case the user is printing the image to 'stdout').
  //
  if !noTiming then
    stderr.writef("Rendering took: %r seconds (%r milliseconds)\n",
                  rendTime, rendTime*1000);

  //
  // Write out the image
  //
  writeImage(image, imgType, pixels);
}

//
// ==============
// computePixel()
// ==============
//
// Given the (y, x) coordinates of a pixel, computePixel() returns the
// color value computed for the pixel as a 'pixelType'.  Given the two
// overloads below, the (y, x) coordinates can either be passed in as
// a tuple of ints or as two separate int arguments.
//
// For each subpixel, trace a ray through the scene, accumulate the
// colors of the subpixels of each pixel, then pack the color and put
// it into the framebuffer.
//
proc computePixel(yx: 2*int, scene, rands): pixelType {
  return computePixel((...yx), scene, rands);  // expand the tuple 'yx'
}

proc computePixel(y: int, x: int, scene, rands): pixelType {
  var rgb: vec3;

  for s in 0..#samples do
    rgb += trace(getPrimaryRay((x,y), s, scene.camera, rands), scene);

  rgb *= rcpSamples;

  return realColorToInt(red) | realColorToInt(green) | realColorToInt(blue);

  inline proc realColorToInt(param color) {
    const colorAsInt = colorMask & ((min(rgb(color), 1.0) * 255.0): pixelType);
    return colorAsInt << colorOffset(color);
  }
}

//
// =============================
// Helper routines for rendering
// =============================
//

//
// Determine the primary ray corresponding to the specified pixel xy
//
proc getPrimaryRay(xy, sample, camera, rands) {
  var k = camera.targ - camera.pos;
  normalize(k);
  const i = crossProduct((0.0, 1.0, 0.0), k),
        j = crossProduct(k, i);

  const m: [0..#numdims] vec3 = [i, j, k];

  var pRay = new ray();
  (pRay.dir(X), pRay.dir(Y)) = getSamplePos(xy, sample, rands);
  pRay.dir(Z) = 1.0 / halfFieldOfView;
  pRay.dir *= rayMagnitude;

  const dir = pRay.dir + pRay.orig;

  pRay.orig = dot(pRay.orig, m) + camera.pos;
  pRay.dir = dot(dir, m) + pRay.orig;

  return pRay;
}

//
// Trace a ray through the scene recursively (the recursion happens
// through shade() to calculate reflection rays if necessary).
//
proc trace(ray, scene, depth=0): vec3 {
  // if we've reached the recursion limit, bail out
  if depth >= maxRayDepth then
    return (0.0, 0.0, 0.0);

  // find the nearest intersection...
  var foundNearest = false,
      nearestObj: sphere,
      nearestSp: spoint;

  for obj in scene.objects {
    const (hit, sp) = raySphere(obj, ray);
    if hit && (!foundNearest || sp.dist < nearestSp.dist) {
      foundNearest = true;
      nearestObj = obj;
      nearestSp = sp;
    }
  }

  // and perform shading calculations as needed by calling shade()
  if foundNearest then
    return shade(nearestObj, nearestSp, depth, scene);
  else
    return (0.0, 0.0, 0.0);
}

//
// Convert pixel coordinates 'xy' into a 2D point 'pt' in scene space
//
proc getSamplePos(xy, sample, rands) {
  var pt = xy / (xres: real, yres: real);
  pt -= (0.5, 0.65);
  pt(Y) = -pt(Y);

  if sample {
    const sf = 2.0 / xres;
    pt += jitter(xy, sample, rands) * sf;
  }

  const aspect = xres:real / yres;  // image aspect ratio
  pt(Y) /= aspect;

  return pt;
}

//
// Compute jitter values for subsequent samples to the same pixel.
//
proc jitter((x, y), s, rands) {
  param mask = nran - 1;

  return (rands.urand[(x + (y << 2) + rands.irand[(x + s) & mask]) & mask](X),
          rands.urand[(y + (x << 2) + rands.irand[(y + s) & mask]) & mask](Y));
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
// Calculate direct illumination with the phong reflectance model.
// Also handles reflections by calling trace again, if necessary.
//
proc shade(obj, sp, depth, scene) {
  var col: vec3;

  // for all lights...
  for l in scene.lights {
    var ldir = l - sp.pos;
    const shadowRay = new ray(orig = sp.pos, dir = ldir);
    var inShadow = false;

    // shoot shadow rays to determine if we have a line of sight with
    // the light
    for obj in scene.objects {
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
          rcol = trace(rRay, scene, depth + 1);
    col += rcol * obj.mat.refl;
  }

  return col;
}

//
// =======================
// General helper routines
// =======================
//

//
// Print usage information
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
  writeln("  --imgType [ppm|bmp]   override the default image file type");
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
// Turn a "WxH" string into a 2-tuple of integers: (W, H)
//
proc computeDims(sizeString) {
  const sizeTup = sizeString.partition("x"); // make into 3-tuple ("W","x","H")

  if (sizeTup.size != 3 || sizeTup[1] != "x") then
    halt("--s option requires argument to be in WxH format");

  return (sizeTup[0]:int, sizeTup[2]:int);
}

//
// Initialize the random number tables for the jitter using either C
// rand() (because the reference version does) or Chapel rand (because
// its results are portable, and it can optionally be used in parallel).
//
proc initRands() {
  use SysCTypes;

  var rands: randNums;
  
  if useCRand {
    // extern declarations of C's random number generators.
    extern const RAND_MAX: c_int;
    extern proc rand(): c_int;
    extern proc srand(seed: c_uint);

    if seed then
      srand(seed.safeCast(c_uint));
    for u in rands.urand do
      u(X) = rand():real / RAND_MAX - 0.5;
    for u in rands.urand do
      u(Y) = rand():real / RAND_MAX - 0.5;
    for r in rands.irand do
      r = (nran * (rand():real / RAND_MAX)): int;
  } else {
    use Random;

    var rng = new owned RandomStream(seed=(if seed then seed
                                                   else SeedGenerator.currentTime),
                                     eltType=real);
    for u in rands.urand do
      u(X) = rng.getNext() - 0.5;
    for u in rands.urand do
      u(Y) = rng.getNext() - 0.5;
    for r in rands.irand do
      r = (nran * rng.getNext()): int;
  }

  return rands;
}

//
// Load the scene from an extremely simple scene description file
//
proc loadScene() {
  var newScene: sceneType;
  //
  // Support a built-in scene in order to avoid file input, should it
  // be problematic in any way.
  //
  if scene == "built-in" {
    newScene.objects.append(new sphere((-1.5, -0.3, -1), 0.7,
                                          new material((1.0, 0.2, 0.05), 50.0,
                                                       0.3)));
    newScene.objects.append(new sphere((1.5, -0.4, 0), 0.6,
                                          new material((0.1, 0.85, 1.0), 50.0,
                                                       0.4)));
    newScene.objects.append(new sphere((0, -1000, 2), 999,
                                          new material((0.1, 0.2, 0.6), 80.0,
                                                       0.8)));
    newScene.objects.append(new sphere((0, 0, 2), 1,
                                          new material((1.0, 0.5, 0.1), 60.0,
                                                       0.7)));
    newScene.lights.append((-50, 100, -50));
    newScene.lights.append((40, 40, 150));
    newScene.camera = new cameraType((0, 6, -17), (0, -1, 0), 45);

    return newScene;
  }

  //
  // Otherwise, read the scene from 'infile'
  //

  // the input file channel
  const infile = if scene == "stdin" then stdin
                                     else open(scene, iomode.r).reader();

  // a map (associative array) from the supported input file argument
  // types to the number of columns of input they expect
  const expectedArgs = ['l'=>4, 'c'=>8, 's'=>10];

  // loop over the lines from the input file, counting them
  for (rawLine, lineno) in zip(infile.lines(), 1..) {
    // drop any comments (text following '#')
    const linePlusComment = rawLine.split('#', maxsplit=1, ignoreEmpty=false),
          line = linePlusComment[1];

    // split the line into its whitespace-separated strings
    const columns = line.split();
    if columns.size == 0 then continue;

    // grab the input type
    const inType = columns[1];

    // handle error conditions
    if !expectedArgs.domain.contains(inType) then
      inputError("unexpected input type: " + inType);
    else if columns.size < expectedArgs(inType) then
      inputError("not enough arguments for input of type '" + inType + "'");
    else if columns.size > expectedArgs(inType) then
      inputError("too many arguments for input of type '" + inType + "'");

    // grab the position columns
    const pos = (columns[2]:real, columns[3]:real, columns[4]:real);

    // if this is a light, store it as such
    if inType == 'l' {
      newScene.lights.append(pos);
      continue;
    }

    // grab the radius/field-of-view and color/target columns
    const rad = columns[5]:real,
          col = (columns[6]:real, columns[7]:real, columns[8]:real);

    // if this is the camera, store it
    if inType == 'c' {
      newScene.camera.pos = pos;
      newScene.camera.targ = col;
      newScene.camera.fov = rad;
      continue;
    }

    // grab the shininess and reflectivity columns
    const spow = columns[9]: real,
          refl = columns[10]: real;

    // this must be a sphere, so store it
    newScene.objects.append(new sphere(pos, rad,
                                          new material(col, spow, refl)));

    // helper routine for printing errors in the input file
    proc inputError(msg) {
      stderr.writeln(newScene, ":", lineno, ": ", msg);
      exit(1);
    }
  }

  return newScene;
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
