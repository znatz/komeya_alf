#include <machine.h>
#include <_h_c_lib.h>
#include "typedefine.h"
#include <stdio.h>

void main(void);

#pragma section AplProgStart

void apl_start(void)
{
	_INITSCT();
	
	main();
}

#pragma section

#pragma section AplProgEnd
volatile unsigned long apl_prog_end;
#pragma section

#pragma section AplWorkEnd
volatile unsigned long apl_work_end;
#pragma section
