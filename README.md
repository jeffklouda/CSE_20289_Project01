Project 01 - README
===================

Members
-------

- Jeff Klouda (jklouda1@nd.edu)
- Matthew D'Alonzo (mdalonz1@nd.edu)

Responses
---------
##### 1.
I used a while loop which iterated through the `argv` array and checked each `char *` in the array. If the string in the array was one I was looking for. I commited the required task for it. For the case of the `exec` arguments, when I found `-exec`, I pushed the subsequent arguments, one-by-one, to the `settings->exec_argv` array. This array is processed in `execute.c`.
##### 2.
First, I opened the `root` file that I wanted to check. Then, if that file wasn't a directory, I returned from the search function. After this, if I couldn't open the directory for whatever reason, I returned from the search function. Then, I iterated through the files in the directory, and as long as the files weren't `.` or `..`, I would filter on the files, execute on them if the filter passed, and then recurse on that files subfiles.
##### 3.
To determine whether or not to print a filesystem object path, I checked both`settings->print` and `settings->exec_argc`. If was not the case that `settings->print` was disabled and `settings->exec_argc` was greater than 0, than I printed. It was only in this case that I didn't print the filesystem object names.
##### syscalls.py
For the system calls, I noticed that find calls a lot of different and weird system calls that I didn't use in search. This includes things like newfstatat, fchdir, set_robust_list, and a lot of things I haven't heard of before. As well, it seems as if my program makes use of many more system calls than find does. 
It is interesting that there are system calls made in my program search that I didn't explicitly make. This would make sense, though, since there are system calls made by normal functions underneath the surface.

Contributions
-------------
Matthew D'Alonzo-
	search.c
	execute.c
	README.md

Jeff Klouda-
	Makefile
	utilities.c
	filter.c
	main.c
	memory leaks
