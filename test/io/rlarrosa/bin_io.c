
#include <stdio.h>
#include <errno.h>

// size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
// size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);




// in chapel:
// _extern def binfwrite (inout ptr:int, size:int(64) , nelm:int(64), file:_file ): int(64);

void binfwriteint (const void *ptr, size_t size, size_t nmemb, FILE *stream, long *res, int *err);
void binfwriteint (const void *ptr, size_t size, size_t nmemb, FILE *stream, long *res, int *err) 
{
  *res=fwrite(ptr,size, nmemb, stream);
  *err=errno; 
}

void binfwrite (const void *ptr, size_t size, size_t nmemb, FILE *stream, long *res, int *err);
void binfwrite (const void *ptr, size_t size, size_t nmemb, FILE *stream, long *res, int *err) 
{
  *res=fwrite(ptr,size, nmemb, stream);
  *err=errno; 
}

void binfread (void *ptr, size_t size, size_t nmemb, FILE *stream, long *res, int *err);
void binfread (void *ptr, size_t size, size_t nmemb, FILE *stream, long *res, int *err)
{

  *res=fread(ptr,size, nmemb, stream);
  *err=errno; 
}






// #include <unistd.h> este para int fd

// ssize_t write(int fd, const void *buf, size_t count);
/*
long binwrite (int fd,int *buf , long count) {


return 0;
}

long binread (int fd,int *buf , long count) {

return 0;
}
*/
