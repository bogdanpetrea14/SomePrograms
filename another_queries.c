//Petrea Bogdan-Vasile, 311CA
#include <stdio.h>

void op1(long N, long S[400][400], long X, long vl[400][400])
{
	long i, val;
	for (i = 0; i < N * N; i++)
		if (S[X - 1][i] != 0) {
			val = S[X - 1][i] - 1;
			if (vl[X - 1][val] != 0)
				vl[X - 1][val]--;
		}
}

void op2(long N, long S[400][400], long Y, long vc[400][400])
{
	long i, val;
		for (i = 0; i < N * N; i++)
			if (S[i][Y - 1] != 0) {
				val = S[i][Y - 1] - 1;
				if (vc[val][Y - 1] != 0)
					vc[val][Y - 1]--;
			}
}

void op3(long N, long S[400][400], long Z, long vca[400][400])
{
	long i, j, lc = 0, cc = 0, val;
	if (Z % N == 0)
		lc = Z / N - 1;
	else
		lc = Z / N;
	if (Z % N != 0)
		cc = Z  % N - 1;
	else
		cc = N - 1;
	for (i = lc * N; i < (lc + 1) * N; i++)
		for (j = cc * N; j < (cc + 1) * N; j++)
			if (S[i][j] != 0) {
				val = S[i][j] - 1;
				if (vca[Z][val] != 0)
					vca[Z][val]--;
			}
	for (i = 0; i < N * N; i++)
		printf("%lu ", vca[Z][i]);
	printf("\n");
}

void op4(long N, long S[][400], long vl[][400], long vc[][400], long vca[][400])
{
	long i, k, X, Y;
	scanf("%lu %lu\n", &X, &Y);
	if ((X - 1) / N == 0)
		k = (Y - 1) % N;
	else
		k = ((X - 1) / N) * N + (Y - 1) % N + 1;
	op1(N, S, X, vl);
	op2(N, S, Y, vc);
	if (S[X - 1][Y - 1] != 0) {
		for (i = 0; i < N * N; i++)
			printf("0 ");
	} else {
		for (i = 0; i < N * N; i++)
			if (vl[X - 1][i] == vc[i][Y - 1] && vl[X - 1][i] == 1) {
				if (vl[X - 1][i] == vca[k][i])
					printf("1 ");
			} else {
				printf("0 ");
			}
	}
	printf("\n");
}

void op5(long S[400][400], long X, long Y, long C)
{
	if (S[X - 1][Y - 1] == 0)
		S[X - 1][Y - 1] = C;
}

int op6(long S[400][400], long N)
{
	int i, j, k, okz = 1, okc = 0;
	for (i = 0; i < N * N /*&& okc == 0*/; i++)
		for (j = 0; j < N * N - 1 /*&& okc == 0*/; j++)
			for (k = j + 1; k < N * N /*&& okc == 0*/; k++) {
				if ((S[i][j] == S[i][k] && S[i][j] != 0))
					okc = 1;
				if ((S[j][i] == S[k][i] && S[j][i] != 0))
					okc = 1;
				if (S[i][k] == 0)
					okz = 0;
				if (okz == 0 && okc == 0)
					return 0;
				if ((okz == 0 && okc == 1) || (okz == 1 && okc == 1))
					return 1;
			}
	return 2;
}

int main(void)
{
	long N, X, Y, Z, C, S[400][400], vl[400][400], vc[400][400], vca[400][400];
	long M, i, j, joc, rez;
	scanf("%lu %lu\n", &N, &M);
	for (i = 0; i < N * N; i++)
		for (j = 0; j < N * N; j++) {
			vc[i][j] = 1;
			vl[i][j] = 1;
			vca[i][j] = 1;
		}
	for (i = 0; i < N * N; i++)
		for (j = 0; j < N * N; j++)
			scanf("%lu ", &S[i][j]);
	for (j = 0; j < M; j++) {
		scanf("%lu ", &joc);
		if (joc == 1) {
			scanf("%lu\n", &X);
			op1(N, S, X, vl);
			for (i = 0; i < N * N; i++)
				printf("%lu ", vl[X - 1][i]);
			printf("\n");
		}
		if (joc == 2) {
			scanf("%lu\n", &Y);
			op2(N, S, Y, vc);
			for (i = 0; i < N * N; i++)
				printf("%lu ", vc[i][Y - 1]);
			printf("\n");
		}
		if (joc == 3) {
			scanf("%lu\n", &Z);
			op3(N, S, Z, vca);
		}
		if (joc == 4)
			op4(N, S, vl, vc, vca);
		if (joc == 5) {
			scanf("%lu %lu %lu\n", &X, &Y, &C);
			op5(S, X, Y, C);
		}
		if (joc == 6) {
			rez = op6(S, N);
			printf("%lu\n", rez);
		}
	}
	return 0;
}
