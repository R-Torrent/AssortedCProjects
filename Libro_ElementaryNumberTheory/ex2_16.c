/* Number Theory - exercise 2.16 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 2000 /* can factor numbers <= MAX^2 */

void main(int argc, char *argv[])
{
	int i, j, num, maxprime;
	static int prime[MAX];

	if(argc != 2)
		exit(1);
	num = atoi(argv[1]);
	if(num < 2 || num > MAX*MAX){
		printf("error: n[2,%d]\n", MAX*MAX);
		exit(1);
	}

	/* Sieve of Eratosthenes
	   prime[i-1]=1 if i is prime; 0 if composite */
	maxprime = (int)floor(sqrt(num));
	prime[0] = 1;
	for(i = 2; i <= maxprime; i++){
		if(!prime[i-1])
			prime[i-1] = 2;
		for(j = 2*i; j <= maxprime; j += i)
			prime[j-1] = 1;
	}
	for(i = 1; i <= maxprime; i++)
		prime[i-1]--;

	printf("Prime factor(s) of %d:\n", num);
	i = 2;
	while(i <= maxprime){
		if(prime[i-1] && !(num%i)){
			printf("%d\n", i);
			num /= i;
			maxprime = (int)floor(sqrt(num));
			i = 2;
		}
		else
			i++;
	}
	printf("%d\n",num);

	return;
}