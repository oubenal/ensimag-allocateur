/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mem.h"

void *zone_memoire = 0;

/* Structure de liste */
struct elem {
	unsigned long size;
	struct elem* next;
};
typedef struct elem Elem;

/* Tableau des zones libres */
Elem** TZL=0;

/* Taille du tableau*/
unsigned int max = BUDDY_MAX_INDEX;


void insertTZL(int index, Elem* elem) {
	if(TZL[index] == 0)
		TZL[index] = elem;
	else {
		Elem* tmp = TZL[index]->next;
		TZL[index]->next = elem;
		elem->next = tmp;
	}
}

void delete(Elem** list, Elem* elem) {
	if (*list != 0) {
		if (*list == elem) {
			*list = elem->next;
			elem->next = 0;
		} else {
			delete(&((*list)->next), elem);
		}
	}
}

void deleteTZL(int index, Elem* elem) {
	delete(&TZL[index], elem);
}

// work only if x is a power of 2
int logOf2(unsigned long x) {
	int i = 0;
	while (!(x & 1)) {
		i++;
		x >>= 1;
	}
	return i;
}


unsigned long roundToPower2(unsigned long x) {
	if (x < sizeof(Elem))
		x = sizeof(Elem);
	
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return x;
}



void* split(Elem* block, unsigned long size) {
	int index = logOf2(block->size);
	for(int i=index; i>0; i--){
		deleteTZL(i, block);

		block->size = block->size / 2;
		insertTZL(i - 1, block);

		// Calcul de l'emplacement du compagnon
		Elem* buddy = (Elem*)((unsigned long)block + block->size);
		buddy->size = block->size; buddy->next = 0;

		insertTZL(i - 1, buddy);
		if (block->size == size) {
			deleteTZL(i - 1, block);
			return (void*)block;
		}
	}
	return 0;
}

void* findBlock(int index, unsigned long size) {
	int i ;
	for(i=index; i<=max; i++) {
		if (TZL[i] != 0) {
			break;
		}
	}
	if(i > max) {
		return 0;
	}
	Elem* block = TZL[i];
	if (block == 0) {
		return 0;
	} else if (block->size == size) {
		deleteTZL(i, block);
		return (void*)block;
	} else {
		return split(block, size);
	}
}

Elem* findBuddy(void* ptr, unsigned long size) {
	int index = logOf2(size);
	void* buddy = zone_memoire;
	
	buddy += (ptr - zone_memoire) ^ size; // xor

	if (index < max) {
		Elem* curr = TZL[index];
		while (curr != 0 && (curr != buddy)) {
			curr = curr->next;
		}
		if (curr != 0) {
			return curr;
		}
	}
	return 0;
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

  if (!TZL) 
    {
	  TZL = (Elem **) malloc((max + 1)* sizeof(Elem *));
	}
	
  if (TZL == 0) 
	{
	  perror("mem_init:");
	  return -1;
	}
	
  for (int i = 0; i <= max ; i++) 
	{
	  TZL[i] = 0;
	}

  Elem* elem = (Elem*)zone_memoire;
  elem->size = ALLOC_MEM_SIZE;
  elem->next = 0;
  TZL[max] = zone_memoire;

  return 0;
  
}

void *
mem_alloc(unsigned long size)
{

	if (size == 0) {
		return 0;
	}
	size = roundToPower2(size);
	
	int index = logOf2(size);

	Elem* elem = findBlock(index, size);
	if (elem != 0) {
		deleteTZL(index, elem);
		return (void*)elem;
	}

	return 0;
    
}

int 
mem_free(void *ptr, unsigned long size)
{
	if (ptr == 0 || ptr > zone_memoire + ALLOC_MEM_SIZE || ptr < zone_memoire) {
		return -1;
	}
	size = roundToPower2(size);

	int index = logOf2(size);
	Elem* buddy = findBuddy(ptr, size);
	deleteTZL(index, buddy);

	if (buddy == 0) {
		Elem* elem = (Elem*)ptr;
		elem->size = size; elem->next = 0;
		insertTZL(index, elem);
		return 0;
	} else if ((void *)buddy < ptr) {
		return mem_free(buddy, size * 2);
	} else {
		return mem_free(ptr, size * 2);
	}
}


int
mem_destroy()
{ for (int i = 0 ; i <= max ; i++) 
	{
	  TZL[i] = 0;
	}

  free(zone_memoire);
  zone_memoire = 0;
  return 0;
}
