#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void find_min(FILE* f) {
	int grade;
	int retval;
	int min = 100;
	int line_n = 1;

	while (1) {
		retval = fscanf(f, "%d", &grade);
		if (retval == EOF) {
			break;
		}
		else if (retval == 0) {
			fprintf(stderr, "Error: input in line %d is not a number\n", line_n);
			exit(1);
        }
		else if (grade < 0 || grade > 100) {
			fprintf(stderr, "Error: input in line %d is not in range\n", line_n);
			line_n++;
			continue;
		}

		if (grade < min) {
			min = grade;
		}
		line_n++;
	}
	printf("%d\n", min);
}


int main(int argc, char** argv) {
	FILE* fp;

	if (argc == 1 || !strcmp("-", argv[1])) {
		fp = stdin;
	}
	else {
		fp = fopen(argv[1], "r");
	}
	if (!fp) {
		fprintf(stderr, "File not found: %p\n", argv[1]);
		return 1;
	}
	find_min(fp);
	fclose(fp);
	return 0;
}
