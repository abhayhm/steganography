#ifndef DECODE_H
#define DECODE_H

//Contains user defined types
#include "types.h" 
#include "common.h"

/* Structure to store information required to
 * decode secret file from stego image.
 */

typedef struct _DecodeInfo
{
    /* Retrieved Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char secret_file_extn[MAX_SECRET_FILE_EXTN + 1];
    long secret_file_size;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    uint width, height;
    uint bpp; //bits per pixel

} DecodeInfo;


/* Encode function prototype */

/*............................................................................................*/

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Decode secret file extension size */ //ready
int decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
int decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/*............................................................................................*/

/*............................................................................................*/

/* Decode function, which does the real encoding */
Status decode_data(char *data, int size, int nbytes, FILE *fptr_stego_image);

/* Decode a byte into LSB of image data array */
Status decode_byte_to_lsb(char *data, char *image_buffer);

/*............................................................................................*/

#endif
