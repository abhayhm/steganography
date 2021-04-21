//Macro for fcloseall()
#define _GNU_SOURCE

#include <stdio.h>
#include<stdlib.h>
#include "encode.h"

/* Function Definitions */

/*............................................................................................*/
Status do_encoding(EncodeInfo *encInfo)
{
	/*Description: Do encoding
	* Input: struct EncodeInfo
	* Output: Encoded image
	* Return Value: success/failure
	*/
	
	//Check for files accessability
	if( e_success == open_encode_files(encInfo) )
	{
		//Info for user
		printf("INFO: ## Encoding Process Started ##\n");
		
		//Check for encode capicaty for original image
		if( e_success == check_capacity(encInfo) )
		{
			//Copying file pointer to local pointer
			FILE *fptr_src_image = encInfo->fptr_src_image;
			FILE *fptr_stego_image = encInfo->fptr_stego_image;
		
			//Check if header copied successfully
			if( e_success == copy_bmp_header(fptr_src_image, fptr_stego_image) )
			{
				//Check for magic string encoded successfully
				if( e_success == encode_magic_string( MAGIC_STRING, encInfo) )
				{
					//Check for secret file extension size encoded successfully
					if( e_success == encode_secret_file_extn_size(MAX_SECRET_FILE_EXTN, encInfo) )
					{
						//Check for secret file extension encoded successfully
						if( e_success == encode_secret_file_extn(encInfo->secret_file_extn, encInfo) )
						{
							//Check for secret file size encoded successfully
							if( e_success == encode_secret_file_size(encInfo->secret_file_size, encInfo) )
							{
								//Check for secret file data encoded successfully
								if( e_success == encode_secret_file_data(encInfo) )
								{
									//Seek to leftover data
									fseek( fptr_src_image, ((encInfo->secret_file_size)) * 8 + 166L, SEEK_SET);
									fseek( fptr_stego_image, ((encInfo->secret_file_size)) * 8 + 166L, SEEK_SET);
									
									//Check for left over data copied successfully
									if( e_success == copy_remaining_img_data(fptr_src_image, fptr_stego_image) )
									{
										//Info for user
										printf("INFO: ## Encoding Done Successfully ##\n");
										
										//Return success enum type
										return e_success;
									}
								}
							}
						}	
					}
				}
			}
		}
	}
	
	//Return failure enum type
    return e_failure;
}

