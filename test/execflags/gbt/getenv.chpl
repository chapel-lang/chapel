extern proc getenv(const name: c_string): c_string;
extern proc printf(const fmt: c_string, const arg1: c_string);
for loc in Locales do on loc do printf('%s\n', getenv('GETENV_ENV_VAR'));
