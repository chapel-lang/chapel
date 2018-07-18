record ForwardingRecord {
  var instance;

  proc init(type msgType) {
    instance = new ForwardedClass(msgType);
  }

  forwarding instance;
}

class ForwardedClass {
  type msgType;
  var _bufDom = {0..0};
  var _buf : [_bufDom] msgType;

  proc init(type msgType) {
    this.msgType = msgType;
  }
  
  proc fn(msg : msgType) {
    _buf[0] = msg;
  }
}

module other {
  record R {}
  proc main() {
    var rec = new ForwardingRecord(R);
    rec.fn(new R());
  }
}
