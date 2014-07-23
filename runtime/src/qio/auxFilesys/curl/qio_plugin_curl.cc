#define QIOPLUGIN_CURL_C

#ifndef _DARWIN_C_SOURCE
// get fcntl(F_GETPATH)
#define _DARWIN_C_SOURCE
#endif

#ifndef _GNU_SOURCE
// get O_DIRECT, fopencookie
#define _GNU_SOURCE
#endif

#include <string.h>

#ifndef SIMPLE_TEST
#include "chplrt.h"
#endif

#include "qio_plugin_hdfs.h"
#include "qio_plugin_curl.h"
#include <curl/curl.h>

#define min(a,b) a < b ? a : b
#undef exit
#define TCH(f) ((curl_handle*)f)

struct str_t {
  char*  mem;
  size_t size;
};

struct curl_handle {
  CURL*       curl;           // Curl handle
  const char* pathnm;         // Path/URL (etc.)
  ssize_t     length;         // length of what we are reading, -1 if we are not able to get the length
  size_t      seek_offset;    // Offset to seek to, set in curl_seek
  size_t      current_offset; // The current offset in the file
  int         seekable;       // Can we request byteranges from this URL?
};

struct curl_iovec_t {
  struct  iovec* vec; // iovec to read into
  size_t  total_read; // total amount read
  size_t  amt_read;   // How much we have read into the current iovec buffer
  int     count;      // number of buffers in the iovec
  size_t  offset;     // offset that we want to skip to (in the case where we cannot request byteranges)
  int     curr;       // the index of the current buffer
};

#define TCH(f) ((curl_handle*)f)
#define min(a,b) a < b ? a : b

static
size_t buf_writer_no_seek(char* ptr_data, size_t size, size_t nmemb, void* userdata)
{
  size_t realsize = size*nmemb;
  size_t real_realsize = realsize;
  struct curl_iovec_t* ret = ((struct curl_iovec_t *)userdata);


  // so that we can "seek"
  if (realsize < ret->offset) {
    ret->offset -= realsize;
    return realsize;
  } else {
    // nop when we are done
    ptr_data = &(ptr_data[ret->offset]);
    realsize -= ret->offset;
    ret->offset = 0;
  }

  // we need to go from one iovbuff to the other
  while (realsize > ret->vec[ret->curr].iov_len*size - ret->amt_read)  {
    memcpy(&(((char*)ret->vec[ret->curr].iov_base)[ret->amt_read]), ptr_data, ret->vec[ret->curr].iov_len*size - ret->amt_read);
    ret->total_read += (ret->vec[ret->curr].iov_len*size - ret->amt_read);
    realsize -= ret->vec[ret->curr].iov_len*size - ret->amt_read;
    ptr_data = &(ptr_data[(ret->vec[ret->curr].iov_len*size - ret->amt_read)]);
    ret->amt_read = 0;
    if (ret->curr == ret->count-1) {
      return 0;
    } else {
      ret->curr++;
    }
  }

  // we won't be able to fully fill everything up at this point
  if (realsize <= (ret->vec[ret->curr].iov_len*size - ret->amt_read)) {
    memcpy(&(((char*)ret->vec[ret->curr].iov_base)[ret->amt_read]), ptr_data, realsize);
    ret->total_read += realsize;
    ret->amt_read += realsize;
    if (ret->vec[ret->curr].iov_len*size == ret->amt_read) {
      if (ret->curr == ret->count -1)
        return 0;
      else {
        ret->curr++;
        ret->amt_read = 0;
      }
    }
  }
  return real_realsize;
}

