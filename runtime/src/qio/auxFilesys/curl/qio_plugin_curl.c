/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "qio_plugin_curl.h"
#include <curl/curl.h>

#define to_curl_handle(f) ((curl_handle*)f)

struct str_t {
  char*  mem;
  size_t size;
};

// Since a curl handle does not hold where it has read to, we need to do this here.
// As well, since we can many times request byte-ranges (for HTTP/HTTPS) we keep
// track of that here as well.
struct curl_handle {
  CURL*       curl;           // Curl handle
  const char* pathnm;         // Path/URL (etc.)
  ssize_t     length;         // length of what we are reading, -1 if we are not able to get the length
  // Note: This is racy if we open up more than one channel on a given curl handle.
  //       It is expected of the user that they will not open more than one channel at once
  //       on a given Curl handle and use openreader() and openwriter() in order to open up
  //       channels for Curl.
  size_t      current_offset; // The current offset in the file
  int         seekable;       // Can we request byteranges from this URL?
};

// Since the callback is called many times from a call to curl_easy_perform, and
// since we know the amount that we need to read into the iovec passed into
// preadv/readv resp. we put the entire iovec (that is passed into readv/preadv) into
// the vec field of this struct. This way, we only call curl_easy_perform once
// (and thus, get rid of the overhead that multiple calls could cause).
struct curl_iovec_t {
  struct  iovec* vec; // iovec to read into -- This is the iovec that is passed into curl_readv/curl_preadv
  size_t  total_read; // total amount read
  size_t  amt_read;   // How much we have read into the current iovec buffer
  int     count;      // number of buffers in the iovec
  size_t  offset;     // offset that we want to skip to (in the case where we cannot request byteranges)
  int     curr;       // the index of the current buffer
};

// userdata, is a curl_iovec_t. This is set to be passed into this function, when we
// call CURLOPT_WRITEDATA in curl_preadv and curl_readv.
// FUTURE: If we have filled the iovec, but have not finished reading from the curl
// handle, pause it (i.e., return CURL_WRITE_PAUSE).
static
size_t buf_writer(char* ptr_data, size_t size, size_t nmemb, void* userdata)
{
  size_t realsize = size*nmemb;
  size_t real_realsize = realsize;
  struct curl_iovec_t* ret = ((struct curl_iovec_t *)userdata);

  // so that we can "seek" when we cannot request byteranges
  if (realsize < ret->offset) {
    ret->offset -= realsize;
    return realsize;
  } else {
    // nop when we are done -- non-zero the first time through
    ptr_data = &(ptr_data[ret->offset]);
    realsize -= ret->offset;
    ret->offset = 0;
  }

  // The amount that we have been given by curl is more than we can stick into a
  // single iovbuf. So we need to go from one iovbuf to the other
  while (realsize > ret->vec[ret->curr].iov_len*size - ret->amt_read)  {
    // This cast to char* is to get rid "subscript of pointer to incomplete type" warnings
    chpl_memcpy(&(((char*)ret->vec[ret->curr].iov_base)[ret->amt_read]), ptr_data, ret->vec[ret->curr].iov_len*size - ret->amt_read);
    ret->total_read += (ret->vec[ret->curr].iov_len*size - ret->amt_read);
    realsize -= ret->vec[ret->curr].iov_len*size - ret->amt_read;
    ptr_data = &(ptr_data[(ret->vec[ret->curr].iov_len*size - ret->amt_read)]);
    // Reset the amount that we have read into this vector.
    ret->amt_read = 0;
    if (ret->curr == ret->count-1) { // last iovbuf in this vector, so stop reading
      return 0; // stop reading
    } else { // go to the next buf in this vector
      ret->curr++;
    }
  }

  // The amount of data that we have been given by curl is <= to the amount of space
  // that we have left in this iovbuf. So we can simply read it all in.
  if (realsize <= (ret->vec[ret->curr].iov_len*size - ret->amt_read)) {
    chpl_memcpy(&(((char*)ret->vec[ret->curr].iov_base)[ret->amt_read]), ptr_data, realsize);
    ret->total_read += realsize;
    ret->amt_read += realsize;
    // We have fully populated this iovbuf
    if (ret->vec[ret->curr].iov_len*size == ret->amt_read) {
      if (ret->curr == ret->count -1) // last iovbuf in this vector
        return 0; // stop reading
      else { // else, step to the next buf.
        ret->curr++;
        ret->amt_read = 0;
      }
    }
  }
  return real_realsize;
}

