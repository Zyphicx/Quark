#include "comperror.h"

void printerror(int errcode, char *errmsg){
    fprintf(stderr, "Error %d: %s", errcode, errmsg);
}
