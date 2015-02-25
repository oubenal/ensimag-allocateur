/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mem.h"
/** squelette du TP allocateur memoire */

void *zone_memoire = 0;
int max = 0;
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

  /* ecrire votre code ici */
	max = get_power2(ALLOC_MEM_SIZE);
	TZL = (void **) malloc(sizeof(void *) * max)
	for(int i = 0; i<max-1; i++) 
		TZL[i]=NULL;
	TZL[max-1] = zone_memoire;
  return 0;
}

void *
mem_alloc(unsigned long size)
{
  /*  ecrire votre code ici */
	int k = get_power(size);
	int i, j;
	for(i=k; i<max; i++)
		if(TZL[i])
			break;

	if ((i==max-1) && TZL[i]==NULL)
	  return 0;  
	
	void *addr1 = TZL[i], *addr2; 
	for(j=i; j>k; j--) {
		*addr2 = addr1 + (unsigned int) 1<< (j-1);
		
		insert(addr2, j-1);
		remove(j);
		
	}
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