// We want to upload data from the iovec passed into curl_writev. We populate a
// curl_iovec_t in curl_writev, and set it to be passed in when we call
// CURLOPT_READDATA. Note, that we can only return a chunk of memory at most
// size*nmemb big to curl (i.e., sizeof(ptr) <= size*nmemb after this function is
// called by CURL).
static
size_t read_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size*nmemb;
  struct curl_iovec_t* ret = ((struct curl_iovec_t *)userp);

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1) || (ret->curr >= ret->count)) {
    return 0; // stop putting data
  }

  // We can upload more than one iovbuf at once, so do it.
  while (realsize > ret->vec[ret->curr].iov_len*size - ret->amt_read)  {
    chpl_memcpy(ptr, &(((char*)ret->vec[ret->curr].iov_base)[ret->amt_read]), ret->vec[ret->curr].iov_len*size - ret->amt_read);
    ret->total_read += (ret->vec[ret->curr].iov_len*size - ret->amt_read);
    realsize -= ret->vec[ret->curr].iov_len*size - ret->amt_read;
    ptr = &(((char*)ptr)[(ret->vec[ret->curr].iov_len*size - ret->amt_read)]);
    // Reset the amount that we have read out of this vector.
    ret->amt_read = 0;
    // go to the next vector
    ret->curr++;
    if (ret->curr >= ret->count)
      return ret->total_read;
  }

  // The amount of data that we need to hand to curl is <= the amount of space
  // that we have left in this iovbuf, so we have to be careful not to exceed it
  if (realsize <= (ret->vec[ret->curr].iov_len*size - ret->amt_read)) {
    chpl_memcpy(ptr, &(((char*)ret->vec[ret->curr].iov_base)[ret->amt_read]), realsize);
    ret->total_read += realsize;
    ret->amt_read += realsize;
    // We have fully read this iovbuf
    if (ret->vec[ret->curr].iov_len*size == ret->amt_read) {
      ret->curr++;
      ret->amt_read = 0;
    }
  }
  return ret->total_read;
}

// Write from curl to a string. Note that userdata is a str_t, since nuch as we have
// to keep track of how much we read in  buf_writer, we have to keep track of how
// long our string is so that we cann lengthen it via realloc as we need to.
static
size_t chpl_curl_write_string(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct str_t *str = (struct str_t *)userp;

  str->mem = (char*)qio_realloc(str->mem, str->size + realsize + 1);
  if(str->mem == NULL) {
    return 0;
  }

  chpl_memcpy(&(str->mem[str->size]), contents, realsize);
  str->size += realsize;
  str->mem[str->size] = 0;

  return realsize;
}

static
qioerr  curl_preadv_internal(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs)
{
  CURLcode err;
  ssize_t got_total;
  qioerr err_out = 0;
  struct curl_iovec_t write_vec;
  curl_handle* local_handle = to_curl_handle(file);

  STARTING_SLOW_SYSCALL;

  got_total = 0;
  write_vec.total_read = 0;
  write_vec.count = count;
  write_vec.vec = (struct iovec*)vector;
  write_vec.amt_read = 0;
  write_vec.offset = 0;
  write_vec.curr = 0;

  // offset = -1 if we want readv
  offset = offset == -1 ? local_handle->current_offset : offset;

  if (local_handle->seekable)  // we can request byteranges
    curl_easy_setopt(local_handle->curl, CURLOPT_RESUME_FROM_LARGE, offset);
  else
    write_vec.offset = offset;

  curl_easy_setopt(local_handle->curl, CURLOPT_WRITEFUNCTION, buf_writer);

  // Read into write_vec
  curl_easy_setopt(local_handle->curl, CURLOPT_WRITEDATA, &write_vec);

  // Read our data into the buf
  err = curl_easy_perform(local_handle->curl);
  DONE_SLOW_SYSCALL;

  got_total = write_vec.total_read;
  local_handle->current_offset += got_total;

  if ((err == CURLE_RANGE_ERROR ||  err == CURLE_OK) && got_total == 0 && sys_iov_total_bytes(vector, count) != 0)
    err_out = qio_int_to_err(EEOF);

  curl_easy_setopt(local_handle->curl, CURLOPT_WRITEFUNCTION, NULL);
  curl_easy_setopt(local_handle->curl, CURLOPT_WRITEDATA, NULL);

  *num_read_out = got_total;
  return err_out;
}


