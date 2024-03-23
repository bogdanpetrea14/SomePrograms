//Petrea Bogdan-Vasile, 311CA
#include <stdio.h>
#include <math.h>
void sortare(int cv[26], char ccv[27])
{
	int i, j, aux;
	char aux2;
	for (i = 0; i < 25; i++) {
		for (j = i + 1; j < 26; j++)
			if (cv[i] < cv[j]) {
				aux = cv[i];
				cv[i] = cv[j];
				cv[j] = aux;
				aux2 = ccv[i];
				ccv[i] = ccv[j];
				ccv[j] = aux2;
			} else {
				if (cv[i] == cv[j])
					if (ccv[i] > ccv[j]) {
						aux = cv[i];
						cv[i] = cv[j];
						cv[j] = aux;
						aux2 = ccv[i];
						ccv[i] = ccv[j];
						ccv[j] = aux2;
					}
			}
	}
}

void atribuire(int v[26], int cv[26], char ccv[26])
{
	for (int i = 0; i < 26; i++) {
		cv[i] = v[i];
		ccv[i] = (char)(i + 97);
	}
}

void alert(char ccv[26], int cv[26], int total)
{
	int m, n;
	printf("%c ", ccv[0]);
	m = cv[0];
	n = total;
	while (m != n)
		if (n > m)
			n = n - m;
		else
			m = m - n;
	printf("%d/%d\n", cv[0] / m, total / m);
}

int main(void)
{
	int K, v[26] = {0}, cv[26], g = 0, i = 0, total = 0, alerta = 0;
	int k;
	char l, L, ccv[27];
	do {
		scanf("%c\n", &l);
		if (l == 'Q') {
			scanf("%c ", &L);
			i = (int)(L - 97);
			printf("%c %d\n", L, v[i]);
		}
		if (l == 'E') {
			scanf("%c\n", &L);
			i = (int)(L - 97);
			total = total - v[i];
			v[i] = 0;
		}
		if (l == 'T') {
			scanf("%d ", &K);
			atribuire(v, cv, ccv);
			sortare(cv, ccv);
			for (i = 0; i < K - 1; i++)
				printf("%c ", ccv[i]);
			printf("%c", ccv[K - 1]);
			printf("\n");
		}
		if ('a' <= l && l <= 'z') {
			total++;
			alerta++;
			i = (int)(l) - 97;
			v[i]++;
			for (k = 0; k < 26 && g < 2; k++)
				if (v[k] > 0)
					g++;
			atribuire(v, cv, ccv);
			//sortare(cv, ccv);
			if (cv[0] * 2 >= total && g == 2 && alerta >= 5) {
				alert(ccv, cv, total);
				alerta = 0;
			}
		}
	} while (('a' <= l && l <= 'z') || (l == 'E' || l == 'T' || l == 'Q'));
	return 0;
}
