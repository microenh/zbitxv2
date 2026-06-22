#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "get_exe_path.h"

static char *exe_path;

const char *get_exe_path() {
	if (exe_path) {
		#ifdef LOG
			log_debug("Using saved value");
		#endif
		return exe_path;
	} else {
		#ifdef LOG
			log_debug("Calculating");
		#endif
	  char *work_path, *last_slash;
		work_path = (char *) malloc(PATH_MAX);
		if (!work_path) {
			#ifdef LOG
				log_fatal("malloc work_path failed");
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
				*(++last_slash) = '\0';
			}
			#ifdef LOG
				log_debug("work_path: %s", work_path);
			#endif
			int l = last_slash - work_path + 1;
			#ifdef LOG
				log_debug("l: %d", l);
			#endif
			exe_path = (char *) malloc(l);
			if (!exe_path) {
				#ifdef LOG
					log_fatal("malloc exe_path failed");
				#endif
				exit(1);
			}
			memcpy(exe_path, work_path, l);
			free(work_path);
			return exe_path;
    }
 	}
}

#if 0
int main() {
	log_set_level(LOG_DEBUG);
	log_info("path %s", get_exe_path());
	log_info("path %s", get_exe_path());
  return 0;
}
#endif


