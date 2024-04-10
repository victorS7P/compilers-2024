#include <stdio.h>

#define BUFSZ (256 * 2) + 4

int main (int argc, char **argv) {
	unsigned char buf[BUFSZ] = {0};
	size_t bytes = 0, i, readsz = sizeof buf;
	FILE *fp = fopen ("./hlt.mem", "rb");

	if (!fp) {
		fprintf (stderr, "error: file open failed '%s'.\n", argv[1]);
		return 1;
	}

	bytes = fread (buf, sizeof *buf, readsz, fp);

	int memory[256];
	for (i = 4; i < bytes; i++) {
		if (i % 2 == 1) continue;

		int n = (int)buf[i];
		memory[(i / 2) - 2] = n;
	}

	for (i = 0; i < 256; i++)
		printf("%d ", memory[i]);
	

	fclose (fp);
	return 0;
}