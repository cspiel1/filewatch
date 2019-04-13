# filewatch
inotify example

Watches a file for the IN_CLOSE_WRITE inotify event. So when a file is closed, which was earlier opened for writing the inotify succeeds. It does not make a difference if the data changed or not.
