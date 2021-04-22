README file for LSB Image Steganography Project
===============================================

Implementation Guidelines:-

Step 1: Implement Encoding Part
Step 2: Implement Decoding Part
step 3: Implement Args Validation Part
Step 4: Implement main function

Implementation details:-

Step 1: Implement Encoding Part
-------------------------------
Files:
------
encode.h: Header file containing EncodeInfo structure and encode function prototypes.
encode.c: Source file containing encode function definitions.

Validation:
You need to validate whether encoding is successful by:
1. stego.bmp should be same as beautiful.bmp
2. Size of stego.bmp and beautiful.bmp should be same (ls -l)
3. Use tools like hexdump to dump source and stego image pixels in hex format. Then compare the binary values.

Step 2: Implement Decoding Part
-------------------------------
Files:
------
decode.h: Header file containing DecodeInfo structure and decode function prototypes.
decode.c: Source file containing decode function definitions.

Validation:
You need to validate whether decoding is successful by comparing password.txt with original file secret.txt

Step 3: Implement Validation Part
---------------------------------
Files:
------
args.h: Header file containing function prototype for argument validation.
args.c: Source file containing validation function defination.

Validation:
You need to pass -h or -e or -d for operation type and different type of arguments.

Step 4: Implement main function
-------------------------------
Files:
------
main.c: Contain main function. Should parse the cmdline arguments and invoke encode or decode function.

Compile:
--------
gcc encode.c decode.c args.c main.c -o steg

Run:
----
1. Encode secret.txt into beautiful.bmp and generate stego.bmp
./steg -e beautiful.bmp secret.txt stego.bmp

This should encode secret.txt into beautiful.bmp and generate output file stego.bmp

2. Decode secret from stego.bmp and save as password.txt
./steg -d stego.bmp password.txt

This should decode password.txt from stego.bmp

3. Get help on how to run the program
./steg -h

This should display usage help about the program