static
qioerr curl_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs)
{
  return curl_preadv_internal(file, vector, count, -1, num_read_out, fs);
}

static
qioerr curl_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs)
{
  return curl_preadv_internal(file, vector, count, offset, num_read_out, fs);
}

static
qioerr curl_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs)
{
  CURLcode ret;
  qioerr err_out = 0;
  struct curl_iovec_t write_vec;

  write_vec.total_read = 0;
  write_vec.count = iovcnt;
  write_vec.vec = (struct iovec*)iov;
  write_vec.amt_read = 0;
  write_vec.offset = 0;
  write_vec.curr = 0;

  STARTING_SLOW_SYSCALL;
  /*tell it to "upload" to the URL*/
  curl_easy_setopt(to_curl_handle(fl)->curl, CURLOPT_UPLOAD, 1L);
  // set it up to write over curl
  curl_easy_setopt(to_curl_handle(fl)->curl, CURLOPT_READFUNCTION, read_data);
  // Tell curl how much data to expect
  curl_easy_setopt(to_curl_handle(fl)->curl, CURLOPT_INFILESIZE_LARGE,  (curl_off_t)sys_iov_total_bytes(iov, iovcnt));
  curl_easy_setopt(to_curl_handle(fl)->curl, CURLOPT_READDATA, &write_vec);
  ret = curl_easy_perform(to_curl_handle(fl)->curl);
  *num_written_out = write_vec.total_read;

  if (ret != CURLE_OK)
    err_out = qio_mkerror_errno();

  curl_easy_setopt(to_curl_handle(fl)->curl, CURLOPT_UPLOAD, 0L);
  curl_easy_setopt(to_curl_handle(fl)->curl, CURLOPT_INFILESIZE_LARGE, 0L);
  curl_easy_setopt(to_curl_handle(fl)->curl, CURLOPT_READFUNCTION, NULL);
  curl_easy_setopt(to_curl_handle(fl)->curl, CURLOPT_READDATA, NULL);

  DONE_SLOW_SYSCALL;

  return err_out;
}

static
int startWith(const char *haystack, const char *needle)
{
  return strncmp(haystack, needle, strlen(needle)) == 0;
}

