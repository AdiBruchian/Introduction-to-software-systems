#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void operate(FILE* f) {
	int grade;
	int retval;
	double avg = 0;
	int line_n = 1;
	int student_number = 0;

	while (1) {
		retval = fscanf(f, "%d", &grade);
		if (retval == EOF) {
			break;
		}
		else if (retval != 1) {
			fprintf(stderr, "Error: input in line %d is not a number\n", line_n);
			exit(1);
		}
		else if (grade < 0 || grade > 100) {
			fprintf(stderr, "Error: input in line %d is not in range\n", line_n);
			line_n++;
			continue;
		}
		avg += grade;
		line_n++;
		student_number++;
	}
	printf("%.2lf\n", avg / student_number);
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
		fprintf(stderr, "File not found: \"%s\"\n", argv[1]);
		return 1;
	}
	operate(fp);
	fclose(fp);

	return 0;
}