Status open_encode_files(EncodeInfo *encInfo)
{
	/* Description: Open files
	* Inputs: Source image filename, Secret filename and Stego Image file name
	* Output: FILE pointers for above files
	* Return Value: success/failure
	*/
 
	//Info for user
	printf("INFO: Opening required files\n");
	
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    
	//Do Error handling
    if (NULL == encInfo->fptr_src_image)
    {
		//Error
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
		
		//Return failure enum type
    	return e_failure;
    }
	
	//Info for user
	printf("INFO: Opened %s successfully\n",encInfo->src_image_fname);
	
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
	
    //Do Error handling
    if (NULL == encInfo->fptr_secret)
    {
		//Error
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
		
		//Return failure enum type
    	return e_failure;
    }

	//Info for user
	printf("INFO: Opened %s successfully\n",encInfo->secret_fname);
	
    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    
	//Do Error handling
    if (NULL == encInfo->fptr_stego_image)
    {
		//Error
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
		
		//Return failure enum type
    	return e_failure;
    }

	//Info for user
	printf("INFO: Opened %s successfully\n",encInfo->stego_image_fname);
	printf("INFO: Done, files opened\n");
	
    //Return success enum type
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{	
	/*Description: Ckeck if secret file can be encoded in bmp file
	* Input: struct encodeinfo
	* Return Value: success/failure
	*/
	
	//Info for user
	printf("INFO: Checking the size of \"%s\"\n",encInfo->secret_fname);
	
	//Getting secret file size
	if( encInfo->secret_file_size = get_file_size( encInfo->fptr_secret ) )
	{
		//Info for user
		printf("INFO: Done, Not Empty\n");
		printf("INFO: Checking for availabe bytes to encode in \"%s\"\n",encInfo->src_image_fname);
		
		//If size is valid
		if( get_image_size_for_bmp(encInfo) > ((encInfo->secret_file_size) * 8) + 166 )
		{
			//Info for user
			printf("INFO: Done, Avaliable\n");
			
			//Retun success enum type
			return e_success;
		}
	}
	//Retun failure enum type
	return e_failure;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
	/*Description: Copy bmp header 54 bytes
	* Input: source file pointer and stego file pointer
	* Output: Header data of source file copied to stego file
	* Return Value: success/failure
	*/
	
	//Temp buffer	
	char character[54];
	
	//Setting pointer to begining of file
	fseek(fptr_src_image, 0L, SEEK_SET);
	fseek(fptr_stego_image, 0L, SEEK_SET);
	
	//Copying header 
	if( 54 == fread( character, sizeof(char), 54, fptr_src_image) )
	{
		if( 54 == fwrite( character, sizeof(char), 54, fptr_stego_image ) )
		{
			//Info for user
			printf("INFO: Copying Image Header\n");
			printf("INFO: Done\n");
			
			//Return success enum type
			return e_success;
		}
		else
		{
			//Error message
			printf("ERROR: Unable to write header data to otuput file\n");
			
			//Return failure enum type
			return e_failure;
		}
	}
	else
	{
		//Error message
		printf("ERROR: Unable to read header data\n");
		
		//Return failure enum type
		return e_failure;
	}
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
	/*Description: Encode magic string to stego image
	* Input: magic_string and structure
	* Output: Encodes magic string to destination file
	* Return Value: success/failure
	*/
	
	//Check for valid magic string
	if( '\0' == *magic_string )
	{
		//Error
		printf("ERROR: Invalid magic string signature\n");
	
		//Return failure enum type
		return e_failure;
	}
	
	//Copying file pointer to local pointer
	FILE *fptr_src_image = encInfo->fptr_src_image;
	FILE *fptr_stego_image = encInfo->fptr_stego_image;
	
	//Start from 54th byte of file
	fseek( fptr_src_image, 54L, SEEK_SET);
	fseek( fptr_stego_image, 54L, SEEK_SET);
	
	//Print info
	printf("INFO: Encoding Magic String Signature\n");
	
	//Encoding magic string
	if( e_success == encode_data( magic_string, sizeof(char), fptr_src_image, fptr_stego_image ) )
	{
		//Info for user
		printf("INFO: Done\n");
	
		//Return success enum type
		return e_success;
	}	
	
	//Error
	printf("ERROR: Unable to encode Magic String Signature\n");
	
	//Return failure enum type
	return e_failure;
}

Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
	/*Description: Encode secret file extension size to stego image
	* Input: secret file extension and structure
	* Output: Encodes secret file extension to destination file
	* Return Value: success/failure
	*/
	
	//Copying file pointer to local pointer
	FILE *fptr_src_image = encInfo->fptr_src_image;
	FILE *fptr_stego_image = encInfo->fptr_stego_image;
	
	//Seek to 56th byte of file
	fseek( fptr_src_image, 70L, SEEK_SET);
	fseek( fptr_stego_image, 70L, SEEK_SET);
	
	//Info for user
	printf("INFO: Encoding secret file extension size\n");
	
	//Encoding file size
	if( e_success == encode_data( (char *)&extn_size, 4, fptr_src_image, fptr_stego_image) )
	{
		//Info for user
		printf("INFO: Done\n");
		
		//Return success enum type	
		return e_success;
	}
	
	//Error
	printf("ERROR: Unable to encode secret file extension size\n");
	
	//Return failure enum type
	return e_failure;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	/*Description: Encode secret file extension to stego image
	* Input: File extension and structure
	* Output: Encodes file extension to destination file
	* Return Value: success/failure
	*/
	
	//Copying file pointer to local pointer
	FILE *fptr_src_image = encInfo->fptr_src_image;
	FILE *fptr_stego_image = encInfo->fptr_stego_image;

	//Seek to 60th byte of file
	fseek( fptr_src_image, 102L, SEEK_SET);
	fseek( fptr_stego_image, 102L, SEEK_SET);
	
	//Info for user
	printf("INFO: Encoding secret file extension\n");
	
	//Encoding file extension
	if( encode_data(file_extn, sizeof(char), fptr_src_image, fptr_stego_image) )
	{
		//Info for user
		printf("INFO: Done\n");
		
		//Return success enum type	
		return e_success;
	}
	
	//Error
	printf("ERROR: Unable to encode secret file extension\n");
	
	//Return failure enum type
	return e_failure;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
	/*Description: Encode secret file size to stego image
	* Input: Secret file size and structure
	* Output: Encodes secret file size to destination file
	* Return Value: success/failure
	*/
	
	//Copying file pointer to local pointer
	FILE *fptr_src_image = encInfo->fptr_src_image;
	FILE *fptr_stego_image = encInfo->fptr_stego_image;
	
	//Seek to 64th byte of file
	fseek( fptr_src_image, 134L, SEEK_SET);
	fseek( fptr_stego_image, 134L, SEEK_SET);
	
	//Info for user
	printf("INFO: Encoding secret file size\n");
	
	//Encoding file size
	if( encode_data((char *)&file_size, 4, fptr_src_image, fptr_stego_image) )
	{
		//Info for user
		printf("INFO: Done\n");
		
		//Return success enum type	
		return e_success;
	}
	
	//Error
	printf("ERROR: Unable to encode secret file size\n");
	
	//Return failure enum type
	return e_failure;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
	/*Description: Encode secret file data to stego image
	* Input: structure EncodeInfo
	* Output: Encodes secret file content to destination file
	* Return Value: success/failure
	*/
	
	//Copying file pointer to local pointer
	FILE *fptr_secret = encInfo->fptr_secret;
	FILE *fptr_src_image = encInfo->fptr_src_image;
	FILE *fptr_stego_image = encInfo->fptr_stego_image;
	
	//Seek to strat of file
	fseek( fptr_secret, 0L, SEEK_SET);
	
	//Seek to 68th byte of file
	fseek( fptr_src_image, 166L, SEEK_SET);
	fseek( fptr_stego_image, 166L, SEEK_SET);
	
	//Temp buffer 
	char *file_data = NULL;
	
	//Allocate memory for file buffer
	file_data = (char *)calloc(encInfo->secret_file_size, sizeof(char));
	
	//Check if memory is allocated
	if( NULL == file_data )
	{
		//Error
		printf("ERROR: Unable to allocate memory for file buffer\n");
		
		//Return failure enum type
		return e_failure;
	}
	
	//Info for user
	printf("INFO: Encoding secret file data\n");
	
	if( (encInfo->secret_file_size) == fread(file_data, sizeof(char), encInfo->secret_file_size, fptr_secret) )
	{		
		//Encode secret file data
		if( encode_data(file_data, sizeof(char), fptr_src_image, fptr_stego_image) )
		{
			//Info for user
			printf("INFO: Done\n");
			
			//Delete memory allocated for file buffer
			free(file_data);
			
			//Return success enum type	
			return e_success;
		}
		else
		{
			//Error
			printf("ERROR: Unable to encode secret file data\n");
			
			//Delete memory allocated for file buffer
			free(file_data);
			
			//Return failure enum type
			return e_failure;
		}
	}

	//Error
	printf("ERROR: Unable to read secret file data\n");
	
	//Delete memory allocated for file buffer
	free(file_data);
	
	//Return failure enum type
	return e_failure;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	/*Description: Copy remaining image data to stego image 
	* Input: source file pointer and destination file pointer
	* Output: copies source file content to destination file
	* Return Value: success/failure
	*/
	
	//Temp byte
	char character;
	
	//Info for user
	printf("INFO: Copying left over data\n");
	
	//Copy byte by byte
	while( character = fgetc(fptr_src), !feof(fptr_src) )
	{
		//Check for error and copy content
		if( EOF == fputc(character, fptr_dest) )
		{
			//Error
			printf("ERROR: Unable to copy left over data\n");

			//Return failure enum type
			return e_failure;
		}
	}
	
	//Close all opened files
	fcloseall();
	
	//Info for user
	printf("INFO: Done\n");
	
	//Return success enum type
	return e_success;
}
/*............................................................................................*/

