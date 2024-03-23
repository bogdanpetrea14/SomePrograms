//Petrea Bogdan-Vasile, 311CA
#include <stdio.h>
#include <math.h>

long long putere(long long n, long long p)
{
	if (p == 0)
		return 1;
	else
		return n * putere(n, p - 1);
}

long long transformare_baza(long long a)
{
	long long a10 = 0, cfa = 0;
	while (a) {
		a10 += a % 10 * putere(8, cfa);
		cfa++;
		a /= 10;
	}
	return a10;
}

int main(void)
{
	long long N, i, amax = -1, apremax = -1, bmax = -1, bpremax = -1, ps = 0;
	long long a, b, ca, cb;
	double n_a = 0, n_b = 0;
	scanf("%lld\n", &N);
	for (i = 0; i < N; i++) {
		scanf("%lld %lld", &a, &b);
		ca = a;
		cb = b;
		a = transformare_baza(ca);
		b = transformare_baza(cb);
		if (a > apremax && a < amax) {
			amax = apremax;
			apremax = a;
		} else {
			if (a > amax) {
				apremax = amax;
				amax = a;
			}
		}
		if (b > bpremax && b < bmax) {
			bmax = bpremax;
			bpremax = b;
		} else {
			if (b > bmax) {
				bpremax = bmax;
				bmax = b;
			}
		}
		n_a += a * a;
		n_b += b * b;
		ps += a * b;
	}
	n_a = sqrt(n_a);
	n_b = sqrt(n_b);
	printf("%lld\n", ps);
	printf("%lld %lld\n", apremax, bpremax);
	printf("%.7lf %.7lf\n", n_a, n_b);
	return 0;
}
