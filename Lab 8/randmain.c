#include "randcpuid.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>

/* Main program, which outputs N bytes of random data.  */

static bool
writebytes (unsigned long long x, int nbytes)
{
  int ndigits = nbytes * 2;
  do
    {
      if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
        return false;
      x >>= 4;
      ndigits--;
    }
  while (0 < ndigits);

  return 0 <= putchar ('\n');
}

int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
        perror (argv[1]);
      else
        valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the                                                                    
     appropriate library.  */
  void *handle_dl;
  
  unsigned long long (*rand64) (void);

  if (rdrand_supported ())
    {
	handle_dl = dlopen( "randlibhw.so" , RTLD_LAZY );
	char* err = dlerror();
	if (handle_dl == NULL){
		fprintf (stderr, "error with dlopen on randlibhw: %s\n", err);
		return 1;
	}
	rand64 = dlsym( handle_dl , "rand64" );
	err = dlerror();
	if (err != NULL){
		fprintf (stderr, "error with dlsym on randlibhw: %s\n", err);
		return 1;
	}
    }
  else
    {
      	handle_dl = dlopen( "randlibsw.so" , RTLD_LAZY );
	char* err = dlerror();
	if (handle_dl == NULL){
		fprintf (stderr, "error with dlopen on randlibsw: %s\n", err);
		return 1;
	}
	rand64 = dlsym( handle_dl , "rand64" );
	err = dlerror();
	if (err != NULL){
		fprintf (stderr, "error with dlsym on randlibsw: %s\n", err);
		return 1;
	}
    }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
        {
          output_errno = errno;
          break;
        }
      nbytes -= outbytes;
    }
  while (0 < nbytes);

if ( dlclose( handle_dl ))
    {
	char* err = dlerror();
        fprintf (stderr, "error with dlclose on handle: %s\n", err);
        return 1;
    }


  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      return 1;
    }

	dlclose( handle_dl );
  return 0;
}