static
int curl_seekable(void* file, double* length_out)
{
  struct str_t buf;
  int ret = 0;
  // We're on HTTP/HTTPS so we should look for byte ranges to see if we can request
  // them
  if (startWith(to_curl_handle(file)->pathnm, "http://") ||
      startWith(to_curl_handle(file)->pathnm, "https://")) {

    // The size doesn't really matter, we just want a place on the heap. This will get
    // expanded in curl_write_string.
    // Headers tend to be ~800, although they can grow much larger than this. If it is
    // larger than this, we'll take care of it in chpl_curl_write_string.
    buf.mem = (char*)qio_calloc(800, 1);
    buf.size = 0;

    curl_easy_setopt(to_curl_handle(file)->curl, CURLOPT_WRITEFUNCTION, chpl_curl_write_string);
    curl_easy_setopt(to_curl_handle(file)->curl, CURLOPT_HEADERDATA, &buf);
    curl_easy_setopt(to_curl_handle(file)->curl, CURLOPT_NOBODY, 1L);

    curl_easy_perform(to_curl_handle(file)->curl);

    curl_easy_setopt(to_curl_handle(file)->curl, CURLOPT_NOBODY, 0L);
    curl_easy_setopt(to_curl_handle(file)->curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(to_curl_handle(file)->curl, CURLOPT_HEADERDATA, NULL);

    // Does this URL accept range requests?
    if (strstr(buf.mem, "Accept-Ranges") == NULL)
      ret = 0;
    else
      ret = 1;

    qio_free(buf.mem);
  }

  // This works for things other than http
  curl_easy_getinfo(to_curl_handle(file)->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, length_out);

  // We can always "seek" on ftp (with the REST command/RESUME_FROM_LARGE)
  if (startWith(to_curl_handle(file)->pathnm, "ftp://"))
    ret = 1;

  return ret;
}

static
qioerr curl_open(void** fd, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs)
{
  qioerr err_out = 0;
  int rc = 0;
  // Curl expects (NEEDS) this to be a double
  double filelength;
  curl_handle* fl = (curl_handle*)qio_calloc(sizeof(curl_handle), 1);

  STARTING_SLOW_SYSCALL;

  to_curl_handle(fl)->curl =  curl_easy_init();

  // Assert that we opened the file
  if (to_curl_handle(fl)->curl == NULL) {
    QIO_GET_CONSTANT_ERROR(err_out, ECONNREFUSED, "Unable to connect with Curl");
    goto error;
  }

  // set URL
  curl_easy_setopt(to_curl_handle(fl)->curl, CURLOPT_URL, path);

  to_curl_handle(fl)->pathnm = path;
  to_curl_handle(fl)->current_offset = 0;


  // Read the header in order to get the length of the thing we are reading
  // If we are writing, we can't really get this information (even if we try to do a
  // 0 length read).
  if (*flags & O_WRONLY) {
    to_curl_handle(fl)->length = -1;
    to_curl_handle(fl)->seekable = 0;
  } else {
    to_curl_handle(fl)->seekable = curl_seekable(fl, &filelength);
    to_curl_handle(fl)->length = (ssize_t)filelength;
  }

  DONE_SLOW_SYSCALL;

  // Not seekable unless we specify otherwise
  *flags &= ~QIO_FDFLAG_SEEKABLE;

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

  // We can seek
  if (to_curl_handle(fl)->seekable)
    *flags |= QIO_FDFLAG_SEEKABLE;

  *fd = fl;
  return err_out;

error:
  qio_free(fl);
  return err_out;
}

static
qioerr curl_close(void* fl, void* fs)
{
  qioerr err_out = 0;

  STARTING_SLOW_SYSCALL;
  curl_easy_cleanup(to_curl_handle(fl)->curl);
  qio_free(fl);
  DONE_SLOW_SYSCALL;

  return err_out;
}

// Note: libcurl really doesn't support seeking. See:
// http://curl.haxx.se/mail/lib-2009-05/0085.html
// So instead, we keep track of where we need to seek to, in order to replicate the
// correct behaviour. We return ESPIPE in the case that we can't get the length
// since we can't get the length of the file (which we expect in this case in qio).
// Instead, in this case we fall back to using readv instead of preadv. In the case
// where we can get the length of the file we call preadv.
// Note: We could get rid of this requirement if we don't have getlength return
// an error message in the case that we don't have the length (but this can cause
// weird user level behaviour).
static
qioerr curl_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs)
{
  curl_handle* curl_local = to_curl_handle(fl);

  if (curl_local->length == -1)
    QIO_RETURN_CONSTANT_ERROR(ESPIPE, "Unable to seek: URL does not support byte ranges");

  switch (whence) {
    case SEEK_CUR:
      curl_local->current_offset = curl_local->current_offset + offset;
      break;
    case SEEK_END:
      if (curl_local->length != -1)  // we have the length
        curl_local->current_offset= curl_local->length + offset;
      else
        QIO_RETURN_CONSTANT_ERROR(ESPIPE, "Unable to SEEK_END for path with unkown length");
      break;
    case SEEK_SET:
      curl_local->current_offset = offset;
      break;
  }

  *offset_out = curl_local->current_offset + offset;
  return 0;
}

static
qioerr curl_getpath(void* file, const char** string_out, void* fs)
{
  *string_out = to_curl_handle(file)->pathnm;
  return 0;
}

