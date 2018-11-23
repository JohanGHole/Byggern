#include "memory.h"
#include "bit_manipulation.h"

void memory_init(){
	// enable extern memory
	set_bit(MCUCR,SRE);
	set_bit(SFIOR,XMM2);
}