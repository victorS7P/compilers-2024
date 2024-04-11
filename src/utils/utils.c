#ifdef UTILS
#define UTILS

void debugMessage(char* message) {
	if (DEBUG)
		printf("%s\n", message);
}

#endif
