use IO, IO.FormattedIO;
import Subprocess.spawn;

iter getEcgSamples(path: string): int throws {
  const fr = openReader(path, locking=false);
  var a, b: int;
  while fr.readf("%i %i\n", a, b) do yield b;
}

iter getGoodCoeffs(path: string): int throws {
  const fr = openReader(path, locking=false);
  var c: int;
  while fr.read(c) do yield c;
}

proc plotDwtData(signal, coefficients, n) throws {
  openWriter("results/signal.txt").write(signal);
  openWriter("results/coeffs.txt").write(coefficients);
  try {
    spawn(["python3", "plot.py", n: string]);
  } catch e {
    writeln("Failed to run plotting script: ", e.message());
  }
  writeln("Generated `results/ecg_dwt.png`");
}
