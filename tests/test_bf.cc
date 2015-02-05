/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <gtest/gtest.h>
#include <vector>

#include "../src/mem.h"

constexpr int NBCHUNK = 8;
constexpr int S8 = ALLOC_MEM_SIZE/8;
using namespace std;

TEST( Variantes, bf ) {
  int multi = 0;
  vector<void *> tab(NBCHUNK);
  
#ifndef BF
  return;
#else
  multi=2;
#endif

  ASSERT_EQ(multi, 2);
  ASSERT_EQ( mem_init(), 0 );

  for(auto &t : tab) {
    t = mem_alloc(S8);
    ASSERT_NE( t, (void*)0 );
    memset(t, 3, S8);
  }
  ASSERT_EQ( mem_free(tab[6], S8), 0);
  ASSERT_EQ( mem_free(tab[7], S8), 0);

  
  int decal = multi*sizeof(void*);
  tab[6] = mem_alloc(S8-decal);
  ASSERT_NE( tab[6], (void *)0 );
  memset(tab[6], 3, S8-decal);
  tab[7] = mem_alloc(S8+decal);
  ASSERT_NE( tab[7], (void*)0 );
  memset(tab[6], 3, S8+decal);
    

  for(int i=0; i < 6; i+= 2)
    ASSERT_EQ( mem_free( tab[i], S8 ), 0);
  ASSERT_EQ( mem_free(tab[6], S8 - decal), 0);
  
  ASSERT_EQ( mem_alloc(3*S8), (void *)0);
  
  tab[6] = mem_alloc(S8-decal);
  ASSERT_NE( tab[6], (void *)0 );
  tab[0] = mem_alloc(S8);
  ASSERT_NE( tab[0], (void *)0 );
  tab[2] = mem_alloc(S8);
  ASSERT_NE( tab[2], (void *)0 );
  tab[4] = mem_alloc(S8);
  ASSERT_NE( tab[4], (void *)0 );

  for(int i=0; i < 6; i++)
    ASSERT_EQ( mem_free( tab[i], S8 ), 0);

  ASSERT_EQ( mem_free(tab[7], S8+decal), 0);
  ASSERT_EQ( mem_free(tab[6], S8-decal), 0);

  tab[0] = mem_alloc(ALLOC_MEM_SIZE);
  ASSERT_NE( tab[0], (void *)0 );
  memset(tab[0], 3, ALLOC_MEM_SIZE);

  ASSERT_EQ(mem_free(tab[0], ALLOC_MEM_SIZE), 0);
}
