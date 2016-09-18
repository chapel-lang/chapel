#include <cstdlib>
#include <cstdio>
#include "defs.h"
#include "optionparser.h"
#include "opts.h"

#define _STR(i) #i
#define STR(i) _STR(i)

using namespace option;

enum optionIndex {
  UNKNOWN,
  HELP,
  VERSION,
  VERBOSE,
  MOL,
  STEPS,
};

struct arg: public option::Arg {
  static ArgStatus Required(const Option& option, bool msg)
  {
    if (option.arg != 0 && option.arg[0] != 0) return ARG_OK;
    if (msg)
      fprintf(stderr, "Option '%s' requires an argument\n",
        option.name);
    return ARG_ILLEGAL;
  }

  static ArgStatus Numeric(const Option& option, bool msg)
  {
    char* endptr = 0;
    if (option.arg != 0 && strtol(option.arg, &endptr, 10)){};
    if (endptr != option.arg && *endptr == 0) return ARG_OK;

    if (msg)
      fprintf(stderr, "Option '%s' requires a numeric argument\n",
        option.name);
    return ARG_ILLEGAL;
  }
};

const Descriptor usage[] =
{
  {UNKNOWN, 0, "", "", arg::None,
    "Usage: "PROG_NAME" [options]\n"},
  {MOL, 0, "n", "nmol", arg::Numeric,
    "  -n, --nmol     number of particles (default: "STR(DEFAULT_MOL)")"},
  {STEPS, 0, "s", "steps", arg::Numeric,
    "  -s, --steps    steps of simulation (default: "STR(DEFAULT_STEPS)")"},
  {VERBOSE, 0, "v", "verbose", arg::Numeric,
    "  -v, --verbose  verbose level (default: "STR(DEFAULT_VERBOSE)")"},
  {HELP, 0, "h", "help", arg::None, 
    "  -h, --help     display this help and exit" },
  {VERSION, 0, "", "version", arg::None, 
    "  --version      display version information and exit" },
  {0,0,0,0,0,0} // boundary guard
};

arguments * parse_cmdline(int argc, char *argv[])
{
  argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
  Stats stats(usage, argc, argv);
  Option options[stats.options_max], buffer[stats.buffer_max];
  Parser parse(usage, argc, argv, options, buffer);

  if (parse.error())
    return NULL;

  if (options[HELP] || options[UNKNOWN]) {
    if (options[UNKNOWN])
      printf("%s: unknown options '%s'\n", PROG_NAME, options[UNKNOWN].name);
    printUsage(fwrite, stdout, usage);
    return NULL;
  }

  arguments * args = new arguments(); // initial with default values
  for (int i = 0; i < parse.optionsCount(); i++) {
    Option& o = buffer[i];
    switch (o.index()) {
      case MOL: args->mol = atoi(o.arg); break;
      case STEPS: args->steps = atoi(o.arg); break;
      case VERBOSE: args->verbose = atoi(o.arg); break;
      case UNKNOWN: break;
    }
  }
  return args;
}

arguments::arguments(int _mol, int _steps, int _verbose)
{
  mol = _mol;
  steps = _steps;
  verbose = _verbose;
}
