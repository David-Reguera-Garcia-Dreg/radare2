/* radare - LGPL - Copyright 2009 pancake<nopcode.org> */

#include "r_core.h"
#include "r_io.h"
#include <stdio.h>
#include <getopt.h>

#define VERSION "0.1"

static struct r_core_t r;

static int main_help(int line)
{
	printf("Usage: radare2 [-dwnV] [-s addr] [-b bsz] [-e k=v] [file] [...]\n");
	if (!line) printf(
		" -d        use 'file' as a program to debug\n"
		" -w        open file in write mode\n"
		" -n        do not run ~/.radarerc\n"
		" -s [addr] initial seek\n"
		" -b [size] initial block size\n"
		" -V        show radare2 version\n"
		" -e k=v    evaluate config var\n");
	return 0;
}

static int main_version()
{
	printf("radare2 "VERSION"\n");
	return 0;
}

int main(int argc, char **argv)
{
	struct r_core_file_t *fh;
 	int c, perms = R_IO_READ;
	int run_rc = 1;
	int debug = 0;
	int bsize = 0;
	int seek = 0; // XXX use 64

	if (argc<2)
		return main_help(1);

	r_core_init(&r);

	while((c = getopt(argc, argv, "whendVs:b:"))!=-1) {
		switch(c) {
		case 'd':
			debug = 1;
			break;
		case 'e':
			r_config_eval(&r.config, optarg);
			break;
		case 'h':
			return main_help(0);
		case 'n':
			run_rc = 0;
			break;
		case 'V':
			return main_version();
			break;
		case 'w':
			perms = R_IO_RDWR;
			break;
		case 'b':
			bsize = atoi(optarg); // XXX use r_num
			break;
		case 's':
			seek = atoi(optarg); // XXX use r_num
			break;
		default:
			return 1;
		}
	}

	if (debug) {
		char file[1024];

		strcpy(file, "dbg://");
		if (optind<argc) {
			char *ptr = r_file_path(argv[optind]);
			if (ptr) {
				strcat(file, ptr);
				free(ptr);
				optind++;
			}
		}
		while (optind<argc) {
			strcat(file, argv[optind]);
			strcat(file, " ");
			optind++;
			if (optind!=argc)
				strcat(file, " ");
		}
		fh = r_core_file_open(&r, file, perms);
		if (fh == NULL) {
			fprintf(stderr,
			"Cannot open file '%s'\n", file);
			return 1;
		}
		
	} else
	while (optind<argc) {
		const char *file = argv[optind++];
		fh = r_core_file_open(&r, file, perms);
		if (fh == NULL) {
			fprintf(stderr, "Cannot open file '%s'\n", file);
			return 1;
		}
	}

	if (r.file == NULL) {
		fprintf(stderr, "Cannot open file\n");
		return 1;
	}

	if (run_rc) {
		char *homerc = r_str_home(".radarerc");
		if (homerc) {
			r_core_cmd_file(&r, homerc);
			free(homerc);
		}
	}
	if (debug) {
		r_core_cmd(&r, "dh ptrace", 0);
		r_core_cmdf(&r, "dp %d", r.file->fd);
		r_core_cmd(&r, ".dr*", 0);
		r_core_cmd(&r, "s eip", 0);
		r_core_cmd(&r, "e cmd.prompt=.dr",0);
		r_core_cmd(&r, "\"e cmd.vprompt=.dr\"",0);
		r_core_cmd(&r, "\"e cmd.visual=.dr&&s eip\"",0);
	}

	if (seek)
		r_core_seek(&r, seek);
	if (bsize)
		r_core_block_size(&r, bsize);

	while(r_core_prompt(&r) != -1);

	return r_core_file_close(&r, fh);
}
