extern proc getenv(const name: c_string): c_string;
for loc in Locales do on loc do writeln(getenv('GETENV_ENV_VAR'));