static
size_t buf_writer_seekable(char* ptr_data, size_t size, size_t nmemb, void* userdata)
{
  size_t realsize = size*nmemb;
  size_t real_realsize = realsize;
  struct curl_iovec_t* ret = ((struct curl_iovec_t *)userdata);

  // we need to go from one iovbuff to the other
  while (realsize > ret->vec[ret->curr].iov_len*size - ret->amt_read)  {
    memcpy(&(((char*)ret->vec[ret->curr].iov_base)[ret->amt_read]), ptr_data, ret->vec[ret->curr].iov_len*size - ret->amt_read);
    ret->total_read += (ret->vec[ret->curr].iov_len*size - ret->amt_read);
    realsize -= ret->vec[ret->curr].iov_len*size - ret->amt_read;
    ptr_data = &(ptr_data[(ret->vec[ret->curr].iov_len*size - ret->amt_read)]);
    ret->amt_read = 0;
    if (ret->curr == ret->count-1) {
      return 0;
    } else {
      ret->curr++;
    }
  }

  // we won't be able to fully fill this buf up at this point
  if (realsize <= (ret->vec[ret->curr].iov_len*size - ret->amt_read)) {
    memcpy(&(((char*)ret->vec[ret->curr].iov_base)[ret->amt_read]), ptr_data, realsize);
    ret->total_read += realsize;
    ret->amt_read += realsize;
    if (ret->vec[ret->curr].iov_len*size == ret->amt_read) {
      if (ret->curr == ret->count -1)
        return 0;
      else {
        ret->curr++;
        ret->amt_read = 0;
      }
    }
  }
  return real_realsize;
}



static
size_t read_data(void* ptr, size_t size, size_t nmemb, void* userp)
{
  struct iovec* src = (struct iovec*)userp;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  memcpy(ptr, src->iov_base, src->iov_len);
  return src->iov_len;
}

// TODO: have this keep track of where we are and if we need to seek someplace
static
size_t chpl_curl_write(void* ptr, size_t size, size_t nmemb, void* fd_ptr)
{
  return write(*((int*)(intptr_t)fd_ptr), ptr, size*nmemb);
}

static
size_t chpl_curl_write_string(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct str_t *str = (struct str_t *)userp;


  str->mem = (char*)qio_realloc(str->mem, str->size + realsize + 1);
  if(str->mem == NULL) {
    return 0;
  }

  memcpy(&(str->mem[str->size]), contents, realsize);
  str->size += realsize;
  str->mem[str->size] = 0;

  return realsize;
}


qioerr curl_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs)
{
  CURLcode err;
  ssize_t got_total;
  qioerr err_out = 0;
  struct curl_iovec_t write_vec;


  STARTING_SLOW_SYSCALL;

  got_total = 0;
  write_vec.total_read = 0;
  write_vec.count = count;
  write_vec.vec = (struct iovec*)vector;
  write_vec.amt_read = 0;
  write_vec.offset = 0;
  write_vec.curr = 0;

  if (TCH(file)->seekable != -1) { // we can request byteranges
    curl_easy_setopt(TCH(file)->curl, CURLOPT_WRITEFUNCTION, buf_writer_seekable);
    curl_easy_setopt(TCH(file)->curl, CURLOPT_RESUME_FROM_LARGE, TCH(file)->seek_offset);
  } else {
    write_vec.offset = TCH(file)->seek_offset;
    curl_easy_setopt(TCH(file)->curl, CURLOPT_WRITEFUNCTION, buf_writer_no_seek);
  }

  // Read into write_vec
  curl_easy_setopt(TCH(file)->curl, CURLOPT_WRITEDATA, &write_vec);

  // Read our data into the buf
  err = curl_easy_perform(TCH(file)->curl);
  DONE_SLOW_SYSCALL;

  got_total = write_vec.total_read;

  if ((err == CURLE_RANGE_ERROR ||  err == CURLE_OK) && got_total == 0 && sys_iov_total_bytes(vector, count) != 0)
    err_out = qio_int_to_err(EEOF);

  *num_read_out = got_total;
  return err_out;
}

