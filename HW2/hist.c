#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *f;
static int nbins = 10;

void operate(int *bins);

void parse_arg(int argc, char **argv) {
    f = stdin;
    for (int i=1; i < argc; i++) {
        if (!strcmp(argv[i], "-")) {
            f = stdin;
        } else if (!strcmp(argv[i], "-nbins")) {
            nbins = i<argc-1 ? atoi(argv[i+1]) : 10;
            i++;
        } else {
            f = fopen(argv[i], "r");
        }
    }

}

int main(int argc, char **argv) {
    parse_arg(argc, argv);

    if (!f) {
        fprintf(stderr, "File not found: \"%s\"\n", argv[1]);
        return 1;
    }

    int *bins = (int*)malloc(sizeof(int)*nbins);
    for (int i=0; i<nbins; i++) {
        bins[i] = 0;
    }
    operate(bins);
    free(bins);
    fclose(f);
    return 0;
}


void operate(int *bins) {
    int grade;
    int retval;
    double pace;
    int line = 1;

    while(1) {
        retval = fscanf(f, "%d", &grade);
        if (retval == EOF) {
            break;
        } 
		else if (retval != 1) {
			fprintf(stderr, "Error: input in line %d is not a number\n", line);
			exit(1);
		}
		else if (grade < 0 || grade > 100) {
			fprintf(stderr, "Error: input in line %d is not in range\n", line);
			line++;
			continue;
		}

        int n = grade / (100/nbins);
        if (n < nbins) {
        	bins[n]++;
        	line++;
        } else if (n == nbins) {
        	bins[n-1]++;
        	line++;
        }
    }

    pace = 100.0 /nbins;
    for (int i=0; i<nbins; i++) {
        printf("%.0lf-%.0lf\t%d\n",i*pace ,(i+1)*pace , bins[i]);
    }
}
