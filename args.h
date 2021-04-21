#ifndef ARGS_H
#define ARGS_H

//Contains user defined types
#include "common.h"
#include "types.h" 

#include "encode.h"
#include "decode.h"

/* Validate arguments function prototype */
/*............................................................................................*/

/* Read and validate args from argv */ //ready ,tested
OperationType read_and_validate_cmdline_args(char *argv[], EncodeInfo *encInfo, DecodeInfo *decInfo);

/*............................................................................................*/

#endif