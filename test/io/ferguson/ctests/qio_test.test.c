#include "qio.h"
#include <assert.h>
#include <stdio.h>


#ifdef LIMIT_TESTING
int limit_testing = 1;
#else
int limit_testing = 0;
#endif

int verbose = 0;

unsigned char data_at(int64_t offset)
{
  return offset + (offset >> 2) + 'a' + (offset % 7);
}

void fill_testdata(int64_t start, int64_t len, unsigned char* data)
{
  int64_t k;
  for( k = 0; k < len; k++ ) {
    data[k] = data_at(start+k);
  }
}

// unbounded_channels <= 0 means no, > 0 means yes
// -1 means advance.
void check_channel(char threadsafe, qio_chtype_t type, int64_t start, int64_t len, int64_t chunksz, qio_hint_t file_hints, qio_hint_t ch_hints, char unbounded_channels, char reopen, char seek) {
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;
  int64_t offset;
  int64_t usesz;
  int64_t end = start + len;
  qioerr err;
  unsigned char* chunk;
  unsigned char* got_chunk;
  int64_t k;
  ssize_t amt_written;
  ssize_t amt_read;
  char* chhints;
  char* fhints;
  FILE* writefp = NULL;
  FILE* readfp = NULL;
  int memory;
  int64_t ch_end = end;
  char filename[128];
  int fd = -1;

  strcpy(filename,"/tmp/qio_testXXXXXX");

  if( unbounded_channels > 0 ) ch_end = INT64_MAX;

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
  if( memory && type == QIO_CH_ALWAYS_UNBUFFERED ) return;
  if( memory && reopen ) return;
  if( (ch_hints & QIO_METHODMASK) == QIO_METHOD_FREADFWRITE ) {
    if( (file_hints & QIO_METHODMASK) != QIO_METHOD_FREADFWRITE ) return;
  }
  if( (ch_hints & QIO_METHODMASK) == QIO_METHOD_MMAP ) {
    if( (file_hints & QIO_METHODMASK) != QIO_METHOD_MMAP ) return;
  }

  fhints = qio_hints_to_string(file_hints);
  chhints = qio_hints_to_string(ch_hints);
  if( verbose ) {
    printf("check_channel(threadsafe=%i, type=%i, start=%lli, len=%lli, chunksz=%lli, file_hints=%s, ch_hints=%s, unbounded=%i, reopen=%i, seek=%i)\n",
         (int) threadsafe,
         (int) type,
         (long long int) start,
         (long long int) len,
         (long long int) chunksz,
         fhints,
         chhints,
         (int) unbounded_channels,
         (int) reopen,
         (int) seek );
  }
  qio_free(fhints);
  qio_free(chhints);

  chunk = qio_malloc(chunksz);
  got_chunk = qio_malloc(chunksz);

  assert(chunk);
  assert(got_chunk);

  if( memory ) {
    err = qio_file_open_mem_ext(&f, NULL, QIO_FDFLAG_READABLE|QIO_FDFLAG_WRITEABLE|QIO_FDFLAG_SEEKABLE, file_hints, NULL);
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
  for( offset = start; offset < end; offset += usesz ) {

    if (seek && !writefp) {
      err = qio_channel_seek(writing, offset, ch_end);
      assert(!err);
      assert(qio_channel_offset_unlocked(writing) == offset);
    }

    usesz = chunksz;
    if( offset + usesz > end ) usesz = end - offset;
    // Fill chunk.
    fill_testdata(offset, usesz, chunk);
    // Write chunk.
    if( writefp ) {
      amt_written = fwrite(chunk, 1, usesz, writefp);
    } else {
      err = qio_channel_write(threadsafe, writing, chunk, usesz, &amt_written);
      assert(!err);
      assert(qio_channel_offset_unlocked(writing) == offset+usesz);
    }
    assert(amt_written == usesz);

  }

  // Attempt to write 1 more byte; we should get EEOF
  // if we've restricted the range of the channel.
  // Write chunk.
  if( unbounded_channels > 0) {
    // do nothing
  } else {
    if( writefp ) {
      int got;
      got = fflush(writefp);
      assert(got == 0);
      amt_written = fwrite(chunk, 1, 1, writefp);
      // fwrite might buffer on its own.
      if( amt_written != 0 ) {
        got = fflush(writefp);
        assert(got == EOF);
      }
      assert(errno == EEOF);
    } else {
      if( unbounded_channels < 0 ) {
        int times = -unbounded_channels;
        for( int z = 0; z < times; z++ ) {
          err = qio_channel_advance(threadsafe, writing, 1);
          assert( !err );
        }
      }

      err = qio_channel_write(threadsafe, writing, chunk, 1, &amt_written);
      assert(amt_written == 0);
      assert( qio_err_to_int(err) == EEOF );
    }
  }

  qio_channel_release(writing);

  // Reopen the file if we're doing reopen
  if( reopen ) {
    // Close the file.
    qio_file_release(f);
    err = qio_file_open_access(&f, filename, "r", file_hints, NULL);
    assert(!err);
  }

  // Check that the file is the right length.
  if( !memory ) {
    struct stat stats;
    int syserr;
    syserr = sys_fstat(f->fd, &stats);
    assert(!syserr);
    if( stats.st_size != end ) {
      printf("File is the wrong length. Length is %i but expected %i\n",
             (int) stats.st_size, (int) end);
      printf("File path is %s\n", filename);
      assert(stats.st_size == end);
    }
  }

  // That was fun. Now start at the beginning of the file
  // and read the data.

  // Rewind the file
  if( !memory ) {
    off_t off;
    int syserr;

    syserr = sys_lseek(f->fd, start, SEEK_SET, &off);
    assert(!syserr);
  }

  // Read the data.
  //err = qio_channel_init_file(&reading, type, f, ch_hints, 1, 0, start, end);
  err = qio_channel_create(&reading, f, ch_hints, 1, 0, start, ch_end, NULL);
  assert(!err);

  // Read stuff from the file.
  for( offset = start; offset < end; offset += usesz ) {

    usesz = chunksz;
    if( offset + usesz > end ) usesz = end - offset;
    // Fill chunk.
    fill_testdata(offset, usesz, chunk);
    memset(got_chunk, 0xff, usesz);
    // Read chunk.
    if( readfp ) {
      amt_read = fread(got_chunk, 1, usesz, readfp);
    } else {
      int errcode;
      err = qio_channel_read(threadsafe, reading, got_chunk, usesz, &amt_read);
      errcode = qio_err_to_int(err);
      assert( errcode == EEOF || errcode == 0);
      assert(qio_channel_offset_unlocked(reading) == offset+usesz);
    }
    assert(amt_read == usesz);

    // Compare chunk.
    for( k = 0; k < usesz; k++ ) {
      assert(got_chunk[k] == chunk[k]);
    }

    if (seek && !readfp) {
      err = qio_channel_seek(reading, offset+usesz, ch_end);
      assert(!err);
      assert(qio_channel_offset_unlocked(reading) == offset+usesz);
    }
  }

  // Check that reading later gives EEOF.
  if( readfp ) {
    amt_read = fread(got_chunk, 1, 1, readfp);
    assert( amt_read == 0 );
    assert( feof(readfp) );
  } else {
    if( unbounded_channels < 0 ) {
      int times = -unbounded_channels;
      for( int z = 0; z < times; z++ ) {
        err = qio_channel_advance(threadsafe, reading, 1);
        assert( !err || qio_err_to_int(err) == EEOF );
      }

      err = qio_channel_advance(threadsafe, reading, -unbounded_channels);
      assert( !err || qio_err_to_int(err) == EEOF );
    }

    err = qio_channel_read(threadsafe, reading, got_chunk, 1, &amt_read);
    if( qio_err_to_int(err) != EEOF ) {
      printf("HERE read something at offset %i \n", (int) qio_channel_offset_unlocked(reading));
      printf("read %x\n", (int) got_chunk[0]);
      assert( qio_err_to_int(err) == EEOF );
    }
  }

  qio_channel_release(reading);
  //err = qio_channel_destroy(&reading);

  // Close the file.
  qio_file_release(f);

  if( reopen ) {
    unlink(filename);
  }

  qio_free(chunk);
  qio_free(got_chunk);
}

void check_channels(void)
{
  int s, i, k;
  int64_t starts[] = {0, 7};
  int nstarts = sizeof(starts)/sizeof(int64_t);
  int64_t lens[] = {1, 2, 3, qbytes_iobuf_size + 13, 4 * qbytes_iobuf_size};
  int nlens = sizeof(lens)/sizeof(int64_t);
  int64_t chunkszs[] = {1, 7, 16, qbytes_iobuf_size + 13, 2 * qbytes_iobuf_size};
  int nchunkszs = sizeof(chunkszs)/sizeof(int64_t);
  qio_chtype_t type;
  char threadsafe;
  char unboundedness[] = {0,1,-1,-2};
  int nunbounded = sizeof(unboundedness)/sizeof(char);
  int unbounded;
  char reopen;
  char seek;
  qio_hint_t hints[] = {QIO_METHOD_DEFAULT, QIO_METHOD_READWRITE, QIO_METHOD_PREADPWRITE, QIO_METHOD_FREADFWRITE, QIO_METHOD_MEMORY, QIO_METHOD_MMAP, QIO_METHOD_MMAP|QIO_HINT_PARALLEL, QIO_METHOD_PREADPWRITE | QIO_HINT_NOFAST};
  int nhints = sizeof(hints)/sizeof(qio_hint_t);
  int file_hint, ch_hint;

  if( limit_testing ) nlens = 4;
  if( limit_testing ) nchunkszs = 4;

  for( file_hint = 0; file_hint < nhints; file_hint++ ) {
    for( ch_hint = 0; ch_hint < nhints; ch_hint++ ) {
      for( i = 0; i < nlens; i++ ) {
        for( s = 0; s < nstarts; s++ ) {
          for( k = 0; k < nchunkszs; k++ ) {
            for( type = 1; type <= QIO_CH_MAX_TYPE; type++ ) {
              for( threadsafe = 0; threadsafe < 2; threadsafe++ ) {
                check_channel(threadsafe, type, starts[s], lens[i],
                    chunkszs[k], hints[file_hint], hints[ch_hint],
                    unboundedness[0], 0, 0);
              }
            }
          }
        }
      }
    }
    // only test default chanel hints with limit_testing
    // moving over file hints should still give us good coverage.
    if( limit_testing ) break;
  }

  // check with reopen/seek variations while limiting other configurations
  for( file_hint = 0; file_hint < nhints; file_hint++ ) {
    ch_hint = file_hint;
    for( i = 0; i < nlens; i++ ) {
      for( s = 0; s < nstarts; s++ ) {
        for( k = 0; k < nchunkszs; k++ ) {
          type = QIO_CH_BUFFERED;
          threadsafe = 0;
          for( unbounded = 0; unbounded < nunbounded; unbounded++ ) {
            for( reopen = 0; reopen < 2; reopen++ ) {
              for( seek = 0; seek < 2; seek++ ) {
                if (unbounded == 0 && reopen == 0 && seek == 0)
                  continue; //handled in above loop

                check_channel(threadsafe, type, starts[s], lens[i],
                    chunkszs[k], hints[file_hint], hints[ch_hint],
                    unboundedness[unbounded], reopen, seek);
              }
            }
          }
        }
      }
    }
  }

  return;

  /*
  for( file_hint = 0; file_hint < nhints; file_hint++ ) {
    printf("Checking very large channel with hints %x\n", hints[file_hint]);
    // Check a very large file.
    check_channel(0, QIO_CH_BUFFERED, 0, 5L*1024L*1024L*1024L, 1024*1024, hints[file_hint], hints[file_hint], 0, 0);
    check_channel(0, QIO_CH_BUFFERED, 0, 5L*1024L*1024L*1024L, 1024*1024, hints[file_hint], hints[file_hint], 1, 0);
    check_channel(0, QIO_CH_BUFFERED, 0, 5L*1024L*1024L*1024L, 1024*1024, hints[file_hint], hints[file_hint], 0, 1);
    check_channel(0, QIO_CH_BUFFERED, 0, 5L*1024L*1024L*1024L, 1024*1024, hints[file_hint], hints[file_hint], 1, 1);
  }*/
}

// Check some path functions.
void check_paths(void)
{
  const char* tmp = NULL;
  qio_relative_path(&tmp, "/", "/tmp/foo");
  assert(0==strcmp(tmp, "tmp/foo"));
  qio_free((void*)tmp);
  qio_relative_path(&tmp, "", "/tmp/foo");
  assert(0==strcmp(tmp, "tmp/foo"));
  qio_free((void*)tmp);
  qio_relative_path(&tmp, "/bar/bre/", "/bar/bre/tmp/foo");
  assert(0==strcmp(tmp, "tmp/foo"));
  qio_free((void*)tmp);
  qio_relative_path(&tmp, "/bar/bre", "/bar/bre/tmp/foo");
  assert(0==strcmp(tmp, "tmp/foo"));
  qio_free((void*)tmp);
  qio_relative_path(&tmp, "/a/b/c/d/e", "/a/b/x/y");
  assert(0==strcmp(tmp, "../../../x/y"));
  qio_free((void*)tmp);
  qio_relative_path(&tmp, "/a/b/c/d/e/", "/a/b/x/y");
  assert(0==strcmp(tmp, "../../../x/y"));
  qio_free((void*)tmp);
  qio_relative_path(&tmp, "/a/b/xc/d/e/", "/a/b/x/y");
  assert(0==strcmp(tmp, "../../../x/y"));
  qio_free((void*)tmp);
  qio_relative_path(&tmp, "/a/b/c/d/e/", "/a/b/cx/y");
  assert(0==strcmp(tmp, "../../../cx/y"));
  qio_free((void*)tmp);
  qio_relative_path(&tmp, "/home/mppf/chapel/svn_ferguson/test/types/file", "/home/mppf/chapel/svn_ferguson/test/types/file/freadNoInt.txt");
  assert(0==strcmp(tmp, "freadNoInt.txt"));
  qio_free((void*)tmp);
  qio_relative_path(&tmp, "/home/mppf/chapel/svn_ferguson/test/types/file/", "/home/mppf/chapel/svn_ferguson/test/types/file/freadNoInt.txt");
  assert(0==strcmp(tmp, "freadNoInt.txt"));
  qio_free((void*)tmp);



}

int main(int argc, char** argv)
{

  if( argc != 1 ) verbose = 1;

  // use smaller mmap chunks for testing.
  qio_mmap_chunk_iobufs = 1;

  // use smaller qbytes_iobuf_size for testing
  qbytes_iobuf_size = 4*1024;

  check_paths();

  check_channels();


  printf("qio_test PASS\n");

  return 0;
}

