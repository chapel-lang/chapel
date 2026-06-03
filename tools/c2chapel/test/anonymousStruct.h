struct foobar {
  int a;
  int b;
  int c;
};

typedef struct {
  int a;
} sports;

union namedUnion {
  int a;
};

typedef union {
  int a;
} anonymousUnion;

struct nestedAnon {
  struct {
    int x;
    int y;
  };
  int z;
};

typedef struct {
  struct {
    int x;
    int y;
  };
  int z;
} nestedAnon_t;

struct nestedAnonWithUnion {
  union {
    int a;
    int b;
  };
  int c;
};

typedef struct {
  union {
    int a;
    int b;
  };
  int c;
} nestedAnonWithUnion_t;
