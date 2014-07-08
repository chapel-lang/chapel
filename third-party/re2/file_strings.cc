#include "stringpiece.h"
#include "re2.h"
#include <cstdio>

namespace re2 {

// must be 2^x - 1
// To see if we should do extra discard-processing work,
//  we check ((index-start) & discard_mask) == 0.
int64_t gFileStringDiscardCheckMask = 0x1FFFF;
// 1 or 0.
int gFileStringAllowBufferSearch = 1;

int FilePiece::discard_check_mask() {
  return gFileStringDiscardCheckMask;
}

int FilePiece::allow_buffer_search() {
  return gFileStringAllowBufferSearch;
}

void FilePiece::set_global_options(int SpecialStringDiscardCheckMask,
                                   int SpecialStringAllowBufferSearch)
{
  gFileStringDiscardCheckMask = SpecialStringDiscardCheckMask;
  gFileStringAllowBufferSearch = SpecialStringAllowBufferSearch;
}

static inline
void read_byte(FileSearchInfo* fi)
{
  fi->offset++;
  fi->prev_byte = fi->byte;
  if( fi->offset < fi->end_offset ) {
    fi->byte = fi->read_byte_fn(fi->file);
    // Keep end_offset as the first time we encountered EOF/error
    if( fi->byte < 0 ) fi->end_offset = fi->offset;
  } else {
    fi->byte = -1; // ie, EOF.
  }
}


int64_t advance_file_to(FileSearchInfo* fi,
                        int64_t offset, int amt)
{
  int64_t new_offset = offset+amt;
  int64_t diff = new_offset - fi->offset;

  /*
  printf("ADVANCE CHANNEL from %lli TO %lli\n",
         (long long int) fi->offset,
         (long long int) new_offset);
  */

  if( diff == 0 ) {
    goto done;
  }
  
  if( diff == 1 ) {
    // advance by one byte by reading a byte!
    read_byte(fi);
    goto done;
  }

  // If the offset is currently INT64_MAX,
  // that represents the end of the channel - wherever that
  // may be. Read until the end.
  if( offset == std::numeric_limits<int64_t>::max() ) {
    while( fi->byte >= 0 ) {
      // While the end offset is unbounded, read bytes until we get to EOF.
      read_byte(fi);
    }
    goto done;
  }

  assert(new_offset >= fi->offset); // can't go backwards

  // At this point, diff >= 2 and should be reasonable.
  // Get prev_byteand byte at new_offset.
  while( fi->offset < new_offset ) {
    read_byte(fi);
  }

done:
  return fi->offset;
}

// TODO -- also pass in ncap
void FilePiece::discard(bool match, ptr_type match_start, ptr_type match_end, ptr_type min_cap) const {
  int64_t cur;
  int64_t target;

  /*printf("DISCARD CALLED match=%i, match_start=%lli, match_end=%lli, min_cap=%lli\n", (int) match,
         (long long int) match_start.offset, (long long int) match_end.offset, (long long int) min_cap.offset);
  */

  cur = fi->offset;

  // Assume we can discard to the current position
  // and then we'll find all the reasons not to.
  target = cur;

  // Don't ever discard if can_discard is not set.
  if( !fi->can_discard ) return;

  // Don't ever discard if we have no discard routine.
  if( !fi->discard_fn ) return;

  // If we have not matched yet, a match could start
  // back max_match_length bytes, and we might not be
  // able to discard at all.
  if( !match ) {
    // Can't discard before match if keep_unmatched is set.
    if( fi->keep_unmatched ) return;
    // Can't discard before match if we don't have a maximum
    // match length.
    if( fi->re->max_match_length_bytes() < 0 ) return;
    // Otherwise, we can discard but must keep a window
    // of max_match_length
    target = cur - fi->re->max_match_length_bytes();
    // (note, we could potentially discard more if keep_whole_pattern
    //  is false and nmatch == 1, or if nmatch == 0).
  } else {
    // There is a match
   
    // If keep_whole_pattern is set, we can't discard to the left
    // of a match start
    if( fi->keep_whole_pattern ) target = match_start.offset;

    // If nmatch > 1 we can't discard left of any possible capture start
    if( fi->nmatch > 1 && target > min_cap.offset ) target = min_cap.offset;

    // Never discard past the *end* of a match
    if( target > match_end.offset ) target = match_end.offset;
  }

  // Fixed prefixes are accounted for separately from the RE
  // and we don't want to discard before they occur
  // (has to be accounted for with keep_whole_pattern or no match)
  target -= fi->re->prefix_length_bytes();
  target -= 2; // to be conservative in case of boundary checks

  if( target > cur ) target = cur;
  fi->discard_fn(fi->file, cur, target);
}

static inline
int file_pos_cmp(int64_t a, int64_t b)
{
  if( a < b ) return -1;
  else if( a == b ) return 0;
  else return 1;
}

int compare_FileSearchPtr(FileSearchPtr a, FileSearchPtr b)
{
  FileSearchInfo* fi = NULL;
  if( a.fi ) fi = a.fi;
  if( b.fi ) fi = b.fi;

  int64_t channel_end = std::numeric_limits<int64_t>::max();
  if( fi ) channel_end = fi->end_offset;

  /*printf("COMPARING %lli %lli end=%lli\n",
         (long long int) a.offset,
         (long long int) b.offset,
         (long long int) channel_end);
  */

  // if both are unbounded... they are the same.
  if( a.offset == std::numeric_limits<int64_t>::max() &&
      b.offset == std::numeric_limits<int64_t>::max() ) {
    return 0;
  }

  // if a is unbounded but b is not
  if( a.offset == std::numeric_limits<int64_t>::max() ) {
    // use channel_end instead of a.offset.
    return file_pos_cmp(channel_end, b.offset);
  }
  // if b is unbounded but a is not
  if( b.offset == std::numeric_limits<int64_t>::max() ) {
    // use channel_end instead of b.offset.
    return file_pos_cmp(a.offset, channel_end);
  }
  // otherwise, just compare like normal.
  return file_pos_cmp(a.offset, b.offset);
}


FileSearchPtr FilePiece::find_c(ReadingFileSearchPtr s, FileSearchPtr end, int c) const
{
  FileSearchPtr begin = s;
  for( ; s < end; s++ ) {
    if( ( (*s) & 0xff ) == c ) return s;
    if( this->can_discard(s - begin) ) {
      this->discard(false, s, s, s);
    } 
  }
  return FileSearchPtr();
}


int file_memcmp(const char* a, ReadingFileSearchPtr s, size_t n)
{
  for( size_t i = 0; i < n; i++,s++ ) {
    int32_t byte = *s;
    if( byte < 0 ) return -1; // encountered an error.
    byte &= 0xff;
    if( a[i] == byte ) continue; // keep checking
    else if( a[i] < byte ) return -1;
    else return 1;
  }
  return 0;
  /*
  qio_channel_re_ptr end = s;
  end.offset = std::numeric_limits<int64_t>::max(); // for s < channel end
  for( size_t i = 0; i < n && s < end; i++,s++ ) {
    int32_t byte = *s;
    byte &= 0xff;
    if( a[i] == byte ) continue; // keep checking
    else if( a[i] < byte ) return -1;
    else return 1;
  }
  return 0;
  */
}


} // end namespace re2
