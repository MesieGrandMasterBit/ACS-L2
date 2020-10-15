#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

int m = 10000, n = 10000;

double meanSquareDeviation(double array[], int n, char combination) {
	double averageSum = 0;
	double difference = 0;
	double sumDifference = 0;
	double dispersion = 0;
	double deviation = 0;

	for (int i = 0; i < n; i++)
		averageSum += array[i];
	averageSum /= n;

	for (int i = 0; i < n; i++) {
		difference = array[i] - averageSum;
		sumDifference += pow(difference, 2);
	}

	if (combination == 'g')
		dispersion = sumDifference / n;
		else if (combination == 'n')
			dispersion = sumDifference / (n - 1);

	deviation = sqrt(dispersion);

	return deviation;
}

void matrixVectorProduct(double *a, double *b, double *c, int m, int n)
{
	int h = 0;
	for (int i = 0; i < m; i++) {
		c[i] = 0.0;
		for (int j = 0; j < n; j++) {
			c[i] += a[i * n + j] * b[j];
			h++;
		}
	}
	printf("Число инструкций: %d\n", 4*h);
}

double run()
{
	double timer;
	double *a, *b, *c;
	srand(time(0));

	a = malloc(sizeof(*a) * m * n);
	b = malloc(sizeof(*b) * n);
	c = malloc(sizeof(*c) * m);

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i * n + j] = rand() % 1000;
	}

	for (int j = 0; j < n; j++)
		b[j] = rand() % 1000;

	struct timeval ta, te;
	gettimeofday(&ta, NULL);
	matrixVectorProduct(a, b, c, m, n);
	gettimeofday(&te, NULL);

	timer = te.tv_sec - ta.tv_sec + (te.tv_usec - ta.tv_usec)/1000000.0;
	printf("Time: %.6f sec.\n", timer);

	free(a);
	free(b);
	free(c);
	return timer;
}


void printFile(double AvTime, double AbsError, double RelError, double TaskPerf) {
	FILE *fp;
	fp = fopen("result.csv", "a");

	char *info;
	info = malloc(sizeof(char) * 512);
	info = "Intel(R) Core(TM) i3-6006U CPU @ 2.00GHz;Dgemv;Double;None;10;400.000.000";

	fprintf(fp, "%s;%f;%f;%f%;%.f;\n", info, AvTime, AbsError, RelError, TaskPerf);

	fclose(fp);
}

int main(int argc, char *argv[])
{
	int count = 10;
	int insCount = 400000000;
	double avTime = 0;
	double absError, relError;
	double taskPerf;
	double times[count];

	for (int i = 0; i < count; i++) {
		times[i] = run();
		avTime += times[i];
		taskPerf += (insCount / times[i]);
	}
	avTime /= count;
	absError = meanSquareDeviation(times, count, 'g');
	relError = absError / avTime * 100;
	taskPerf = pow((1 / taskPerf), -1);

	printFile(avTime, absError, relError, taskPerf);

	return 0;
}
