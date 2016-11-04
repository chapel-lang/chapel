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

use Time;

//
// Configuration constants
// Set on command line via --<cfg> <val> or --<cfg>=<val>.
//
config const size = "800x600",            // size of output image
             rays = 1,                    // number of rays per pixel
             scene = "scene",             // input scene filename, or stdin
             image = "image.ppm",         // output image filename, or stdout

             rayMagnitude = 1000.0,       // trace rays of this magnitude
             maxRayDepth = 5,             // raytrace recurion limit
             fieldOfView = quarter_pi,    // field of view in radians
             errorMargin = 1e-6,          // avoids surface acne

             printTiming = true;          // print rendering times?

//
// Compute values that are dependent on configs
//
const ssize = size.partition("x");        // split size into components

if (ssize.size != 3 || ssize(2) != "x") then
  halt("--s option requires argument to be in WxH format");

const xres = ssize(1):int,                // grab x- and y-resolutions
      yres = ssize(3):int,
      aspect = xres:real / yres;          // aspect ratio of the image

const halfFieldOfView = fieldOfView / 2;  // compute half the field-of-view


//
// Define types and params (compile-time constants) for use later
//

param redShift = 16,                      // bit shift amounts for each color
      greenShift = 8,
      blueShift = 0;

type vec3 = 3*real;                       // a 3-tuple for positions, vectors

param X = 1,                              // names for accessing vec3 elements
      Y = 2,
      Z = 3;

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

