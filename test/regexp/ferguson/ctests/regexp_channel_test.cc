
#include "qio.h"
#include "qio_regexp.h"
#include <assert.h>
#include <stdio.h>


#include "re2/re2.h"
#include <limits>

#define KEEP_NONE          0
#define KEEP_UNMATCHED     (1 << 0)
#define KEEP_WHOLE_PATTERN (1 << 1)
#define KEEP_ALL           (KEEP_UNMATCHED | KEEP_WHOLE_PATTERN)

#define TEST_RE   0
#define TEST_KEEP 1
using namespace re2;

struct capture {
  int64_t match_offset;
  int64_t match_length;
};

struct re_test_case {
  int kind;
  const char *data;
  const char *pattern;
  int64_t start;
  int64_t first_offset;
  int64_t length;
  int n_captures;
  struct capture captures[5];
  int idx;
};

struct keep_test_case {
  int kind;
  const char *data;
  const char *pattern;
  int keep_flags;
  int64_t min_channel_offset;
  int64_t max_channel_offset;
  int idx;
};

struct base_test_case {
  int kind;
  const char *data;
  const char *pattern;
};


struct re_test_case re_test_cases[] = {
  {TEST_RE, "It was the best of times, it was the blurst of times.", "[0-9]", 0, -1, 0}, 
  {TEST_RE, "It was the best of times, it was the blurst of times.", "times", 0, 19, 5},
  {TEST_RE, "It was the best of times, it was the blurst of times.", "times", 20, 47, 5},
  {TEST_RE, "From: Montgomery Burns", "From: (\\w+) (\\w+)", 0, 0, 22, 
        2, {{6, 10}, {17, 5}}},
  {TEST_RE, " xxxxxxxy", "x*yx*", 0, 1, 8},

  {TEST_RE, " xxxxxxxy", "^x*yx*", 0, -1, 0},
  {TEST_RE, " xxxxxxxy", "x*yx*$", 0, 1, 8},
  {TEST_RE, " xxxxxxxy ", "x*yx*$", 0, -1, 0},
  {TEST_RE, "xxxxxxxy", "^x*yx*$", 0, 0, 8},
  {TEST_RE, "xxxxxxxy ", "^x*yx*", 0, 0, 8},
  {TEST_RE, " xz...", "x(y|z)", 0, 1, 2, 1, {{2, 1}}},

  {TEST_RE, " xb.y.", "x(y|z)", 0, -1, 0},
  {TEST_RE, " xz...", "^x(y|z)", 0, -1, 0},
  {TEST_RE, "xz...", "^x(y|z)", 0, 0, 2, 1, {{1, 1}}},
  {TEST_RE, " xz...", "x(y|z)$", 0, -1, 0},
  {TEST_RE, " xz", "x(y|z)$", 0, 1, 2, 1, {{2, 1}}},

  {TEST_RE, " xy", "(xy|xz)", 0, 1, 2, 1, {{1, 2}}},
  {TEST_RE, " xz", "(xy|xz)", 0, 1, 2, 1, {{1, 2}}},
  {TEST_RE, " zb", "(xy|xz)", 0, -1, 0},
  {TEST_RE, " xy", "^(xy|xz)", 0, -1, 0},
  {TEST_RE, "xz", "^(xy|xz)", 0, 0, 2, 1, {{0, 2}}},
  {TEST_RE, "xz", "(xy|xz)$", 0, 0, 2, 1, {{0, 2}}},
  {TEST_RE, "xz ", "(xy|xz)$", 0, -1, 0},
  {TEST_RE, "xz", "^(xy|xz)$", 0, 0, 2, 1, {{0, 2}}},
  {TEST_RE, "x z", "^(xy|xz)$", 0, -1, 0},

  {TEST_RE, "abcd", "abcd|bc", 0, 0, 4},
};

int64_t n_re_test_cases = sizeof(re_test_cases)/sizeof(re_test_case);