/*............................................................................................*/
Status encode_data(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	/*Description: Encode the data in buffer passed
	* Inputs: data, size, source file pointer and destination file pointer
	* Output: Encodes and writes data  to destination file
	* Return Value: success/failure
	*/
	
	//Image buffer
	char image_buffer[8*size];
	
	//Validate size
	if( sizeof(char) != size && sizeof(int) != size )
	{
		//Error
		printf("ERROR: Invalid data size\n");
		
		//Return failure enumb type
		return e_failure;
	}
	
	//For each byte
	while( '\0' != (*data)  )
	{
		//Getting size*8 bytes from source image
		if( (8 * size) == fread(image_buffer, sizeof(char), (8 * size), fptr_src_image))
		{
			//Check data size
			if( sizeof(char) == size )
			{
				//Encoding each byte of data
				if( e_success == encode_byte_to_lsb( *data, image_buffer ) )
				{
					//Write encoded bytes to destination file
					if( !(fwrite(image_buffer, sizeof(char), (8 * size), fptr_stego_image)  ) )
					{
						//Error
						printf("ERROR: Unable to write data to destination file\n");
			
						//Return failure enum type
						return e_failure;
					}
				}
			}	
			else
			{
				//Encoding 4 byte of data
				if( e_success == encode_4byte_to_lsb( *(int *)data, image_buffer) )
				{
					//Write encoded bytes to destination file
					if( !(fwrite(image_buffer, sizeof(char), (8 * size), fptr_stego_image)  ) )
					{
						//Error
						printf("ERROR: Unable to write data to destination file\n");
			
						//Return failure enum type
						return e_failure;
					}
					//Breaking for integer data
					break;
				}
			}
		}
		else
		{
			//Error
			printf("ERROR: Unable to read data from source file\n");
			
			//Return failure enum type
			return e_failure;
		}
		//Next byte
		++data;
	}
	
	//Return success enum type
	return e_success;
}