record spoint {
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
// variables used by the computation
//

var objects: [1..0] sphere,  // the scene's spheres; start with an empty list
    lights: [1..0] vec3,     // the scene's lights
    cam: camera;             // camera (there will be only one)

param nran = 1024,
      mask = nran - 1;

var urand: [0..#nran] vec3,
    irand: [0..#nran] int;

proc main(args: [] string) {
  if (args.size > 1) then usage(args);

  var pixels: [0..#yres, 0..#xres] uint(32);
  const infile = if scene == "stdin" then stdin
                                     else open(scene, iomode.r).reader(),
        outfile = if image == "stdout" then stdout
                                       else open(image, iomode.cw).writer();

  loadScene(infile);

  /* initialize the random number tables for the jitter */
  //
  // TODO: If specifics of random numbers don't matter, could switch to PCG
  // and gen in parallal?
  //
  {
    extern const RAND_MAX: c_int;
    extern proc rand(): c_int;

    for u in urand do
      u(X) = rand():real / RAND_MAX - 0.5;
    for u in urand do
      u(Y) = rand():real / RAND_MAX - 0.5;
    for r in irand do
      r = (nran * (rand():real / RAND_MAX)): int;
  }

  var t: Timer;
  t.start();
  render(pixels, rays);
  const rendTime = t.elapsed();

  if printTiming then
    stderr.writeln("Rendering took: ", rendTime, " seconds (", rendTime*1000,
                   " milliseconds)");

  outfile.writeln("P6");
  outfile.writeln(xres, " ", yres);
  outfile.writeln(255);
  for p in pixels {
    outfile.writef("%|1i", ((p >> redShift)   & 0xff):uint(8));
    outfile.writef("%|1i", ((p >> greenShift) & 0xff):uint(8));
    outfile.writef("%|1i", ((p >> blueShift)  & 0xff):uint(8));
  }
}

proc usage(args) {
  for i in 1..args.size-1 {
    if (args[i] == "--help" || args[i] == "-h") {
      writeln("Usage: ", args[0], " [options]");
      writeln(" Reads a scene file, writes a ray-traced image file, prints timing to stderr.");
      writeln();
      writeln("Main Options:");
      writeln("  --size WxH     where W is the width and H the height of the image");
      writeln("  --rays <rays>  shoot <rays> rays per pixel (antialiasing)");
      writeln("  --scene <file> read scene from <file> (can be 'stdin')");
      writeln("  --image <file> write image to <file> (can be 'stdout')");
      writeln("  --help         this help screen");
      writeln();
      writeln("Other options:");
      writeln("  --rayMagnitude <mag>  trace rays of this magnitude");
      writeln("  --maxRayDepth <max>   the number of times a ray can reflect");
      writeln("  --fieldOfView <fov>   the field-of-view in radians");
      writeln("  --errorMargin <err>   an error margin to avoid surface acne");
      writeln();
      exit(0);
    } else {
      stderr.writeln("unrecognized argument: ", args[i]);
      exit(1);
    }
  }
}

proc loadScene(infile) {
  var line: string;
  param delim = " \t\n";

  while (infile.readline(line)) {
    var comment = false;
    for c in line {
      if c == '#' {
        comment = true;  // TODO: use a labeled break?
        break;
      }
      if (c != ' ' || c != '\t') then break;
    }
    if comment then continue;

    const substrs = line.split();
    if substrs.size == 0 then continue;

    const intype = substrs[1];
    const pos: vec3 = (substrs[2]:real, substrs[3]:real, substrs[4]:real);
    if intype == 'l' {
      lights.push_back(pos);
      continue;
    }
    const rad = substrs[5]:real;

    const col: vec3 = (substrs[6]:real, substrs[7]:real, substrs[8]:real);

    if intype == 'c' {
      cam.pos = pos;
      cam.targ = col;
      cam.fov = rad;
      continue;
    }

    const spow = substrs[9]: real;
    const refl = substrs[10]: real;

    if intype == 's' then
      objects.push_back(new sphere(pos, rad, new material(col, spow, refl)));

    else
      stderr.writeln("unknown type: ", intype);
  }
}


/* render a frame of xsz/ysz dimensions into the provided framebuffer */
proc render(fb: [?D], samples) {
  const rcpSamples = 1.0 / samples;
    
  forall (y, x) in D {
    var rgb: vec3;

    for s in 0..#samples do
      rgb += trace(getPrimaryRay(x, y, s));

    rgb *= rcpSamples;

    fb[y,x] = ((min(rgb(1), 1.0) * 255.0):uint(32) & 0xff) << redShift |
              ((min(rgb(2), 1.0) * 255.0):uint(32) & 0xff) << greenShift |
              ((min(rgb(3), 1.0) * 255.0):uint(32) & 0xff) << blueShift;
  }
}


//
// TODO: can x, y be made into a 2-tuple
//
proc getPrimaryRay(x, y, sample) {
  var k = cam.targ - cam.pos,
      j = (0.0, 1.0, 0.0);
  normalize(k);
  const i = crossProduct(j, k);
  j = crossProduct(k, i);
  const m: [0..2] vec3 = [i, j, k];
  var pRay = new ray();
  pRay.dir = getSamplePos(x, y, sample);
  pRay.dir(Z) = 1.0 / halfFieldOfView;
  pRay.dir *= rayMagnitude;

  const dir = pRay.dir + pRay.orig,
        // TODO: there has to be a better way to write this:
        foo = dir(X) * m[0] + dir(Y) * m[1] + dir(Z) * m[2],
        orig = pRay.orig(X) * m[0] + pRay.orig(Y)*m[1] + pRay.orig(Z) * m[2] + cam.pos;

  // TODO: assign directly into orig?
  pRay.orig = orig;

  pRay.dir = foo + orig;

  return pRay;
}


/* trace a ray throught the scene recursively (the recursion happens through
 * shade() to calculate reflection rays if necessary).
 */
proc trace(ray, depth=0): vec3 {
  var nearestObj: sphere;
  var sp, nearestSp: spoint;
  
  /* if we reached the recursion limit, bail out */
  if depth > maxRayDepth then
    return (0.0, 0.0, 0.0);

  /* find the nearest intersection ... */
  //
  // TODO: This would clean up nicely if using a vector
  //
  // TODO: minloc reduction?
  for obj in objects {
    if raySphere(obj, ray, sp, useSp=true) {
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

proc getSamplePos(x, y, sample) {
  const sf = 2.0 / xres;

  var pt: vec3;
  pt(X) = x:real / xres:real - 0.5;
  pt(Y) = -((y:real / yres:real) - 0.65) / aspect;

  if sample {
    const jt = jitter(x, y, sample);
    pt(X) += jt(X) * sf;
    pt(Y) += jt(Y) * sf / aspect;
  }

  return pt;
}

proc jitter(x, y, s) {
  return (urand[(x + (y << 2) + irand[(x + s) & mask]) & mask](X),
          urand[(y + (x << 2) + irand[(y + s) & mask]) & mask](Y));
}


proc raySphere(sph, ray, ref sp, useSp=false) {
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

  if (d < 0.0) then return 0;

  const sqrtD = sqrt(d);
  var t1 = (-b + sqrtD) / (2.0 * a),
      t2 = (-b - sqrtD) / (2.0 * a);

  // TODO: simplify?
  if (t1 < errorMargin && t2 < errorMargin) || (t1 > 1.0 && t2 > 1.0) then
    return 0;

  if (useSp) {
    if (t1 < errorMargin) then t1 = t2;
    if (t2 < errorMargin) then t2 = t1;
    sp.dist = min(t1, t2);
    
    sp.pos = ray.orig + ray.dir * sp.dist;

    sp.normal = (sp.pos - sph.pos) / sph.rad;

    sp.vref = reflect(ray.dir, sp.normal);
    normalize(sp.vref);
  }

  return 1;
}


proc reflect(v, n) {
  return -(2.0 * dot(v, n) * n - v);
}

proc shade(obj, sp, depth) {
  var col: vec3;  // TODO: reduction?

  for l in lights {
    var ldir = l - sp.pos;
    const shadowRay = new ray(orig = sp.pos, dir = ldir);
    var inShadow = false;
    // TODO: use reduction
    //
    // shoot shadow rays to determine if we have a line of sight with
    // the light
    //
    var dummy: spoint;
    for obj in objects {
      if (raySphere(obj, shadowRay, useSp=false, dummy)) {
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


inline proc normalize(ref a) {
  a /= sqrt(dot(a,a));
}

inline proc dot(a,b) {
  return a(X)*b(X) + a(Y)*b(Y) + a(Z)*b(Z);
}

proc crossProduct(v1, v2) {
  return (v1(Y)*v2(Z) - v1(Z)*v2(Y),
          v1(Z)*v2(X) - v1(X)*v2(Z),
          v1(X)*v2(Y) - v1(Y)*v2(X));
}