struct keep_test_case keep_test_cases[] = {
  {TEST_KEEP, "aaaabbbbccccdddd", "aaaabbbbccccdddd|bc",
              KEEP_WHOLE_PATTERN, 0, 0},
  {TEST_KEEP, "aaaabbbbccccdddddddddddd", "aaaabbbbccccd+|bc",
              KEEP_UNMATCHED, 20, 24},
  {TEST_KEEP, "aaaabbbbccccdddd", "aaaabbbbccccdddd|bc",
              KEEP_ALL, 0, 0},
  {TEST_KEEP, "aaaabbbbccccdddddddddddd", "aaaabbbbccccd+|bc",
              KEEP_NONE, 20, 24},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "no", KEEP_NONE, 15, 20},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "a+", KEEP_ALL, 0, 0},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "a+", KEEP_UNMATCHED, 15, 20},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "(a+)", KEEP_NONE, 0, 0},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "a+", KEEP_WHOLE_PATTERN, 0, 0},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "a+", KEEP_WHOLE_PATTERN, 0, 0},
  {TEST_KEEP, "bbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaa", "a+", KEEP_WHOLE_PATTERN, 15, 20},
  {TEST_KEEP, "bbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaa", "(a+)", KEEP_WHOLE_PATTERN, 15, 20},
  {TEST_KEEP, "bbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaa", "(a+)", KEEP_NONE, 15, 20},
  {TEST_KEEP, "bbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaa", "a+", KEEP_NONE, 35, 40},
  // Try them with start-at-begining to run onepass search in RE2
  {TEST_KEEP, "aaaabbbbccccdddd", "^aaaabbbbccccdddd|bc",
              KEEP_WHOLE_PATTERN, 0, 0},
  {TEST_KEEP, "aaaabbbbccccdddddddddddd", "^aaaabbbbccccd+|bc",
              KEEP_UNMATCHED, 20, 24},
  {TEST_KEEP, "aaaabbbbccccdddd", "^aaaabbbbccccdddd|bc",
              KEEP_ALL, 0, 0},
  {TEST_KEEP, "aaaabbbbccccdddddddddddd", "^aaaabbbbccccd+|bc",
              KEEP_NONE, 20, 24},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "^no", KEEP_NONE, 0, 1},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "^a+", KEEP_ALL, 0, 0},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "^a+", KEEP_UNMATCHED, 15, 20},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "^(a+)", KEEP_NONE, 0, 0},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "^a+", KEEP_WHOLE_PATTERN, 0, 0},
  {TEST_KEEP, "aaaaaaaaaaaaaaaaaaaa", "^a+", KEEP_WHOLE_PATTERN, 0, 0},
  {TEST_KEEP, "bbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaa", "^a+", KEEP_WHOLE_PATTERN, 0, 1},
  {TEST_KEEP, "bbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaa", "^(a+)", KEEP_WHOLE_PATTERN, 0, 1},
  {TEST_KEEP, "bbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaa", "^(a+)", KEEP_NONE, 0, 1},
  {TEST_KEEP, "bbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaa", "^a+", KEEP_NONE, 0, 1},
};

int64_t n_keep_test_cases = sizeof(keep_test_cases)/sizeof(keep_test_case);

// Try for some different configurations:
//  File/Channel Type:
//    -- in-memory file
//    -- memory mapped file
//    -- pipe
//  Vary the values of:
//    int gSpecialStringDiscardCheckMask = 1;  // must be 2^x - 1
//    int gSpecialStringAllowBufferSearch = 0;
//     (in re2/special_strings.cc)
//
// test that channel discarding works correctly
// test channel RE search with large files,
//   check that memory is not balooning
//
// see qio_regexp.h

