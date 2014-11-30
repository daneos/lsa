/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <dirent.h>
#include <stdio.h>

#include "include/perm_filters.h"
#include "include/print_entry.h"
#include "include/options.h"

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	struct stat finfo;
	char fname[PATH_MAX+1];
	int n, i, opts, single_file_mode = 0;
	struct dirent **list;
	config c;

	opts = options(argc, argv, &c);
	if(opts != OPT_OK) return -1;

	// if scandir raises error, switch to single file mode as argument is probably not a directory
	if((n = scandir(c.dir, &list, NULL, alphasort)) < 0)
	{
		perror("main()/scandir(): falling back to single file mode");
		single_file_mode = 1;
		n = 1;
	}

	for(i=0; i < n; i++)
	{
		// prepend file name with directory (needed for dirs other than .)
		if(!single_file_mode) snprintf(fname, sizeof fname, "%s/%s", c.dir, list[i]->d_name);
		
		// get file info
		if(lstat(single_file_mode ? c.dir : fname, &finfo) == -1)
		{
			perror("main()/stat()");
			continue;
		}

		if(!filter_perm(&finfo, &c)) continue;
		print_long(&finfo, fname, single_file_mode ? c.dir : list[i]->d_name);
	}
	return 0;
}