Status encode_byte_to_lsb(char data,  char *image_buffer)
{
	/*Description: Encode one byte to buffer
	* Input: data(char) and image buffer
	* Return Value: success/failure
	*/

	//Encode bit by bit
	for( int bit = 7; bit >= 0; --bit )
	{
		//Checking for bit
		if( (data >> bit) & 1 )
		{
			//Writing 1
			*image_buffer = (*image_buffer) | 1;
		}
		else
		{
			//Writing 0
			*image_buffer = (*image_buffer) & (~1);
		}
		//Next byte
		++image_buffer;
	}
	
	//Return success enum type
	return e_success;
}

Status encode_4byte_to_lsb(int data,  char *image_buffer)
{
	/*Description: Encode 4 bytes to buffer
	* Input: data(int) and image buffer
	* Return Value: success/failure
	*/
	
	//Encode bit by bit
	for( int bit = 31; bit >= 0; --bit )
	{
		//Checking for bit
		if( (data >> bit) & 1 )
		{
			//Writing 1
			*image_buffer = (*image_buffer) | 1;
		}
		else
		{
			//Writing 0
			*image_buffer = (*image_buffer) & (~1);
		}
		//Next byte
		++image_buffer;
	}
	
	//Return success enum type
	return e_success;
}

long get_image_size_for_bmp(EncodeInfo *encInfo)
{
	/*Description: Get original image size
	* Input: Image file ptr
	* Output: width * height * bytes per pixel (3 in case of RGB color image)
	* Return Value: image size
	*/
	
	//Declaration
    uint width, height;
    unsigned short bpp;
    long img_size;

	//Copy to local pointer
    FILE *fptr_src_image = encInfo->fptr_src_image;
    
	//Seek to 18th byte
    fseek(fptr_src_image, 18, SEEK_SET);

    //Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_src_image);  

    //Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_src_image);

    //Get bits per pixel
    fseek(fptr_src_image, 28, SEEK_SET);
    fread(&bpp, sizeof(short), 1, fptr_src_image);

    //Store values in encInfo
    encInfo->width = width;
    encInfo->height = height;
    encInfo->bpp = bpp;

	//Calculate image size
    img_size = width * height * bpp / 8; 
	
	//Return image size
    return img_size;
}

long get_file_size(FILE *fptr)
{	
	/*Description: Get secret file size
	* Inputs: File pointer
	* Retun: File size
	*/
	
	//Seek local pointer to end
	fseek(fptr, 0L, SEEK_END);
	
	//Return the size of file
	return ftell(fptr);
}
/*............................................................................................*/
