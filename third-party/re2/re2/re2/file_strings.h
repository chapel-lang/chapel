#ifndef FILE_STRINGS_H__
#define FILE_STRINGS_H__

#include <limits>
#include <string>
#include <cassert>

extern "C" {
  #include <stdint.h>
}

namespace re2 {

// Read a byte. Returns a byte in [0..255] if everything
// is OK, or a negative number for EOF or an error. 
// getc is an example of such a function. The first
// argument is a file pointer (e.g. FILE* or something else).
typedef int (*read_byte_fn_t)(void*);

// During a search over a file, we might be able to
// drop buffers for data already searched.
// This function performs the dropping. It takes
// in the file pointer, what the search process
// believes to be the current offset, and what the
// search process believes to be the minimum offset
// that we cannot discard.
typedef void (*discard_fn_t)(void*, int64_t, int64_t);

// These are really just variables (not constants) to
// enable better testing.
extern int64_t gFileStringDiscardCheckMask;
extern int gFileStringAllowBufferSearch;

class RE2; // forward declare

struct FileSearchInfo {
  // These "bytes" might be negative to indicate an error
  // but normally they are in 0..255
  int32_t byte;
  int32_t prev_byte;

  // What is the offset of byte (that we already read)?
  int64_t offset;
  // At what offset did we read the first EOF or error?
  int64_t end_offset;

  // The file and operations on it
  void* file;
  read_byte_fn_t read_byte_fn;
  discard_fn_t discard_fn;

  // We keep a pointer to the regular expression so that our discard
  // method can use prefix length, max match length
  RE2* re;

  // What are we going to ultimately return from this match?
  // 0 => we aren't going to return any positions
  // 1 => we're going to return the position of the entire match
  // 2+=> we're going to also return capture groups
  int nmatch;

  // Settings controlling the discarding.
  // If can_discard is set, according to need_reset_unmatched
  // and need_whole_pattern, we can discard from the buffer
  // bytes that we will not need to revisit.
  //
  // In any case, if nmatch > 1 (ie we are returning capture
  // groups), we will not discard left of a capture group.
  //
  // If keep_unmatched is set and if we have not matched yet,
  // we cannot discard (since we need to be able to go back)
  //
  // If keep_whole_pattern is set, we will not discard left of
  // where a match starts or could start.
  // If it is not set, we may discard up to the end of a match
  // if there is a match (except for the rule above about captures).
  //
  bool can_discard;
  bool keep_unmatched;
  bool keep_whole_pattern;
};

// forward declare.
struct FileSearchPtr;

static int64_t get_fileptr_offset(const FileSearchPtr& x);

// advance to offset+amt, but offset might be INT64_MAX to indicate to end
int64_t advance_file_to(FileSearchInfo* fi, int64_t offset, int amt);

// THE RULES:
//  RE2 always initializes the pointer to dereference with
//    strpiece.begin_reading()
//
struct ReadingFileSearchPtr {
  int64_t offset;
  FileSearchInfo* fi;

  // construct it
  ReadingFileSearchPtr()
    : offset(-1), fi(NULL) {
  }

  // RE2 uses these operators normally:
  //   for( p = something; p < something; p++ )
  //     int c = *p & 0xFF
  //   but will occasionally use p[0] or p[-1]

  // ptr dereference *
  char operator*() {
    return fi->byte;
  }

  // postfix ++
  ReadingFileSearchPtr operator++(int) {
    assert( offset != std::numeric_limits<int64_t>::max() );

    ReadingFileSearchPtr ret = *this;
    this->offset = advance_file_to(fi, offset, 1);
    return ret;
  }
  // += 
  ReadingFileSearchPtr operator+=(int amt) {
    assert( offset != std::numeric_limits<int64_t>::max() );

    this->offset = advance_file_to(fi, offset, amt);
    return *this;
  }
  // = 
  ReadingFileSearchPtr operator=(const FileSearchPtr & p) {
    offset = get_fileptr_offset(p);
    this->offset = advance_file_to(fi, offset, 0);
    return *this;
  }


  // prefix -- when running backward
  ReadingFileSearchPtr operator--() {
    assert(0);
    fi = NULL;
    return *this;
  }
  // []
  char operator[](int i) {
    assert( offset != std::numeric_limits<int64_t>::max() );
    if( i == -1 ) {
      return fi->prev_byte;
    } else if( i == 0 ) {
      return fi->byte;
    } else {
      this->offset = advance_file_to(fi, offset, i);
      return fi->byte;
    }
  }
};

// A non-reading pointer (used for boundaries and captures)
// is just an integer.
struct FileSearchPtr {
  int64_t offset;
  FileSearchInfo* fi;

  FileSearchPtr()
    : offset(-1), fi(NULL) {
  }
  // allow implicit conversion from NULL.
  //FileSearchPtr(void* arg)
  //  : offset(-1) {
  //}
  // allow implicit conversion from ReadingFileSearchPtr.
  FileSearchPtr(const ReadingFileSearchPtr p)
    : offset(p.offset), fi(p.fi) {
  }

  // No dereference.

