/*
 * numBits(type) -- returns the number of bits in a type
 */

def numBits(type t) where t == complex(64) return 64;
def numBits(type t) where t == complex(128) return 128;
def numBits(type t) where t == complex(256) return 256;
def numBits(type t) where t == int(8) return 8;
def numBits(type t) where t == int(16) return 16;
def numBits(type t) where t == int(32) return 32;
def numBits(type t) where t == int(64) return 64;
def numBits(type t) where t == uint(8) return 8;
def numBits(type t) where t == uint(16) return 16;
def numBits(type t) where t == uint(32) return 32;
def numBits(type t) where t == uint(64) return 64;
def numBits(type t) where t == float(32) return 32;
def numBits(type t) where t == float(64) return 64;
def numBits(type t) where t == float(128) return 128;
