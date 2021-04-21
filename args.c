#include <stdio.h>
#include<string.h>

#include "args.h"
#include "encode.h"
#include "decode.h"

/* Function Definitions */

/*............................................................................................*/
OperationType read_and_validate_cmdline_args(char *argv[], EncodeInfo *encInfo, DecodeInfo *decInfo)
{
	/*Description: Validate command line arguments
	* Inputs: argumet count, arguments, encode struct object address
	* Output: File names for input, secret and output file
	* Return:
	* e_encode for encoding
	* e_decode for decoding
	* e_unsupported for unsupported operation
	*/
	
	//Temp buffer
	char *extension = NULL;
	
	//Check for operation type
	if( 0 == strcmp( argv[1], "-e" ) )
	{
		//Check for original file extension
		if ( ( NULL != argv[2] ) && ( extension = strrchr( argv[2], '.' ) ) && ( 0 == strcmp( extension,".bmp" ) ) )
		{
			//Assign file name to struct
			encInfo->src_image_fname = argv[2];
		}
		else
		{
			//Error
			printf("ERROR: Invalid original file name\n");
			printf("Try \"%s -h\" for more information\n",argv[0]);
			
			//Return unsupported enum type
			return e_unsupported;
		}
		
		//Check for secret file name
		if( NULL != argv[3] )
		{
			//Store secret file name
			encInfo->secret_fname = argv[3];
		}
		else
		{
			//Error
			printf("ERROR: Invalid secret file name\n");
			printf("Try \"%s -h\" for more information\n",argv[0]);
			
			//Return unsupported enum type
			return e_unsupported;
		}
		
		//Assign secret file extenction
		if( extension = strrchr( argv[3], '.' ) )
		{
			//Store byte by byte
			for( short extn = 0; extn < 4; ++extn, ++extension )
			{
				encInfo->secret_file_extn[extn] = *extension;
			}
			//Null the lat byte
			encInfo->secret_file_extn[4] = 0;
		}
		else
		{
			//Assign null for first byte
			encInfo->secret_file_extn[0] = 0;
		}
		
		//Assign stegno image file name
		if( NULL != argv[4] )
		{
			//Check for original file extension
			if ( ( extension = strrchr( argv[4],'.' ) ) && ( 0 == strcmp( extension,".bmp" ) ) )
			{
				//Assign file name to struct
				encInfo->stego_image_fname = argv[4];
			}
			else
			{
				//Info for user
				printf("INFO: Invalid output file name\n");
				printf("INFO: Using default output file name \"stego.bmp\"\n");
				
				//assign file name
				encInfo->stego_image_fname = "stego.bmp";
			}
		}
		else
		{
			//Info for user
			printf("INFO: Output file name not available\n");
			printf("INFO: Using default output file name \"stego.bmp\"\n");
			
			//Assign file name
			encInfo->stego_image_fname = "stego.bmp";
		}
		
		//Return encode enum type
		return e_encode;
	}
	else if( 0 == strcmp( argv[1], "-d" ) )
	{
		//Check for original file extension
		if ( ( NULL != argv[2] ) && ( extension = strrchr( argv[2], '.' ) ) && ( 0 == strcmp( extension,".bmp" ) ) )
		{
			//Assign file name to struct
			decInfo->stego_image_fname = argv[2];
		}
		else
		{
			//Error
			printf("ERROR: Invalid stego file name\n");
			printf("Try \"%s -h\" for more information\n",argv[0]);
			
			//Return unsupported enum type
			return e_unsupported;
		}
		
		//Check for secret file name
		if ( ( NULL != argv[3] ) )
		{
			//Assign file name to struct
			decInfo->secret_fname = argv[3];
		}
		else
		{
			//Info for user
			printf("INFO: Invalid secret file name\n");
			printf("INFO: Using default secret file name \"password.txt\"\n");
			
			//Assign file name
			decInfo->secret_fname = "password.txt";
		}
		
		if( argv[3] && argv[4] )
		{
			//Info for user
			printf("INFO: Discarding extra argument \"%s\"\n",argv[4]);
		}
		
		//Return decode enum type
		return e_decode;
	}
	else
	{
		//Error for invalid operation
		if( 0 != strcmp( argv[1], "-h" ) )
			printf("ERROR: Invalid operation type\n");
		
		//Usage information
		printf("USAGE:\n");
		printf("ARGUMENT: <required> [optional]\n");
		printf("%s: Encoding: %s -e <.bmp file> <.txt file> [output file]\n",argv[0],argv[0]);
		printf("%s: Decoding: %s -d <.bmp file> [output file]\n",argv[0],argv[0]);
	
		//Return unsupported enum type
		return e_unsupported;
	}
}
/*............................................................................................*/