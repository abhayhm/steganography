//Macro for fcloseall()
#define _GNU_SOURCE

//Macro for changine byte order (endiness)
#define swap_endian(byte) (((byte & 0xff000000) >> 24)| ((byte & 0x00ff0000) >> 8) | ((byte & 0x0000ff00) << 8) | ((byte & 0x000000ff) << 24))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"

/* Function Definitions */

/*............................................................................................*/
Status do_decoding(DecodeInfo *decInfo)
{
	/*Description: Do decoding
	* Input: struct DecodeInfo
	* Output: Secret file
	* Return Value: success/failure
	*/	
	
	//Check for files accessability
	if( e_success == open_decode_files(decInfo) )
	{
		//Info for user
		printf("INFO: ## Decoding Process Started ##\n");
		
		//Check magic string signature
		if( e_success ==  decode_magic_string(MAGIC_STRING, decInfo) )
		{
			//Check secret file extension
			if( e_success == decode_secret_file_extn(decInfo) )
			{
				//Decode secret file data
				if( e_success == decode_secret_file_data(decInfo) )
				{
					//Info for user
					printf("INFO: ## Decoding Done Successfully ##\n");
					
					//Return success enum type
					return e_success;
				}
			}
		}
	}

	//Return failure enum type
    return e_failure;	
}

Status open_decode_files(DecodeInfo *decInfo)
{
	/* Description: Open files
	* Inputs: Secret filename and Stego Image filename
	* Output: FILE pointers for above files
	* Return Value: success/failure
	*/
 
	//Info for user
	printf("INFO: Opening required files\n");
	
    // Stego Image file
	decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    
	//Do Error handling
    if (NULL == decInfo->fptr_stego_image)
    {
		//Error
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
		
		//Return failure enum type
    	return e_failure;
    }
	
	//Info for user
	printf("INFO: Opened %s successfully\n",decInfo->stego_image_fname);
	
    // Secret file
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "wb");
	
    //Do Error handling
    if (NULL == decInfo->fptr_secret)
    {
		//Error
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
		
		//Return failure enum type
    	return e_failure;
    }

	//Info for user
	printf("INFO: Opened %s successfully\n",decInfo->secret_fname);
	printf("INFO: Done, files opened\n");
	
    //Return success enum type
    return e_success;
}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
	/*Description: decode magic string and validate
	* Input: magic_string and structure
	* Output: Encodes magic string to destination file
	* Return Value: success/failure
	*/
	
	//Copying file pointer to local pointer
	FILE *fptr_stego_image = decInfo->fptr_stego_image;
	
	//Start from 54th byte of file
	fseek(fptr_stego_image, 54L, SEEK_SET);
	
	//Temp buffer
	char decoded_magic_string[sizeof(MAGIC_STRING)];
	
	//Info for user
	printf("INFO: Decoding Magic String Signature\n");
	
	if( e_success == decode_data(decoded_magic_string, sizeof(char), strlen(MAGIC_STRING), fptr_stego_image) )
	{
		
		//Adding null
		decoded_magic_string[sizeof(MAGIC_STRING)] = 0;

		//Info for user
		printf("INFO: Done\n");
		printf("INFO: Validating Magic String Signature\n");
	
		//Verify magic string
		if( 0 == strcmp(MAGIC_STRING, decoded_magic_string) )
		{
			//Info for user
			printf("INFO: Done, Valid\n");
		
			//Return success enum type
			return e_success;
		}
	
		//Error
		printf("ERROR: Invalid Magic String Signature\n");
	
		//Return failure enum type
		return e_failure;
	}
	
	//Error
	printf("ERROR: Error decoding Magic String Signature\n");
	
	//Return failure enum
	return e_failure;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{	
	/*Description: Decode secret file extension
	* Input: struct DecodeInfo
	* Output: Secret file extension
	* Return Value: success/failure
	*/	
	
	//Temp pointer
	char *extension;
	
	//Get secret file extension size
	int secret_file_extn_size = decode_secret_file_extn_size( decInfo );
	
	//Copying file pointer to local pointer
	FILE *fptr_stego_image = decInfo->fptr_stego_image;
	
	//Start from 54th byte of file
	fseek(fptr_stego_image, 102L, SEEK_SET);
	
	//Info for user
	printf("INFO: Decoding secret file extension\n");
	
	//Check for secret file extension size
	if( secret_file_extn_size > 0 )
	{
		//Decode secret file extension
		if( e_success == decode_data(decInfo->secret_file_extn, sizeof(char), secret_file_extn_size, fptr_stego_image) )
		{
			//Info for user
			printf("INFO: Done\n");
			printf("INFO: Validating secret file extension with given secret file extension\n");
			
			//Validate secret file extension
			if( ( extension = strrchr(decInfo->secret_fname,'.') ) && ( 0 == strcmp(decInfo->secret_file_extn,extension) ) )
			{
				//Info for user
				printf("INFO: Done, Valid\n");
				
				//Return success enum type
				return e_success;
			}
			else
			{
				//Info for user
				printf("INFO: Done, Invalid\n");
				printf("INFO: Rename the file name to .txt before opening\n");
				
				//Return success enum type
				return e_success;
			}
		}
		else
		{
			//Error
			printf("ERROR: Unable to decode secret file extension\n");
			
			//Return failure enum type
			return e_failure;
		}
	}
	
	//Error
	printf("ERROR: Invalid secret file extension size\n");
	
	//Return failure enum type
	return e_failure;
}

