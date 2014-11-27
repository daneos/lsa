/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL 2.0 license
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <dirent.h>
#include <stdio.h>

#include "fmode.h"
#include "perm_filters.h"
#include "print_entry.h"

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	struct stat finfo;
	char *dirname;
	char fname[PATH_MAX+1];
	int n,i;
	struct dirent **list;

	// if no argument given, default to .
	if(argc > 1)
		dirname = argv[1];
	else
		dirname = ".";
	
	if((n = scandir(dirname, &list, NULL, alphasort)) < 0)
	{
		perror("main()/scandir()");
		return -1;
	}
	for(i=0; i < n; i++)
	{
		// prepend file name with directory (needed for dirs other than .)
		snprintf(fname, sizeof fname, "%s/%s", dirname, list[i]->d_name);
		
		// get file info
		if(lstat(fname, &finfo) == -1)
		{
			perror("main()/stat()");
			continue;
		}

		if(!filter_perm(&finfo, S_IWOTH))continue;
		print_long(&finfo, fname, list[i]->d_name);
	}
	return 0;
}