extern proc getenv(const name: c_string): c_string;
writeln(createStringWithNewBuffer(getenv('DASH_E_ENV_VAR')));