qioerr curl_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs)
{
  CURLcode err;
  ssize_t got_total;
  qioerr err_out = 0;
  struct curl_iovec_t write_vec;


  STARTING_SLOW_SYSCALL;

  got_total = 0;
  write_vec.total_read = 0;
  write_vec.count = count;
  write_vec.vec = (struct iovec*)vector;
  write_vec.amt_read = 0;
  write_vec.offset = 0;
  write_vec.curr = 0;

  if (TCH(file)->seekable != -1) { // we can request byteranges
    curl_easy_setopt(TCH(file)->curl, CURLOPT_WRITEFUNCTION, buf_writer_seekable);
    curl_easy_setopt(TCH(file)->curl, CURLOPT_RESUME_FROM_LARGE, offset);
  } else {
    write_vec.offset = offset;
    curl_easy_setopt(TCH(file)->curl, CURLOPT_WRITEFUNCTION, buf_writer_no_seek);
  }

  // Read into write_vec
  curl_easy_setopt(TCH(file)->curl, CURLOPT_WRITEDATA, &write_vec);

  // Read our data into the buf
  err = curl_easy_perform(TCH(file)->curl);
  DONE_SLOW_SYSCALL;

  got_total = write_vec.total_read;

  if ((err == CURLE_RANGE_ERROR ||  err == CURLE_OK) && got_total == 0 && sys_iov_total_bytes(vector, count) != 0)
    err_out = qio_int_to_err(EEOF);

  *num_read_out = got_total;
  return err_out;
}

qioerr curl_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t offset, ssize_t* num_read_out, void* fs)
{
  qioerr t = 0;
  QIO_GET_CONSTANT_ERROR(t, ENOSYS, "pwrites in CURL are not supported -- for now");
  fprintf(stderr, "positional writes in CURL are not supported -- for now\n");
  return t;
}

qioerr curl_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs)
{
  CURLcode ret;
  ssize_t got = 0;
  ssize_t got_total = 0;
  qioerr err_out = 0;
  int i;


  STARTING_SLOW_SYSCALL;
    /*tell it to "upload" to the URL*/
    curl_easy_setopt(TCH(fl)->curl, CURLOPT_UPLOAD, 1L);
    // set it up to write over curl
    curl_easy_setopt(TCH(fl)->curl, CURLOPT_READFUNCTION, read_data);

  for (i = 0; i < iovcnt; i++) {
    curl_easy_setopt(TCH(fl)->curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)iov[i].iov_len);
    curl_easy_setopt(TCH(fl)->curl, CURLOPT_READDATA, &(iov[i]));
    ret = curl_easy_perform(TCH(fl)->curl);
    got += iov[i].iov_len;

    if (ret != CURLE_OK)
      err_out = qio_mkerror_errno();

    if (got != -1)
      got_total += got;
    else {
      err_out = qio_mkerror_errno();
      break;
    }

    if (got != sys_iov_total_bytes(&iov[i], iov[i].iov_len))
      break;
  }
  // no longer set as write
  curl_easy_setopt(TCH(fl)->curl, CURLOPT_UPLOAD, 0L);

  *num_written_out = got_total;
  DONE_SLOW_SYSCALL;

  return err_out;
}

static
int curl_seekable(void* file)
{
  struct str_t buf;
  int ret = 0;


  buf.mem = (char*)qio_calloc(1, 1); // doesn't really matter, we just want a place on the heap
  buf.size = 0;

  curl_easy_setopt(TCH(file)->curl, CURLOPT_WRITEFUNCTION, chpl_curl_write_string);
  curl_easy_setopt(TCH(file)->curl, CURLOPT_HEADERDATA, &buf);
  curl_easy_setopt(TCH(file)->curl, CURLOPT_NOBODY, 1L);

  curl_easy_perform(TCH(file)->curl);

  curl_easy_setopt(TCH(file)->curl, CURLOPT_NOBODY, 0L);
  curl_easy_setopt(TCH(file)->curl, CURLOPT_WRITEFUNCTION, NULL);
  curl_easy_setopt(TCH(file)->curl, CURLOPT_HEADERDATA, NULL);

  if (strstr(buf.mem, "Accept-Ranges") == NULL)
      ret = -1;
  else
      ret = 1;

  qio_free(buf.mem);
  return ret;
}

