use IO, IO.FormattedIO;
import Subprocess.spawn;

proc readEcgData(path: string): [] int throws {
  iter getSamples(fr) {
    var a, b: int;
    while fr.readf("%i %i\n", a, b) {
      yield b;
    }
  }
  const samples = getSamples(openReader(path));
  return samples;
}

proc plotDwtData(signal, coefficients, n: int) throws {
  openWriter("results/signal.txt").write(signal);
  openWriter("results/coeffs.txt").write(coefficients);

  try {
    spawn(["python3", "plot.py", n:string]);
  } catch e {
    writeln("Failed to run plotting script: ", e.message());
  }
  writeln("Generated `results/ecg_dwt.png`");
}
