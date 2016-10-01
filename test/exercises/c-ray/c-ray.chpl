/* c-ray.chpl - a simple raytracing filter.
 *
 * Based on c-ray-f, developed by John Tsiombikas
 * Ported to Chapel by Brad Chamberlain, September 2016
 *
 * ---------------------------------------------------------------------
 * Usage:
 *   compile:  chpl -o c-ray c-ray.chpl
 *   run:      cat scene | ./c-ray >foo.ppm
 *   enjoy:    emacs foo.ppm (with a modern version of emacs)
 * ---------------------------------------------------------------------
 * Scene file format:
 *   # sphere (many)
 *   s  x y z  rad   r g b   shininess   reflectivity
 *   # light (many)
 *   l  x y z
 *   # camera (one)
 *   c  x y z  fov   tx ty tz
 * ---------------------------------------------------------------------
 */

config const rayMagnitude = 1000.0;  // trace rays of this magnitude

// TODO: all globals used?  Any that should be configs?

param maxLights = 16,
      maxRayDepth = 5,  // raytrace recurion limit
      fieldOfView = quarter_pi,
      errorMarging = 1e-6,
      redShift = 0,
      greenShift = 8,
      blueShift = 16;

inline proc halfFieldOfView return fieldOfView / 2;

type vec3 = 3*real;
param x = 1,
      y = 2,
      z = 3;

record ray {
  var orig, dir: vec3;
}

record material {
  var color: vec3,
      spow,           // specular power
      refl: real;     // reflection intensity
}

class sphere {
  var pos: vec3,
      rad: real,
      mat: material,
      next: sphere;
}

record spoint {
  var pos,         // position
      normal,      // normal
      vref: vec3,  // view reflection
      dist: real;  // parametric distance of intersection along the ray
}

record camera {
  var pos, targ: vec3,
      fov: real;
}

config const size = "800x600";

const ssize = size.partition("x");

if (ssize(2) != "x") then
  halt("--s option requires format WxH specifying the image Width x Height");

const xres = ssize(1):int,
      yres = ssize(3):int;

config const ifile = "stdin",
             ofile = "stdout";

config const rays = 1;

const aspect = xres:real / yres;

var objList: sphere;

config const numLights = 16;

var lights: [0..-1] vec3;

var cam: camera;

param nran = 1024,
      mask = nran - 1;

var urand: [0..#nran] vec3,
    irand: [0..#nran] int;

// TODO: implement usage
// config const help = false;

config const debug = true;

proc main(args: [] string) {
  if (args.size > 1) then usage(args);

  var pixels: [0..#xres, 0..#yres] uint(32);
  const infile = if ifile == "stdin" then stdin
                                     else open(ifile, iomode.r).reader(),
        outfile = if ofile == "stdout" then stdout
                                       else open(ofile, iomode.cw).writer();

  if debug {
    writeln("Rendering a scene that's ", (xres,yres));
    writeln("infile is: ", ifile);
    writeln("outfile is: ", ofile);
    writeln("rays is: ", rays);
  }

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
      u(x) = rand():real / RAND_MAX - 0.5;
    for u in urand do
      u(y) = rand():real / RAND_MAX - 0.5;
    for r in irand do
      r = (nran * (rand():real / RAND_MAX)): int;
  }
  if debug {
    writeln("urand: ", urand);
    writeln("irand: ", irand);
  }
}

proc usage(args) {
  for i in 1..args.size-1 {
    if (args[i] == "--help" || args[i] == "-h") {
      writeln("Usage: ", args[0], " [options]");
      writeln(" Reads a scene file from stdin, writes the image to stdout, and stats to stderr.");
      writeln();
      writeln("Options:");
      writeln("  --size WxH     where W is the width and H the height of the image");
      writeln("  --rays <rays>  shoot <rays> rays per pixel (antialiasing)");
      writeln("  --ifile <file> read from <file> instead of stdin");
      writeln("  --ofile <file> write to <file> instead of stdout");
      writeln("  --help         this help screen");
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

  objList = new sphere();
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
      objList.next = new sphere(pos, rad, new material(col, spow, refl),
                                objList.next);
    // TODO: Consider making objList into a vector as well?

    else
      stderr.writeln("unknown type: ", intype);
  }

  if debug {
    writeln("lights:  ", lights);
    writeln("camera:  ", cam);
    writeln("spheres: ", objList.next);
  }
}