qioerr curl_open(void** fd, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs)
{
  qioerr err_out = 0;
  int rc = 0;
  // Why curl wants this to be a double is beyond me...
  double filelength;
  curl_handle* fl = (curl_handle*)qio_calloc(sizeof(curl_handle), 1);


  STARTING_SLOW_SYSCALL;

  // assert that we connected

  TCH(fl)->curl =  curl_easy_init();

  // Assert that we opened the file
  if (TCH(fl)->curl == NULL) {
    QIO_GET_CONSTANT_ERROR(err_out, ECONNREFUSED, "Unable to connect to Curl");
    goto error;
  }

  // set URL
  curl_easy_setopt(TCH(fl)->curl, CURLOPT_URL, path);

  // Read the header in order to get the length of the thing we are reading
  curl_easy_setopt(TCH(fl)->curl, CURLOPT_HEADER, 0L);
  curl_easy_setopt(TCH(fl)->curl, CURLOPT_NOBODY, 1L);
  curl_easy_perform(TCH(fl)->curl);
  curl_easy_getinfo(TCH(fl)->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &filelength);
  curl_easy_setopt(TCH(fl)->curl, CURLOPT_NOBODY, 0L); // now we want to read the body

  DONE_SLOW_SYSCALL;

  rc = *flags | ~O_ACCMODE;
  rc &= O_ACCMODE;
  if( rc == O_RDONLY ) {
    *flags |= QIO_FDFLAG_READABLE;
  } else if( rc == O_WRONLY ) {
    *flags |= QIO_FDFLAG_WRITEABLE;
  } else if( rc == O_RDWR ) {
    *flags |= QIO_FDFLAG_READABLE;
    *flags |= QIO_FDFLAG_WRITEABLE;
  }

  TCH(fl)->pathnm = path;
  TCH(fl)->length = (size_t)filelength;
  TCH(fl)->seekable = curl_seekable(fl);
  TCH(fl)->seek_offset = 0;
  TCH(fl)->current_offset = 0;
  *fd = fl;
  return err_out;

error:
  curl_easy_cleanup(*fd);
  return err_out;
}

qioerr curl_close(void* fl, void* fs)
{
  qioerr err_out = 0;


  STARTING_SLOW_SYSCALL;
  curl_easy_cleanup(TCH(fl)->curl);
  qio_free(fl);
  DONE_SLOW_SYSCALL;

  return err_out;
}

// Note: libcurl really doesn't support seeking. See:
// http://curl.haxx.se/mail/lib-2009-05/0085.html
// So instead, we keep track of where we need to seek to, in order to replicate the
// correct behaviour
qioerr curl_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs)
{
  curl_handle* curl_local = TCH(fl);


  switch (whence) {
    case SEEK_CUR:
      curl_local->seek_offset = curl_local->current_offset + offset;
      break;
    case SEEK_END:
      if (curl_local->length != -1)  // we have the length
        curl_local->seek_offset = curl_local->length + offset;
      else
        QIO_RETURN_CONSTANT_ERROR(ESPIPE, "Unable to SEEK_END for path with unkown length");
      break;
    case SEEK_SET:
      curl_local->seek_offset = offset;
      break;
  }

  *offset_out = curl_local->current_offset + offset;
  return 0;
}

qioerr curl_getpath(void* file, const char** string_out, void* fs)
{

  qioerr err = 0;
  *string_out = TCH(file)->pathnm;
  return err;
}

qioerr curl_getlength(void* fl, int64_t* len_out, void* fs)
{

  qioerr err = 0;
  *len_out = TCH(fl)->length;
  // don't have the length
  if (TCH(fl)->length == 0)
    err = qio_int_to_err(-1);
  return err;
}

