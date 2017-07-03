#ifndef COMPERROR_H
#define COMPERROR_H

#include "stdio.h"

/*Error codes*/

#define ERR_STRAYTOKEN      1

void printerror(int errcode, char *errmsg);

#endif
