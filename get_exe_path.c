#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "get_exe_path.h"

static char *exe_path;
static int exe_len;	// excludes trailing '\0';

#ifdef LOG
	static const char out_of_memory[] = "out of memory";
#endif

static const char *get_exe_path() {
	if (exe_path) {
		#ifdef LOG
			log_trace("Using saved value");
		#endif
		return exe_path;
	} else {
		#ifdef LOG
			log_trace("Calculating");
		#endif
	  char *work_path, *last_slash;
		work_path = (char *) malloc(PATH_MAX);
		if (!work_path) {
			#ifdef LOG
				log_fatal(out_of_memory);
			#endif
			exit(1);
		}
    ssize_t len = readlink("/proc/self/exe", work_path, PATH_MAX - 1);
		if (len == -1) {
			#ifdef LOG
				log_fatal("readlink failed");
			#endif
			exit(1);
		} else {
			work_path[len] = '\0';
			
			char *last_slash = strrchr(work_path, '/');
			if (last_slash) {
				*(last_slash + 1) = '\0';
			}
			#ifdef LOG
				log_debug("work_path: %s", work_path);
			#endif
			exe_len = last_slash - work_path + 1;
			exe_path = (char *) malloc(exe_len + 1);
			if (!exe_path) {
				#ifdef LOG
					log_fatal("out_of_memory");
				#endif
				exit(1);
			}
			memcpy(exe_path, work_path, exe_len + 1);
			#ifdef LOG
				log_debug("exe_path: (%d) %s", exe_len, exe_path);
			#endif
			free(work_path);
			return exe_path;
    }
 	}
}

/*
 * returns a pointer to a malloc'ed string with the full path the the given file
 * in a sub_directory under the directory containing the executable file.
 * this must be freed by the call
 * aborts with a call to exit() if the malloc fails.
 *
 * assume executable is /home/pi/Developer/zbitx/sbitx
 * malloc_file_paty("data/", "sbitx", ".db")
 *   returns /home/pi/Developer/zbitx/data/sbitx.db
 *
 * any of the paramaters may be an empty string
 */
const char *malloc_file_path(const char *sub_dir, const char *filename, const char *extension)
{
	#ifdef LOG
		log_debug("malloc_file_path: [%s] [%s] [%s]", sub_dir, filename, extension);
	#endif

	if (!exe_len) {
		get_exe_path();	// set exe_len on first call, if needed
	}
	int path_len = exe_len + strlen(sub_dir) + strlen(filename) + strlen(extension);
	log_debug("path_len %d", path_len);
  char *result = malloc(path_len + 1);
	if (!result) {
		#ifdef LOG
			log_fatal(out_of_memory);
		#endif
		exit(1);
	}
	snprintf(result, path_len + 1, "%s%s%s%s",
		get_exe_path(), sub_dir, filename, extension);
	return result;		
}

#if TEST_MAIN
int main() {
	log_set_level(LOG_DEBUG);
	// log_info("path %s", get_exe_path());
	// log_info("path %s", get_exe_path());
  const char *test = malloc_file_path("data/file.txt", "", "");
	log_info("file %s", test);
  return 0;
}
#endif


