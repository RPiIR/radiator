#include <stdlib.h>
#include "info_rad.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

int main_alarm(int argc, char *argv[]) {
	if (!argc) {
		return 3;
	}
	if (strcmp(argv[0],"on") == 0) {
		i_rad_light(1);
	} else {
		i_rad_light(0);
	}
	return 0;
}

int main (int argc, char *argv[])
{
	printf("%s\n", PACKAGE_STRING);
	if (argc > 1 && strcmp(argv[1],"alarm") == 0) {
		return main_alarm(argc-2, argv+2);
	}
	return 0;
}
