proc file.dup():file {
  return open(path=this.path, mode=iomode.r, hints=IOHINT_NONE, style=this._style);
}

var fl = open("open_inside_proc.good", iomode.r);
var newFl = fl.dup();
