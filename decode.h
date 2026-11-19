#ifndef DECODE_H
#define DECODE_H
#include "types.h"
#include <stdio.h>
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4


typedef struct Decode_Info
{   /* Magic STRING LEN*/
    uint magic_string_size;  //4bytes

    /* Source Stego Image info */ //bmp file
    char *src_stego_image_fname;
    FILE *fptr_src_stego_image;
    char image_data[MAX_IMAGE_BUF_SIZE]; // 8bytes buffer
    
    /*Output File info*/
    char * output_fname;
    unsigned long max_file_suffix;
    char *extn_op_file;
    FILE * fptr_op;
    char decoded_data[MAX_SECRET_BUF_SIZE];
    unsigned long output_data_size;
} Decode_Info;

Status read_and_validate_decode_args(char * argv[],Decode_Info *decodeinfo);
Status do_decoding(Decode_Info *decodeinfo);

/* Get File pointers for i/p and o/p files */
Status open_files_d(Decode_Info *);
Status open_output_file(Decode_Info*);
Status decode_magic_string_size(Decode_Info *decodeinfo);
Status decode_magic_string(Decode_Info *decodeinfo);
Status decode_secret_file_extn_size(Decode_Info *decodeinfo);
Status decode_secret_file_extn(Decode_Info *decodeinfo);
Status decode_secret_data_size(Decode_Info *decodeinfo);
Status decode_secret_data(Decode_Info *decodeinfo);
Status LSB_to_byte(unsigned char *,unsigned char *);

#endif