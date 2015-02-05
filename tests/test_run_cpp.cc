/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <unistd.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>
#include <random>
#include <functional>

#include "test_run.H"


#ifndef ALLOC_MEM_SIZE
#error "#define ALLOC_MEM_SIZE XXX n'est pas defini dans mem.h ! Interruption de la compilation"
#endif


/* options */
static bool be_verbose = false; 

/* manipulation de la liste des allocations */


void allocat::doAlloc()
{
  static int nb=0;

  assert (size > 0);
  
  adr = mem_alloc (size);


  if ( adr == (char *)0 )
    {
      /* l'allocation a echoue */
      printf ("Une allocation de %d a echoue !\n", size );
      
      adr = nullptr;
    }
  else
    {
      idx = nb;
      nb ++;
      if (be_verbose)
	printf ("alloc %d (taille: %d): %p\n", idx, size, adr);
      
      /* ecrire dans le bloc */
      memset (adr, 0, size );
    }
  
  
}

void allocat::doLiberer()
{
  assert (size > 0);
  
  if (adr)
    {
      /* ecrire dans le bloc */
      memset (adr, 0, size );

      if (be_verbose)
	printf ("libere %d (taille: %d): %p\n", idx, size, adr);
      
      mem_free (adr, size);
      adr = nullptr;
    }
}

void random_run_cpp(int nb=100, bool verbose=false)
{
  be_verbose = verbose;
  
  
  vector<allocat> liste_allocation;
    
  /* choisir les allocations */
  fillList_fibo<ALLOC_MEM_SIZE> ( liste_allocation );
    
  /* afficher l'etat de la memoire */
    
  /* faire les allocations */
  for(auto &l: liste_allocation)
    l.doAlloc();
  
  /* melanger la liste */
  shuffle (liste_allocation);
  
  /* faire les deallocations */
  for(auto &l: liste_allocation)
    l.doLiberer ();
  
}

