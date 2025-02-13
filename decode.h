#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Steged Image info */
    char *steged_image_fname;
    FILE *fptr_steged_image;

    /* Output File Info */
    char output_file_fname[20];
    FILE *fptr_output_file;
    char magic_string[3];
    unsigned int secret_file_ext_size;
    char secret_file_ext[5];
    unsigned int secret_file_size;


} DecodeInfo;

Status read_and_validate_decode_args(char *argv[], DecodeInfo *dcInfo);
Status open_decode_files(DecodeInfo *dcInfo);
Status decode_magic_string(DecodeInfo *dcInfo);
Status decode_secret_extn_size(DecodeInfo *dcInfo);
Status decode_secret_extn(DecodeInfo *dcInfo);
Status decode_secret_file_size(DecodeInfo *dcInfo);
Status decode_secret_file_data(DecodeInfo *decode);
Status image_to_data(int size,DecodeInfo *dcInfo,char * decoded_data);
Status lsb_to_byte(char *image_buffer , DecodeInfo *dcInfo);
Status lsb_to_size(char *image_buffer,DecodeInfo *dcInfo);
Status do_decoding(DecodeInfo *dcInfo);

#endif