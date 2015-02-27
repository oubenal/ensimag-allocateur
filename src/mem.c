/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "mem.h"
/** squelette du TP allocateur memoire */

void *zone_memoire = 0;
int max;
void **TZL = 0;
/* ecrire votre code ici */

unsigned int get_power2(unsigned long x) {
	return ceil(log(x)/log(2));
}

int 
mem_init()
{
  if (! zone_memoire)
    zone_memoire = (void *) malloc(ALLOC_MEM_SIZE);
  if (zone_memoire == 0)
    {
      perror("mem_init:");
      return -1;
    }

	max = get_power2(ALLOC_MEM_SIZE);
	TZL = (void **) malloc(sizeof(void *) * (max+1));
	for(int i = 0; i<max; i++) {
		TZL[i]=0;
	}
	TZL[max] = zone_memoire;
  return 0;
}

void insertTZL(void *addr, int j) {
	void *tmp = TZL[j];
	if(!TZL[j]) {
		TZL[j] = addr;
		*(void **)addr = NULL;
	}
	else {
		while(!(*(void **)tmp)) {
			tmp = *(void **)tmp;
		}
		*(void **)tmp = addr;
		*(void **)addr = 0;
	}	
}

void removeTZL(int j) {
	void *curr = TZL[j], * prec;
	if(!(*(void **)curr)) {
		TZL[j]=0;
	}
	else {
		prec = curr;
		curr = *(void **)prec;
		while(*(void **)curr) {
			prec = curr; curr = *(void **)prec;
		}
		*(void **)prec = 0;
	}
	
}

void *get_mem(int k){
	void *curr = TZL[k], *addr;
	while(*(void **)curr) {
		curr = *(void **)curr;
	}
	addr = *(void **)curr + sizeof(void *);
	return addr;
}

void *
mem_alloc(unsigned long size)
{
	if(!TZL)
		return 0;
  /*  ecrire votre code ici */
	int k = get_power2(size);//printf("\n ****2^k = %lu, k = %d ******\n", size, k);
	int i, j;
	for(i=k; i<=max; i++)
		if(TZL[i])
			break;

	if (i == max && TZL[max]==0) {
		return 0;
	} 
//	void *addr1 = TZL[i], *addr2; 
	void *addr = TZL[i];
	TZL[i] = 0;
	for(j=i-1; j>=k; j--) {
		TZL[j] = addr;
		TZL[j] += (unsigned int) 1<< j;
	}
	return addr;
 

//		addr2 = addr1;
//		*(void **)addr2 += (unsigned int) 1<< (j-1);
		
//		insertTZL(addr2, j-1);
//		removeTZL(j);
	

return 0;
}

int 
mem_free(void *ptr, unsigned long size)
{
  /* ecrire votre code ici */
  return 0;
}

int
mem_destroy()
{
  /* ecrire votre code ici */

  free(zone_memoire);
  zone_memoire = 0;
  return 0;
}