// Blech, but in order to be modular, we need to essentially do what the Curl folks
// did...
qioerr chpl_curl_set_opt(qio_file_t* fl, int opt, ...)
{
  qioerr err;
  va_list argp;
  va_start(argp, opt);
  STARTING_SLOW_SYSCALL;
  // This reasoning about what to cast too is pulled from the libcurl source
  if (opt < CURLOPTTYPE_OBJECTPOINT) {
    long tmp = va_arg(argp, long);
    err = qio_int_to_err(curl_easy_setopt(TCH(fl->file_info)->curl, (CURLoption)opt, tmp));
  } else if (opt < CURLOPTTYPE_OFF_T) {
    void* tmp = va_arg(argp, void*);
    err = qio_int_to_err(curl_easy_setopt(TCH(fl->file_info)->curl, (CURLoption)opt, tmp));
  } else {
    curl_off_t tmp = va_arg(argp, curl_off_t);
    err = qio_int_to_err(curl_easy_setopt(TCH(fl->file_info)->curl, (CURLoption)opt, tmp));
  }
  DONE_SLOW_SYSCALL;
  return err;
}

qioerr chpl_curl_perform(qio_file_t* fl)
{
  qioerr err;
  STARTING_SLOW_SYSCALL;
  err = qio_int_to_err(curl_easy_perform(TCH(fl->file_info)->curl));
  DONE_SLOW_SYSCALL;
  return err;
}

// str resides in heap
qioerr chpl_curl_stream_string(qio_file_t* fl, const char** str)
{
  qioerr err = 0;
  struct str_t chunk;
  CURLcode res;

  chunk.mem = (char*)qio_calloc(1, 1); // doesn't really matter, we just want a place on the heap
  chunk.size = 0;

  STARTING_SLOW_SYSCALL;
  curl_easy_setopt(TCH(fl->file_info)->curl, CURLOPT_WRITEFUNCTION, chpl_curl_write_string);
  curl_easy_setopt(TCH(fl->file_info)->curl, CURLOPT_WRITEDATA, (void*)&chunk);
  res = curl_easy_perform(TCH(fl->file_info)->curl);
  DONE_SLOW_SYSCALL;
  *str = chunk.mem;

  if (res != CURLE_OK)
    err = qio_int_to_err(errno);

  return err;
}

// get the fp/fd and stream in
qioerr chpl_curl_stream_file(qio_file_t* fl_curl, qio_file_t* fl_local)
{
  qioerr err = 0;
  STARTING_SLOW_SYSCALL;
  if (fl_local->fp) {
    // curl defaults CURLOPT_WRITEFUNCTION = fwrite
    curl_easy_setopt(TCH(fl_curl->file_info)->curl, CURLOPT_FILE, fl_local->fp);
    err = qio_int_to_err(curl_easy_perform(TCH(fl_curl->file_info)->curl));
    fflush(fl_local->fp);
  } else if (fl_local->fd != -1) {
    curl_easy_setopt(TCH(fl_curl->file_info)->curl, CURLOPT_WRITEFUNCTION, chpl_curl_write);
    curl_easy_setopt(TCH(fl_curl->file_info)->curl, CURLOPT_FILE, (void*)((intptr_t)&fl_local->fd));
    err = qio_int_to_err(curl_easy_perform(TCH(fl_curl->file_info)->curl));
    err = qio_int_to_err(sys_fsync(fl_local->fd));
  } else {
    return err;
  }
  DONE_SLOW_SYSCALL;
  return err;
}

qio_file_functions_t glob_struct = {
    &curl_writev,    //writev
    &curl_readv,     //readv
    NULL,            //pwritev
    &curl_preadv,    //preadv
    &curl_close,     //close
    &curl_open,      //open
    &curl_seek,      //seek
    &curl_getlength, //filelength
    &curl_getpath,   //getpath
    NULL,            //fsync
    NULL,            //getcwd
    /*3,*/           //fs_type
};

const qio_file_functions_ptr_t ptr_glob_struct = &glob_struct;
