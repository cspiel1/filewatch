#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOG_WRN(args, ...) fprintf(stderr,"::WRN line %d, in file %s " "" \
		args "" "\n", __LINE__, __FILE__, ## __VA_ARGS__)

#define LOG_INF(args, ...) fprintf(stderr,"::MSG line %d, in file %s " "" \
		args "" "\n", __LINE__, __FILE__, ## __VA_ARGS__)

#define INOTIFY_TIMEOUT 2000

void print_usage(const char* pname)
{
	printf("Usage: %s filename\n", pname);
}

int main(int argc, char *argv[])
{
  int fd;
  int err;

  if (argc < 2) {
	  print_usage(argv[0]);
	  exit(EINVAL);
  }

  const char *filename = argv[1];

  fd = open(filename, O_RDWR);
  if (fd == -1) {
    LOG_WRN("Could not open %s.", filename);
    exit(errno);
  }
  LOG_INF("Opened %s.", filename);
  close(fd);
  err = errno;
  LOG_INF("Closed %d.", err);
  return err;
}