  // postfix ++
  FileSearchPtr operator++(int) {
    FileSearchPtr ret = *this;
    this->offset++;
    return ret;
  }
  // += 
  FileSearchPtr operator+=(int amt) {
    offset += amt;
    return *this;
  }
  // = 
  FileSearchPtr operator=(const ReadingFileSearchPtr & p) {
    offset = p.offset;
    return *this;
  }
};

static inline
int64_t get_fileptr_offset(const FileSearchPtr& x)
{
  return x.offset;
}

static inline
int64_t operator-(FileSearchPtr lhs, FileSearchPtr rhs)
{
  assert( lhs.offset != std::numeric_limits<int64_t>::max() );

  // If lhs is unbounded, return unbounded.
  if( lhs.offset == std::numeric_limits<int64_t>::max() ) return lhs.offset;
 
  return lhs.offset - rhs.offset;
}

// used in NFA p-1, p+1 for captures
static inline
FileSearchPtr operator-(FileSearchPtr lhs, int rhs)
{
  assert( lhs.offset != std::numeric_limits<int64_t>::max() );
  // If lhs is unbounded, return unbounded.
  if( lhs.offset == std::numeric_limits<int64_t>::max() ) return lhs;
 
  FileSearchPtr ret;
  ret.offset = lhs.offset - rhs;
  return ret;
}
static inline
FileSearchPtr operator+(FileSearchPtr lhs, int rhs)
{
  assert( lhs.offset != std::numeric_limits<int64_t>::max() );
  // If lhs is unbounded, return unbounded.
  if( lhs.offset == std::numeric_limits<int64_t>::max() ) return lhs;
 
  FileSearchPtr ret;
  ret.offset = lhs.offset + rhs;
  return ret;
}

int compare_FileSearchPtr(FileSearchPtr a, FileSearchPtr b);

// Comparison routines that take into account EOF character
static inline
bool operator<(FileSearchPtr a, FileSearchPtr b)
{
  return compare_FileSearchPtr(a, b) < 0;
}
// used in NFA p <= context.end()
static inline
bool operator<=(FileSearchPtr a, FileSearchPtr b)
{
  return compare_FileSearchPtr(a, b) <= 0;
}
static inline
bool operator>=(FileSearchPtr a, FileSearchPtr b)
{
  return compare_FileSearchPtr(a, b) >= 0;
}

// used in NFA p > text.end()
static inline
bool operator>(FileSearchPtr a, FileSearchPtr b)
{
  return compare_FileSearchPtr(a, b) > 0;
}
// NFA and Prog::EmptyFlags do p == text.end()
static inline
bool operator==(FileSearchPtr a, FileSearchPtr b)
{
  return compare_FileSearchPtr(a, b) == 0;
}
static inline
bool operator!=(FileSearchPtr a, FileSearchPtr b)
{
  return compare_FileSearchPtr(a, b) != 0;
}


// DFA uses memch (and memrchr) and checks the result == NULL
static inline
bool operator==(FileSearchPtr lhs, void* rhs)
{
  return lhs.offset == -1; 
}

FileSearchPtr file_memchr_ptr_end(ReadingFileSearchPtr s, FileSearchPtr end, int c);


inline FileSearchPtr re2_memchr_ptr_end(ReadingFileSearchPtr s, FileSearchPtr end, int c)
{
  return file_memchr_ptr_end(s, end, c);
}

inline FileSearchPtr re2_memrchr(ReadingFileSearchPtr s, int c, size_t n)
{
  assert(0);
  return FileSearchPtr();
}

int file_memcmp(const char* a, ReadingFileSearchPtr s, size_t n);

static inline
int re2_memcmp(const char* a, ReadingFileSearchPtr s, size_t n)
{
  return file_memcmp(a, s, n);
}

struct FilePiece {
  int64_t start;
  int64_t end_;
  FileSearchInfo* fi;

  typedef FileSearchPtr ptr_type;
  typedef ReadingFileSearchPtr ptr_rd_type;

  static bool can_discard(int64_t diff) {
    return (diff & gFileStringDiscardCheckMask) == 0;
  }
  void discard(bool match, ptr_type match_start, ptr_type match_end, ptr_type min_cap) const;

  static ptr_type null_ptr() {
    return ptr_type();
  }

  FilePiece()
    : start(-1), end_(0), fi(NULL)
  { }
  // Used for extracting matches.
  FilePiece(FileSearchPtr p, int len)
    : start(p.offset), end_(p.offset + len), fi(NULL)
  { }
  // Use this one for context.
/*  FilePiece(int64_t start, int64_t len)
    : start(start), len(len), fi(NULL)
  { }
*/

  int64_t size() const {
    return end_ - start;
  }
  void set_ptr_end(FileSearchPtr p, FileSearchPtr end) {
    this->start = p.offset;
    this->end_ = end.offset;
  }

  /*
  void set(FileSearchPtr p, int64_t len) {
    this->start = p.offset;
    this->len = len;
  }
  */
  void set_channel_info(FileSearchInfo* fi, int64_t start, int64_t end) {
    this->fi = fi;
    this->start = start;
    this->end_ = end;
  }

  void clear() {
    fi = NULL;
    start = -1;
    end_ = 0;
  }

  FileSearchPtr begin() const {
    FileSearchPtr ret;
    ret.offset = start;
    return ret;
  }
  ReadingFileSearchPtr begin_reading() const {
    ReadingFileSearchPtr ret;
    ret.offset = start;
    ret.fi = fi; 
    advance_file_to(fi, start, 0);
    return ret;
  }

  FileSearchPtr end() const {
    FileSearchPtr ret;
    ret.offset = end_;
    ret.fi = fi;
    return ret;
  }

  void remove_prefix(int n) {
    start += n;
    //len -= n;
  }

  // Find the position where c occurs (ie memchr)
  // uses begin, end pointers
  FileSearchPtr find_c(ReadingFileSearchPtr s, FileSearchPtr end, int c) const;

  // For debugging.
  std::string as_string() const {
    return std::string();
  }

  static int discard_check_mask();
  static int allow_buffer_search();
  static void set_global_options(int gSpecialStringDiscardCheckMask,
                                 int gSpecialStringAllowBufferSearch);
};

} // end namespace re2


#endif
