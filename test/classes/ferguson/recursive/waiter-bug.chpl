class Waiter {
  type valueType;
  var value:valueType;

  var prev : unmanaged Waiter?;
  var next : unmanaged Waiter?;

  proc init(value) {
    this.valueType = value.type;
    this.value = value;
  }
}

var x = new Waiter(1);


