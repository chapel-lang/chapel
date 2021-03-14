use ExampleErrors;

try {
  throwAnError();
} catch (e) {
  writeln("named catchall");
}

try! {
  throwOtherError();
} catch (e: OtherError) {
  writeln("specified catch");
}