void check_channel(qio_hint_t file_hints, qio_hint_t ch_hints, char unbounded_channels, char reopen, int discard_check_mask, int allow_buffer_search, struct base_test_case *test)
{
  int threadsafe = 0;
  qio_chtype_t type = QIO_CH_BUFFERED;
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;
  int64_t start = 0;
  qioerr err;
  char* chhints;
  char* fhints;
  int memory;
  char filename[128];
  int fd = -1;
  // mostly, we just want to make sure the channel we open for writing 
  // will have enough room for the test data
  int64_t ch_end = std::numeric_limits<int64_t>::max();

  // Set our favorite globals!
  
  re2::FilePiece::set_global_options(discard_check_mask,allow_buffer_search);

  strcpy(filename,"/tmp/qio_testXXXXXX");

  //if( unbounded_channels ) ch_end = std::numeric_limits<int64_t>::max();


  ch_hints = (ch_hints & ~ QIO_CHTYPEMASK) | type;

  memory = 0;

  if( (file_hints & QIO_METHODMASK) == QIO_METHOD_MEMORY ||
      (ch_hints & QIO_METHODMASK) == QIO_METHOD_MEMORY ) {
    memory = 1;
  }
  if( memory ) {
    file_hints = (file_hints & ~ QIO_METHODMASK ) | QIO_METHOD_MEMORY;
    ch_hints = (ch_hints & ~ QIO_METHODMASK ) | QIO_METHOD_MEMORY;
  }
  if( memory && reopen ) return;
  if( (ch_hints & QIO_METHODMASK) == QIO_METHOD_FREADFWRITE ) {
    if( (file_hints & QIO_METHODMASK) != QIO_METHOD_FREADFWRITE ) return;
  }
  if( (ch_hints & QIO_METHODMASK) == QIO_METHOD_MMAP ) {
    if( (file_hints & QIO_METHODMASK) != QIO_METHOD_MMAP ) return;
  }

  fhints = qio_hints_to_string(file_hints);
  chhints = qio_hints_to_string(ch_hints);

  int idx = -1;
  if (TEST_RE == test->kind)
  {
    idx = ((re_test_case*)test)->idx;
  }
  else if (TEST_KEEP == test->kind)
  {
    idx = ((keep_test_case*)test)->idx;
  }
  else
  {
    printf("unknown test kind: %d\n", test->kind);
  }

  printf("check_channel(file_hints=%i, ch_hints=%i, unbounded=%i, reopen=%i, discard_mask=%x, allow_buffer=%i, test_case_index=%i)\n",
         file_hints,
         ch_hints,
         (int) unbounded_channels,
         (int) reopen,
         discard_check_mask,
         allow_buffer_search,
	 idx );
  qio_free(fhints);
  qio_free(chhints);

  printf("[%d] '%s' vs '%s'\n", test->kind, test->data, test->pattern);

  if( memory ) {
    err = qio_file_open_mem_ext(&f, NULL, (qio_fdflag_t)(QIO_FDFLAG_READABLE|QIO_FDFLAG_WRITEABLE|QIO_FDFLAG_SEEKABLE), file_hints, NULL);
    assert(!err);
  } else {
    // Open a temporary file.
    if( reopen ) {
      fd = mkstemp(filename);
      close(fd);
      err = qio_file_open_access(&f, filename, "w", file_hints, NULL);
      assert(!err);
    } else {
      err = qio_file_open_tmp(&f, file_hints, NULL);
      assert(!err);
    }

    // Rewind the file
    if (f->fp ) {
      int got;
      got = fseek(f->fp, start, SEEK_SET);
      assert( got == 0 );
    } else {
      off_t off;
      int syserr;

      syserr = sys_lseek(f->fd, start, SEEK_SET, &off);
      assert(!syserr);
    }
  }

  // Create a "write to file" channel.
  err = qio_channel_create(&writing, f, ch_hints, 0, 1, start, ch_end, NULL);
  assert(!err);
  // Write stuff to the file.
  err = qio_channel_write_amt(threadsafe,writing,test->data,strlen(test->data));
  assert(!err);

  qio_channel_release(writing);

  // Reopen the file if we're doing reopen
  if( reopen ) {
    // Close the file.
    qio_file_release(f);
    err = qio_file_open_access(&f, filename, "r", file_hints, NULL);
    assert(!err);
  }

  // That was fun. Now start at the beginning of the file
  // and read the data.

  int64_t test_start=0;
  if (TEST_RE == test->kind)
  {
    test_start = ((re_test_case*)test)->start;
  }


  // Rewind the file to the start of what we should read.
  if( !memory ) {
    off_t off;
    int syserr;

    syserr = sys_lseek(f->fd, test_start, SEEK_SET, &off);
    assert(!syserr);
  }

  // Read the data.
  //err = qio_channel_init_file(&reading, type, f, ch_hints, 1, 0, start, end);
  err = qio_channel_create(&reading, f, ch_hints, 1, 0, test_start, ch_end, NULL);
  assert(!err);

  // Read stuff from the file.
  // TODO -- do regular expression search instead.

  {
    qio_regexp_t compiled;
    const char* pattern = test->pattern;
    const char* flags = "";
    bool isUtf8 = false;
    // maximum to match a RE to
    int64_t maxlen = std::numeric_limits<int64_t>::max();
    // TODO -- try different anchorings.
    int anchor = QIO_REGEXP_ANCHOR_UNANCHORED; // QIO_REGEXP_ANCHOR_START, QIO_REGEXP_ANCHOR_BOTH
    bool can_discard = true;
    bool keep_unmatched = false;
    bool keep_whole_pattern = false;
    qio_regexp_string_piece_t submatches[1024];

    if (TEST_KEEP == test->kind)
    {
      keep_test_case *keep_test = (keep_test_case*)test;
      can_discard = true;
      keep_unmatched = (keep_test->keep_flags & KEEP_UNMATCHED) > 0;
      keep_whole_pattern = (keep_test->keep_flags & KEEP_WHOLE_PATTERN) > 0;
    }
 

    qio_regexp_create_compile_flags(pattern, strlen(pattern), flags, strlen(flags), isUtf8, &compiled);

    qio_channel_mark(threadsafe, reading);

    err = qio_regexp_channel_match(&compiled, threadsafe, reading, maxlen, anchor, can_discard, keep_unmatched, keep_whole_pattern, submatches, 1024);
    // TODO -- check ENOERR if we matched or EFORMAT if we didn't
    //         possibly check submatches

    //assert(err == 0);
    if (TEST_RE == test->kind)
    {
      re_test_case *re_test = (re_test_case*)test;

      assert(re_test->first_offset == submatches[0].offset);
      assert(re_test->length == submatches[0].len);

      for(int i=0; i < re_test->n_captures; i++)
      {
        assert(re_test->captures[i].match_offset == submatches[i+1].offset);
        assert(re_test->captures[i].match_length == submatches[i+1].len);
      }

      if( re_test->first_offset == -1 ) {
        assert( qio_err_to_int(err) == EFORMAT );
      } else {
        assert(!err);
      }

      qio_channel_commit(threadsafe, reading);
    }
    else if (TEST_KEEP == test->kind)
    {
      keep_test_case *keep_test = (keep_test_case*)test;
      int64_t offset;

      qio_channel_revert_unlocked(reading);
      offset = qio_channel_offset_unlocked(reading);

      //printf("looking for offset %lli and found %lli\n", 
      //        (long long)keep_test->min_channel_offset, (long long)offset);
      
      // With a buffered search, we don't care if we
      // kept more than we should have since it would
      // be all in memory anyway.
      assert(allow_buffer_search || keep_test->min_channel_offset <= offset);
      assert(offset <= keep_test->max_channel_offset);
    }
   
  }

  qio_channel_release(reading);

  // Close the file.
  qio_file_release(f);

  if( reopen ) {
    unlink(filename);
  }

}

