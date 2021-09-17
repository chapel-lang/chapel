typedef union myOpaque myOpaque;

union foobar {
  int a;
  int b;
  int c;
};

// Ensure that we don't generate 'foobar' twice
typedef union foobar foobar;