static
qioerr curl_getlength(void* fl, int64_t* len_out, void* fs)
{
  if (to_curl_handle(fl)->length == -1) {
    // This will set initial length to 0 in QIO
    *len_out = 0;
    QIO_RETURN_CONSTANT_ERROR(ENOTSUP, "Unable to get length of curl URL");
  }

  *len_out = to_curl_handle(fl)->length;
  return 0;
}

static
int curl_get_fs_type(void* fl, void* fs)
{
  // In the future, we could see this returning differently based upon whether we
  // were using SSL, TLS etc. For now though, we just return that we are on CURL.
  return FTYPE_CURL;
}


// Blech, but in order to get the parametricity that we want, we need to
// essentially do what the Curl folks did...
qioerr chpl_curl_set_opt(qio_file_t* fl, int opt, ...)
{
  qioerr err;
  va_list argp;
  va_start(argp, opt);
  STARTING_SLOW_SYSCALL;

  // This reasoning is pulled from the libcurl source
  if (opt < CURLOPTTYPE_OBJECTPOINT) {
    long tmp = va_arg(argp, long);
    err = qio_int_to_err(curl_easy_setopt(to_curl_handle(fl->file_info)->curl, (CURLoption)opt, tmp));
  } else if (opt < CURLOPTTYPE_OFF_T) {
    void* tmp = va_arg(argp, void*);
    err = qio_int_to_err(curl_easy_setopt(to_curl_handle(fl->file_info)->curl, (CURLoption)opt, tmp));
  } else {
    curl_off_t tmp = va_arg(argp, curl_off_t);
    err = qio_int_to_err(curl_easy_setopt(to_curl_handle(fl->file_info)->curl, (CURLoption)opt, tmp));
  }

  DONE_SLOW_SYSCALL;
  return err;
}

qioerr chpl_curl_perform(qio_file_t* fl)
{
  qioerr err;
  STARTING_SLOW_SYSCALL;
  err = qio_int_to_err(curl_easy_perform(to_curl_handle(fl->file_info)->curl));
  DONE_SLOW_SYSCALL;
  return err;
}

qioerr chpl_curl_slist_append(chpl_slist* list, const char* str) {
  *list = curl_slist_append(*list, str);
  if (*list == NULL)
    return qio_int_to_err(ENODATA);
  return 0;
}

void chpl_curl_slist_free(chpl_slist list) {
  curl_slist_free_all(list);
}

qio_file_functions_t curl_function_struct = {
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
    &curl_get_fs_type, // get_fs_type
};

const qio_file_functions_ptr_t curl_function_struct_ptr = &curl_function_struct;

