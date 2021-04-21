#include <stdio.h>

#include "args.h"
#include "encode.h"
#include "decode.h"

int main(int argc, char *argv[])
{
	//Declaration
	EncodeInfo encInfo;
	DecodeInfo decInfo;
	OperationType operation;
	
	//Check command line args
	if( argc > 1 && argc < 6)
	{
		//Check for operation typedef
		operation = read_and_validate_cmdline_args( argv, &encInfo, &decInfo );
	}
	else
	{
		//Error
		printf("ERROR: Invalid number of arguments\n");
		printf("Try \"%s -h\" for more information\n",argv[0]);
		
		//Return abort
		return 1;
	}
	
	//Operation type
	if( operation == e_encode )
	{
		//Encoding
		if( e_success == do_encoding( &encInfo ) )
		{
			//Exit
			return 0;
		}
	}
	else if( operation == e_decode )
	{	
		//Decoding
		if( e_success == do_decoding( &decInfo ) )
		{
			//Exit
			return 0;
		}
	}
		
	//Return abort
	return 1;
}