int decode_secret_file_extn_size(DecodeInfo *decInfo)
{	
	/*Description: Decode secret file extension size
	* Input: struct DecodeInfo
	* Output: secret file extension size
	* Return Value: secret file extension size
	*/	
	
	//Temp integer
	int secret_file_extn_size = 0;
	
	//Copying file pointer to local pointer
	FILE *fptr_stego_image = decInfo->fptr_stego_image;
	
	//Start from 54th byte of file
	fseek(fptr_stego_image, 70L, SEEK_SET);
	
	//Info for user
	printf("INFO: Decoding secret file extension size\n");
	
	//Decode secret file extension size
	if( e_success == decode_data((char*)&secret_file_extn_size, sizeof(char), sizeof(int), fptr_stego_image) )
	{
		//Info for user
		printf("INFO: Done\n");
		
		//Conver to little endian
		secret_file_extn_size = swap_endian(secret_file_extn_size);
		
		//Return secret file extension size
		return secret_file_extn_size;
	}
	
	//Error
	printf("ERROR: Unable to decode secret file extension size\n");
	
	//Return failure
	return -1;
}



Status decode_secret_file_data(DecodeInfo *decInfo)
{
	/*Description: Decode secret file data
	* Input: struct DecodeInfo
	* Output: secret file data
	* Return Value: success/failure
	*/	
	
	//Temp buffer
	char *secret_buffer = NULL;
	
	//Get secret fille size
	int secret_file_size = decode_secret_file_size( decInfo );
	
	//Validate secret file size
	if( secret_file_size > 0)
	{
		//Allocate memory for secret buffer
		secret_buffer = (char *)calloc(secret_file_size, sizeof(char));
		
		//Check if memory is allocated
		if( NULL == secret_buffer )
		{
			//Error
			printf("ERROR: Unable to allocate buffer memory\n");
			
			//Return failure enum type
			return e_failure;
		}
		
		//Copying file pointer to local pointer
		FILE *fptr_stego_image = decInfo->fptr_stego_image;
		FILE *fptr_secret = decInfo->fptr_secret;
		
		
		//Seek to secret file data
		fseek(fptr_stego_image, 166L, SEEK_SET);
		fseek(fptr_secret, 0L, SEEK_SET);
		
		//Info for user
		printf("INFO: Decoding secret file data\n");
		
		//Decode secret file data
		if( e_success == decode_data(secret_buffer, sizeof(char), secret_file_size, fptr_stego_image) )
		{
			//Info for user
			printf("INFO: Done\n");
			printf("INFO: Writing secret data to \"%s\"\n",decInfo->secret_fname);
			
			//Write secret data to secret file
			if( secret_file_size == fwrite(secret_buffer, sizeof(char), secret_file_size, fptr_secret))
			{
				//Info for user
				printf("INFO: Done\n");
				
				//Free memory allocated for secret buffer
				free(secret_buffer);
				
				//Return success enum type
				return e_success;
			}
			else
			{
				//Error
				printf("ERROR: Unable to write data to \"%s\"\n",decInfo->secret_fname);
				
				//Free memory allocated for secret buffer
				free(secret_buffer);
				
				//Return failure enum type
				return e_failure;
			}
		}
		else
		{
			//Error
			printf("ERROR: Unable to decode secret file data\n");
			
			//Free memory allocated for secret buffer
			free(secret_buffer);
			
			//Return failure enum type
			return e_failure;
		}
	}
	
	//Error
	printf("ERROR: Invalid secret file size\n");
	
	//Free memory allocated for secret buffer
	free(secret_buffer);
	
	//Return failure enum type
	return e_failure;
}