const int curlopt_file                       = CURLOPT_FILE;
const int curlopt_url                        = CURLOPT_URL;
const int curlopt_port                       = CURLOPT_PORT;
const int curlopt_proxy                      = CURLOPT_PROXY;
const int curlopt_userpwd                    = CURLOPT_USERPWD;
const int curlopt_proxyuserpwd               = CURLOPT_PROXYUSERPWD;
const int curlopt_range                      = CURLOPT_RANGE;
const int curlopt_infile                     = CURLOPT_INFILE;
const int curlopt_errorbuffer                = CURLOPT_ERRORBUFFER;
const int curlopt_writefunction              = CURLOPT_WRITEFUNCTION;
const int curlopt_readfunction               = CURLOPT_READFUNCTION;
const int curlopt_timeout                    = CURLOPT_TIMEOUT;
const int curlopt_infilesize                 = CURLOPT_INFILESIZE;
const int curlopt_postfields                 = CURLOPT_POSTFIELDS;
const int curlopt_referer                    = CURLOPT_REFERER;
const int curlopt_ftpport                    = CURLOPT_FTPPORT;
const int curlopt_useragent                  = CURLOPT_USERAGENT;
const int curlopt_low_speed_limit            = CURLOPT_LOW_SPEED_LIMIT;
const int curlopt_low_speed_time             = CURLOPT_LOW_SPEED_TIME;
const int curlopt_resume_from                = CURLOPT_RESUME_FROM;
const int curlopt_cookie                     = CURLOPT_COOKIE;
const int curlopt_httpheader                 = CURLOPT_HTTPHEADER;
const int curlopt_httppost                   = CURLOPT_HTTPPOST;
const int curlopt_sslcert                    = CURLOPT_SSLCERT;
const int curlopt_keypasswd                  = CURLOPT_KEYPASSWD;
const int curlopt_crlf                       = CURLOPT_CRLF;
const int curlopt_quote                      = CURLOPT_QUOTE;
const int curlopt_writeheader                = CURLOPT_WRITEHEADER;
const int curlopt_cookiefile                 = CURLOPT_COOKIEFILE;
const int curlopt_sslversion                 = CURLOPT_SSLVERSION;
const int curlopt_timecondition              = CURLOPT_TIMECONDITION;
const int curlopt_timevalue                  = CURLOPT_TIMEVALUE;
const int curlopt_customrequest              = CURLOPT_CUSTOMREQUEST;
const int curlopt_stderr                     = CURLOPT_STDERR;
const int curlopt_postquote                  = CURLOPT_POSTQUOTE;
const int curlopt_writeinfo                  = CURLOPT_WRITEINFO;
const int curlopt_verbose                    = CURLOPT_VERBOSE;
const int curlopt_header                     = CURLOPT_HEADER;
const int curlopt_noprogress                 = CURLOPT_NOPROGRESS;
const int curlopt_nobody                     = CURLOPT_NOBODY;
const int curlopt_failonerror                = CURLOPT_FAILONERROR;
const int curlopt_upload                     = CURLOPT_UPLOAD;
const int curlopt_post                       = CURLOPT_POST;
const int curlopt_dirlistonly                = CURLOPT_DIRLISTONLY;
const int curlopt_append                     = CURLOPT_APPEND;
const int curlopt_netrc                      = CURLOPT_NETRC;
const int curlopt_followlocation             = CURLOPT_FOLLOWLOCATION;
const int curlopt_transfertext               = CURLOPT_TRANSFERTEXT;
const int curlopt_put                        = CURLOPT_PUT;
const int curlopt_progressfunction           = CURLOPT_PROGRESSFUNCTION;
const int curlopt_progressdata               = CURLOPT_PROGRESSDATA;
const int curlopt_autoreferer                = CURLOPT_AUTOREFERER;
const int curlopt_proxyport                  = CURLOPT_PROXYPORT;
const int curlopt_postfieldsize              = CURLOPT_POSTFIELDSIZE;
const int curlopt_httpproxytunnel            = CURLOPT_HTTPPROXYTUNNEL;
const int curlopt_interface                  = CURLOPT_INTERFACE;
const int curlopt_krblevel                   = CURLOPT_KRBLEVEL;
const int curlopt_ssl_verifypeer             = CURLOPT_SSL_VERIFYPEER;
const int curlopt_cainfo                     = CURLOPT_CAINFO;
const int curlopt_maxredirs                  = CURLOPT_MAXREDIRS;
const int curlopt_filetime                   = CURLOPT_FILETIME;
const int curlopt_telnetoptions              = CURLOPT_TELNETOPTIONS;
const int curlopt_maxconnects                = CURLOPT_MAXCONNECTS;
const int curlopt_closepolicy                = CURLOPT_CLOSEPOLICY;
const int curlopt_fresh_connect              = CURLOPT_FRESH_CONNECT;
const int curlopt_forbid_reuse               = CURLOPT_FORBID_REUSE;
const int curlopt_random_file                = CURLOPT_RANDOM_FILE;
const int curlopt_egdsocket                  = CURLOPT_EGDSOCKET;
const int curlopt_connecttimeout             = CURLOPT_CONNECTTIMEOUT;
const int curlopt_headerfunction             = CURLOPT_HEADERFUNCTION;
const int curlopt_httpget                    = CURLOPT_HTTPGET;
const int curlopt_ssl_verifyhost             = CURLOPT_SSL_VERIFYHOST;
const int curlopt_cookiejar                  = CURLOPT_COOKIEJAR;
const int curlopt_ssl_cipher_list            = CURLOPT_SSL_CIPHER_LIST;
const int curlopt_http_version               = CURLOPT_HTTP_VERSION;
const int curlopt_ftp_use_epsv               = CURLOPT_FTP_USE_EPSV;
const int curlopt_sslcerttype                = CURLOPT_SSLCERTTYPE;
const int curlopt_sslkey                     = CURLOPT_SSLKEY;
const int curlopt_sslkeytype                 = CURLOPT_SSLKEYTYPE;
const int curlopt_sslengine                  = CURLOPT_SSLENGINE;
const int curlopt_sslengine_default          = CURLOPT_SSLENGINE_DEFAULT;
const int curlopt_dns_use_global_cache       = CURLOPT_DNS_USE_GLOBAL_CACHE;
const int curlopt_dns_cache_timeout          = CURLOPT_DNS_CACHE_TIMEOUT;
const int curlopt_prequote                   = CURLOPT_PREQUOTE;
const int curlopt_debugfunction              = CURLOPT_DEBUGFUNCTION;
const int curlopt_debugdata                  = CURLOPT_DEBUGDATA;
const int curlopt_cookiesession              = CURLOPT_COOKIESESSION;
const int curlopt_capath                     = CURLOPT_CAPATH;
const int curlopt_buffersize                 = CURLOPT_BUFFERSIZE;
const int curlopt_nosignal                   = CURLOPT_NOSIGNAL;
const int curlopt_share                      = CURLOPT_SHARE;
const int curlopt_proxytype                  = CURLOPT_PROXYTYPE;
const int curlopt_encoding                   = CURLOPT_ENCODING;
const int curlopt_private                    = CURLOPT_PRIVATE;
const int curlopt_http200aliases             = CURLOPT_HTTP200ALIASES;
const int curlopt_unrestricted_auth          = CURLOPT_UNRESTRICTED_AUTH;
const int curlopt_ftp_use_eprt               = CURLOPT_FTP_USE_EPRT;
const int curlopt_httpauth                   = CURLOPT_HTTPAUTH;
const int curlopt_ssl_ctx_function           = CURLOPT_SSL_CTX_FUNCTION;
const int curlopt_ssl_ctx_data               = CURLOPT_SSL_CTX_DATA;
const int curlopt_ftp_create_missing_dirs    = CURLOPT_FTP_CREATE_MISSING_DIRS;
const int curlopt_proxyauth                  = CURLOPT_PROXYAUTH;
const int curlopt_ftp_response_timeout       = CURLOPT_FTP_RESPONSE_TIMEOUT;
const int curlopt_ipresolve                  = CURLOPT_IPRESOLVE;
const int curlopt_maxfilesize                = CURLOPT_MAXFILESIZE;
const int curlopt_infilesize_large           = CURLOPT_INFILESIZE_LARGE;
const int curlopt_resume_from_large          = CURLOPT_RESUME_FROM_LARGE;
const int curlopt_maxfilesize_large          = CURLOPT_MAXFILESIZE_LARGE;
const int curlopt_netrc_file                 = CURLOPT_NETRC_FILE;
const int curlopt_use_ssl                    = CURLOPT_USE_SSL;
const int curlopt_postfieldsize_large        = CURLOPT_POSTFIELDSIZE_LARGE;
const int curlopt_tcp_nodelay                = CURLOPT_TCP_NODELAY;
const int curlopt_ftpsslauth                 = CURLOPT_FTPSSLAUTH;
const int curlopt_ioctlfunction              = CURLOPT_IOCTLFUNCTION;
const int curlopt_ioctldata                  = CURLOPT_IOCTLDATA;
const int curlopt_ftp_account                = CURLOPT_FTP_ACCOUNT;
const int curlopt_cookielist                 = CURLOPT_COOKIELIST;
const int curlopt_ignore_content_length      = CURLOPT_IGNORE_CONTENT_LENGTH;
const int curlopt_ftp_skip_pasv_ip           = CURLOPT_FTP_SKIP_PASV_IP;
const int curlopt_ftp_filemethod             = CURLOPT_FTP_FILEMETHOD;
const int curlopt_localport                  = CURLOPT_LOCALPORT;
const int curlopt_localportrange             = CURLOPT_LOCALPORTRANGE;
const int curlopt_connect_only               = CURLOPT_CONNECT_ONLY;
const int curlopt_conv_from_network_function = CURLOPT_CONV_FROM_NETWORK_FUNCTION;
const int curlopt_conv_to_network_function   = CURLOPT_CONV_TO_NETWORK_FUNCTION;
const int curlopt_conv_from_utf8_function    = CURLOPT_CONV_FROM_UTF8_FUNCTION;
const int curlopt_max_send_speed_large       = CURLOPT_MAX_SEND_SPEED_LARGE;
const int curlopt_max_recv_speed_large       = CURLOPT_MAX_RECV_SPEED_LARGE;
const int curlopt_ftp_alternative_to_user    = CURLOPT_FTP_ALTERNATIVE_TO_USER;
const int curlopt_sockoptfunction            = CURLOPT_SOCKOPTFUNCTION;
const int curlopt_sockoptdata                = CURLOPT_SOCKOPTDATA;
const int curlopt_ssl_sessionid_cache        = CURLOPT_SSL_SESSIONID_CACHE;
const int curlopt_ssh_auth_types             = CURLOPT_SSH_AUTH_TYPES;
const int curlopt_ssh_public_keyfile         = CURLOPT_SSH_PUBLIC_KEYFILE;
const int curlopt_ssh_private_keyfile        = CURLOPT_SSH_PRIVATE_KEYFILE;
const int curlopt_ftp_ssl_ccc                = CURLOPT_FTP_SSL_CCC;
const int curlopt_timeout_ms                 = CURLOPT_TIMEOUT_MS;
const int curlopt_connecttimeout_ms          = CURLOPT_CONNECTTIMEOUT_MS;
const int curlopt_http_transfer_decoding     = CURLOPT_HTTP_TRANSFER_DECODING;
const int curlopt_http_content_decoding      = CURLOPT_HTTP_CONTENT_DECODING;
const int curlopt_new_file_perms             = CURLOPT_NEW_FILE_PERMS;
const int curlopt_new_directory_perms        = CURLOPT_NEW_DIRECTORY_PERMS;
const int curlopt_postredir                  = CURLOPT_POSTREDIR;
const int curlopt_ssh_host_public_key_md5    = CURLOPT_SSH_HOST_PUBLIC_KEY_MD5;
const int curlopt_opensocketfunction         = CURLOPT_OPENSOCKETFUNCTION;
const int curlopt_opensocketdata             = CURLOPT_OPENSOCKETDATA;
const int curlopt_copypostfields             = CURLOPT_COPYPOSTFIELDS;
const int curlopt_proxy_transfer_mode        = CURLOPT_PROXY_TRANSFER_MODE;
const int curlopt_seekfunction               = CURLOPT_SEEKFUNCTION;
const int curlopt_seekdata                   = CURLOPT_SEEKDATA;
const int curlopt_crlfile                    = CURLOPT_CRLFILE;
const int curlopt_issuercert                 = CURLOPT_ISSUERCERT;
const int curlopt_address_scope              = CURLOPT_ADDRESS_SCOPE;
const int curlopt_certinfo                   = CURLOPT_CERTINFO;
const int curlopt_username                   = CURLOPT_USERNAME;
const int curlopt_password                   = CURLOPT_PASSWORD;
const int curlopt_proxyusername              = CURLOPT_PROXYUSERNAME;
const int curlopt_proxypassword              = CURLOPT_PROXYPASSWORD;
const int curlopt_noproxy                    = CURLOPT_NOPROXY;
const int curlopt_tftp_blksize               = CURLOPT_TFTP_BLKSIZE;
const int curlopt_socks5_gssapi_service      = CURLOPT_SOCKS5_GSSAPI_SERVICE;
const int curlopt_socks5_gssapi_nec          = CURLOPT_SOCKS5_GSSAPI_NEC;
const int curlopt_protocols                  = CURLOPT_PROTOCOLS;
const int curlopt_redir_protocols            = CURLOPT_REDIR_PROTOCOLS;
const int curlopt_lastentry                  = CURLOPT_LASTENTRY;
const int curlopt_mail_from                  = CURLOPT_MAIL_FROM;
const int curlopt_mail_rcpt                  = CURLOPT_MAIL_RCPT;
const int curlopt_mail_auth                  = CURLOPT_MAIL_AUTH;

