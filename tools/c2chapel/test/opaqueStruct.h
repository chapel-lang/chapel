typedef struct myOpaque myOpaque;

struct foobar {
  int a;
  int b;
  int c;
};

// Ensure that we don't generate 'foobar' twice
typedef struct foobar foobar;
