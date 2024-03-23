//Petrea Bogdan-Vasile, 311CA
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	long tab[1000][1000], n, i, j, dist = 0, val, linie;
	char coloana1, coloana2, coloana3;
	scanf("%ld\n", &n);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			scanf("%ld ", &tab[i][j]);
	i = 0;
	j = 0;
	while (tab[i][j]) {
		val = tab[i][j];
		tab[i][j] = 0;
		if (val >= 0)
			dist += val;
		else
			dist += -val;
		if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))
			if (val >= 0)
				if (val + j < n)
					j = j + val;
				else
					j = (j + val) % n;
			else
				if (-val <= j)
					j = j + val;
				else
					j = n + (j + val) % n;
		else
			if (val >= 0)
				if (val + i < n)
					i = i + val;
				else
					i = (i + val) % n;
			else
				if (-val <= i)
					i = i + val;
				else
					i = n + (i + val) % n;
	}
	printf("%ld\n", dist);
	linie = n - i;
	printf("%ld ", linie);
	if (j <= 26) {
		coloana1 = (char)('A' + j);
		printf("%c\n", coloana1);
	}
	if (j > 26 && j <= 676) {
		coloana1 = (char)('A' + j / 26 - 1);
		coloana2 = (char)('A' + j % 26);
		printf("%c%c\n", coloana1, coloana2);
	}
	if (j > 676) {
		coloana1 = (char)('A');
		coloana2 = (char)('A' + (j - 676) / 26);
		coloana3 = (char)('A' + (j - 676) % 26);
		printf("%c%c%c\n", coloana1, coloana2, coloana3);
	}
	return 0;
}