int decode_secret_file_size(DecodeInfo *decInfo)
{
	/*Description: Decode secret file size
	* Input: struct DecodeInfo
	* Output: secret file size
	* Return Value: secret file size
	*/	
	
	//Temp integer
	int secret_file_size = 0;
	
	//Copying file pointer to local pointer
	FILE *fptr_stego_image = decInfo->fptr_stego_image;

	//Start from 54th byte of file
	fseek(fptr_stego_image, 134L, SEEK_SET);
	
	//Info for user
	printf("INFO: Decoding secret file size\n");
	
	//Decode secret file size
	if( e_success == decode_data((char*)&secret_file_size, sizeof(char), sizeof(int), fptr_stego_image) )
	{
		//Info for user
		printf("INFO: Done\n");
		
		//Convert to little endian
		secret_file_size = swap_endian(secret_file_size);
		
		//Return secret file size
		return secret_file_size;
	}
	
	//Error
	printf("ERROR: Unable to decode secret file size\n");
	
	//Return failure
	return -1;
}

/*............................................................................................*/
/*............................................................................................*/

Status decode_data(char *data, int size, int nbytes, FILE *fptr_stego_image)
{
	/*Description: Decode data from buffer 
	* Input: buffer ,file pointer, size and nbytes
	* Output: Decoded buffer data
	* Return Value: success/failure
	*/	
	
	//char image_buffer[nbytes * 8];
	char *image_buffer = (char*)calloc(nbytes*8,sizeof(char));
	
	//Check for memory allocation
	if( image_buffer == NULL)
	{
		//Error
		printf("ERROR: Unable to allocate temp memory buffer\n");
		
		//Return failure enum type
		return e_failure;
	}	

	//Read nbytes to memory buffer 
	if( ( nbytes * 8 ) ==  fread(image_buffer, sizeof(char), ( nbytes * 8 ), fptr_stego_image) )
	{
		//Decode byte by byte
		for( int byte = 0; byte < nbytes; ++byte )
		{
			//Decode byte
			decode_byte_to_lsb( (data + byte), (image_buffer + byte * 8));
		}
		
		//Delete buffer memory
		free(image_buffer);
		
		//Return success enum type
		return e_success;
	}
	
	//Error
	printf("ERROR: Unable to read data from buffer\n");
	
	//Delete buffer memory
	free(image_buffer);
	
	//Return failure enum type
	return e_failure;
}


Status decode_byte_to_lsb(char *data, char *image_buffer)
{
	/*Description: Decode 1 byte data
	* Input: storage buffer and image buffer
	* Output: Decoded byte of data
	* Return Value: success/failure
	*/	
	
	//Initialize data to avoid errors
	*data = 0;
	
	//Decode a byte
	for(int byte = 0; byte < 8; ++byte)
	{
		//Check for bit
		if( (image_buffer[byte]) & 1 )
		{
			//For 1
			*data = ( (*data) | 1 << ( 7 - byte ) );
		}
		else
		{
			//For 0
			*data = ( (*data) & (~1) << ( 7 - byte ) );
		}
	}
	
	//Return success enum type
	return e_success;
}

/*............................................................................................*/