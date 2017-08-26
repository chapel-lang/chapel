
/* A help module for the mason package manager */

use Help;


proc masonHelp() {
  writeln("Chapel's package manager");
  writeln();
  writeln('Usage:');
  writeln('    mason <command> [<args>...]');
  writeln('    mason [options]');
  writeln();
  writeln('Options:');
  writeln('    -h, --help          Display this message');
  writeln('    -V, --version       Print version info and exit');
  writeln('    --list              List installed commands');
  writeln();
  writeln('Some common mason commands are (see all commands with --list):');
  writeln('    new         Create a new mason project');
  writeln('    update      Update/Generate Mason.lock');
  writeln('    build       Compile the current project');
  writeln('    clean       Remove the target directory');
  writeln('    doc         Build this project\'s documentation');
  //writeln('    init        Create a new mason project in an existing directory');
  writeln('    run         Build and execute src/<project name>.chpl');
}

proc masonList() {
  writeln('Installed Mason Commands:');
  writeln('      new                ');
  writeln('      update             ');
  writeln('      build              ');
  writeln('      run                ');
  writeln('      clean              ');
  writeln('      help               ');
  writeln('      version            ');
}


proc masonRunHelp() {
  writeln('Run the compiled project and output to standard output');
  writeln();
  writeln('Usage:');
  writeln('   mason run [options]');
  writeln();
  writeln('Options:');
  writeln('    -h, --help                   Display this message');
  writeln('        --build                  Compile before running binary');
  writeln('        --show                   Increase verbosity');
  writeln('Runtime arguments can also be included after the run arguments ');
  writeln();
  writeln('When no options are provided, the following will take place:');
  writeln('   - Execute binary from mason project if target/ is present');
  writeln('   - If no target directory, build and run is Mason.toml is present');
}

proc masonBuildHelp() {
  writeln('Compile a local package and all of its dependencies');
  writeln();
  writeln('Usage:');
  writeln('    mason build [options]');
  writeln();
  writeln('Options:');
  writeln('    -h, --help                   Display this message');
  writeln('        --show                   Increase verbosity');
  writeln('        --release                Compile to target/release with optimizations (--fast)');
  writeln();
  writeln('When no options are provided, the following will take place:');
  writeln('   - Build from mason project if Mason.lock present');
}

proc masonNewHelp() {
  writeln('Usage:');
  writeln('    mason new [options] <project name>');
  writeln();
  writeln('Options:');
  writeln('    -h, --help                   Display this message');
  writeln('        --show                   Increase verbosity');
  writeln('        --no-vcs                 Do not initialize a git repository');
  
}
