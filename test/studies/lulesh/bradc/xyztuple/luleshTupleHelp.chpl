inline operator *(v:real, xyz:3*real) {
  return (v*xyz[0], v*xyz[1], v*xyz[2]);
}

inline operator *(xyz:3*real, v:real) {
  return (xyz[0]*v, xyz[1]*v, xyz[2]*v);
}

inline operator /(xyz:3*real, v:real) {
  return (xyz[0]/v, xyz[1]/v, xyz[2]/v);
}

inline operator -=(ref xyz: 3*real, v:real) {
  xyz[0] -= v;
  xyz[1] -= v;
  xyz[2] -= v;
}

inline proc sumOfProduct(t1:3*real, t2:3*real) {
  return t1[0]*t2[0] + t1[1]*t2[1] + t1[2]*t2[2];
}

inline proc sumOfSquare(xyz:3*real) {
  return xyz[0]**2 + xyz[1]**2 + xyz[2]**2;
}

inline proc sum(xyz:3*real) {
  return xyz[0] + xyz[1] + xyz[2];
}
