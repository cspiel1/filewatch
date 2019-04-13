#include <stdio.h>
#include <sys/inotify.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>

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
  int wd;
  int result=0;
  int r = 0;

  if (argc < 2) {
	  print_usage(argv[0]);
	  exit(EINVAL);
  }

  const char *filename = argv[1];

  fd = inotify_init();
  if ( fd < 0 ) {
    LOG_WRN("inotify_init failed");
    return 2;
  }

  wd = inotify_add_watch(fd, filename, IN_CLOSE_WRITE);
  struct pollfd pfd = {fd, POLLIN, 0 };
  while (!r) {
	  /* Note: A timeout of 0 is also possible. */
	  r = poll(&pfd, 1, INOTIFY_TIMEOUT);

	  if (r<0) {
		  LOG_WRN("inotify, poll failed");
		  result = 3;
	  } else if (r==0) {
		  LOG_INF("inotify, poll timeout");
		  result = 4;
	  } else {
		  LOG_INF("inotify, poll succeeded %d", r);
		  result = 1;
	  }
	  if (!r) {
		  LOG_INF("sleeping...");
		  sleep(5);
		  LOG_INF("next poll");
	  } else {
		  struct inotify_event ie;
		  /* Be careful. The read will block if the poll had a timeout. */
		  ssize_t s = read(fd, &ie, sizeof(ie));
		  LOG_INF("now a read returns %lu", s);
		  if (s == sizeof(ie)) {
			  LOG_INF("inotify mask = %u", ie.mask);
		  } else {
			  LOG_WRN("Read only too less bytes for an inotify_event.");
		  }
	  }
	  fflush(stderr);
  }

  ( void ) inotify_rm_watch( fd, wd );
/*  ( void ) close( fd );*/
  return result;
}
