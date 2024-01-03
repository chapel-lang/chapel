class C { }

{
  type T = borrowed C;
  var b = new T();
}
{
  type T = borrowed C?;
  var b = new T();
}

{
  proc T type {
    return borrowed C;
  }
  var b = new T();
}

{
  proc T() type {
    return borrowed C?;
  }
  var b = new T()();
}
