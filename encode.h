#ifndef ENCODE_H
#define ENCODE_H

//Contains user defined types
#include "types.h" 
#include "common.h"

/*Structure to store information required to
 *encode secret file to source image.
 */


typedef struct _EncodeInfo
{
    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char secret_file_extn[MAX_SECRET_FILE_EXTN + 1];
    long secret_file_size;

    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint width, height;
    uint bpp; //bits per pixel

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} EncodeInfo;


/* Encode function prototype */

/*............................................................................................*/

/* Perform the encoding */ //ready, tested
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */ //ready tested
Status open_encode_files(EncodeInfo *encInfo);

/* check capacity */ //ready tested
Status check_capacity(EncodeInfo *encInfo);

/* Copy bmp image header */ //ready
Status copy_bmp_header(FILE * fptr_src_image, FILE *fptr_stego_image);

/* Store Magic String */ //ready
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extension size */ //ready
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo);

/* Encode secret file extenstion */ //ready to test
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */ //ready to test 
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/ //ready to test 
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Copy remaining image bytes from src to stego image after encoding *///readt to test
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/*............................................................................................*/

/*............................................................................................*/

/* Encode function, which does the real encoding */ // ready to test
Status encode_data(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */ //ready to test
Status encode_byte_to_lsb(char data,  char *image_buffer);

/* Encode 4 bytes into LSB of image data array */ //ready to test
Status encode_4byte_to_lsb(int data,  char *image_buffer);

/* Get image size */ //ready
long get_image_size_for_bmp(EncodeInfo *encInfo);

/* Get file size */ //ready to test
long get_file_size(FILE *fptr);

/*............................................................................................*/

#endif