void check_re_channels(void)
{
  char unbounded;
  char reopen;
  qio_hint_t hints[] = {QIO_METHOD_DEFAULT, QIO_METHOD_READWRITE, QIO_METHOD_PREADPWRITE, QIO_METHOD_FREADFWRITE, QIO_METHOD_MEMORY, QIO_METHOD_MMAP, QIO_METHOD_MMAP|QIO_HINT_PARALLEL, QIO_METHOD_PREADPWRITE|QIO_HINT_NOFAST};
  int discard_check_mask[] = {1, 3, 0xfff};
  int ndiscard_check_masks = sizeof(discard_check_mask)/sizeof(int);
  int discard_check;
  int allow_buffer_search;
  int nhints = sizeof(hints)/sizeof(qio_hint_t);
  int file_hint, ch_hint;

  check_channel(0,(qio_chtype_t)3, 0, 0, 1, 0, (base_test_case*)&keep_test_cases[20]);
  //check_channel(0,(qio_chtype_t)1,7,1,1,QIO_METHOD_DEFAULT,QIO_METHOD_DEFAULT,1,0,1,0);
  /*
  check_channel(0, (qio_chtype_t)3, 1, 0, QIO_METHOD_DEFAULT,
          0, 0, 1, 0, &re_test_cases[2]);
  check_channel(0, (qio_chtype_t)3, 1, 0, QIO_METHOD_READWRITE,
          0, 0, 1, 0, &re_test_cases[2]);

  printf("bailing early\n");
  exit(0);
  // */




  for( file_hint = 0; file_hint < nhints; file_hint++ ) {
    for( ch_hint = 0; ch_hint < nhints; ch_hint++ ) {
      for( unbounded = 0; unbounded < 2; unbounded++ ) {
        for( reopen = 0; reopen < 2; reopen++ ) {
          for(allow_buffer_search=0; allow_buffer_search < 2; allow_buffer_search++){
            for(int tc=0; tc < n_re_test_cases; tc++)
            {
              re_test_cases[tc].idx = tc;

              for(discard_check = 0; discard_check < ndiscard_check_masks; discard_check++){

                check_channel(hints[file_hint], hints[ch_hint], 
                        unbounded, reopen, discard_check_mask[discard_check],
                        allow_buffer_search, (base_test_case*)&re_test_cases[tc]);
              }
            }

            for(int tc=0; tc < n_keep_test_cases; tc++)
            {
              keep_test_cases[tc].idx = tc;

              check_channel(hints[file_hint], hints[ch_hint], 
                      unbounded, reopen, 1,
                      allow_buffer_search, (base_test_case*)&keep_test_cases[tc]);
            }
          }
        }
      }
    }
  }
}

int main(int argc, char** argv)
{
  // use smaller mmap chunks for testing.
  qio_mmap_chunk_iobufs = 1;

  // use smaller qbytes_iobuf_size for testing
  qbytes_iobuf_size = 4*1024;

  assert(RE2::FullMatch("hello", "h.*o"));
  check_re_channels();
  return 0;
}

