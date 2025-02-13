#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *dcInfo)
{
     if(strstr(argv[2],".bmp")!=NULL && strcmp(strstr(argv[2],".bmp"),".bmp")==0)
    {
        dcInfo->steged_image_fname=argv[2];
    }
    else
    {
        return e_failure;
    }

    if((argv[3])!=NULL )
    {
        char *file_name = strtok(argv[3],".");
        strcpy(dcInfo->output_file_fname, file_name);
    }
    else
    {
    strcpy(dcInfo->output_file_fname,"decoded");
    }

    return e_success;
}


Status open_decode_files(DecodeInfo *dcInfo)
{
    dcInfo->fptr_steged_image = fopen(dcInfo->steged_image_fname,"r");
    if (dcInfo->fptr_steged_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", dcInfo->steged_image_fname);

    	return e_failure;
    }

    return e_success;
}


Status do_decoding(DecodeInfo *dcInfo)
{
    usleep(30000);
    printf("INFO: ## Decoding Procedure Started ##\n");
    usleep(30000);
    printf("INFO: Opening required Files\n");
    if(open_decode_files(dcInfo)==e_success)
    {
        usleep(30000);
        printf("INFO: Opened %s\n",dcInfo->steged_image_fname);
        usleep(30000);
        printf("INFO: Decoding Magic String Signature\n");
        if( decode_magic_string(dcInfo)==e_success)
        {
            usleep(30000);
            printf("INFO: Done\n");
            usleep(30000);
            printf("INFO: Decoding Output File Extension size\n");
            if(decode_secret_extn_size(dcInfo)==e_success);
            {
                usleep(30000);
                printf("INFO: Done\n");
                usleep(30000);
                printf("INFO: Decoding Output File Extension\n");
                if(decode_secret_extn(dcInfo)==e_success);
                {
                    usleep(30000);
                    printf("INFO: Done\n");
                    usleep(30000);
                    if(strcmp(dcInfo->output_file_fname,"decoded.txt")==0)
                    {
                    printf("INFO: Output File not mentioned. Creating decoded.txt as default\n");
                    }
                    printf("INFO: Opened %s\n",dcInfo->output_file_fname);
                    usleep(30000);
                    printf("INFO: Done. Opened all required files\n");
                    usleep(30000);
                    printf("INFO: Decoding %s File Size\n",dcInfo->output_file_fname);
                    if(decode_secret_file_size(dcInfo)==e_success)
                    {
                        usleep(30000);
                        printf("INFO: Done\n");
                        usleep(30000);
                        printf("INFO: Decoding %s File Data\n",dcInfo->output_file_fname);
                        if(decode_secret_file_data(dcInfo)==e_success)
                        {
                            usleep(30000);
                            printf("INFO: Done\n");
                        }
                    }    
                }
            }
        }
    }
  
    return e_success;
}


/* Check the Magic string is same or not */

Status decode_magic_string(DecodeInfo *dcInfo)
{
    char decoded_data[strlen(MAGIC_STRING)+1];
    fseek(dcInfo->fptr_steged_image,54,SEEK_SET);
    if(image_to_data(strlen(MAGIC_STRING),dcInfo,decoded_data) == e_success)
    {
        if(strcmp(decoded_data,MAGIC_STRING)==0)
        {
            return e_success;
        }
        else
        {
            return e_failure;
        }
    }
   
    return e_success;
}


Status decode_secret_extn_size(DecodeInfo *dcInfo)
{
    char buffer[32];
    fread(buffer,32,1,dcInfo->fptr_steged_image);
    dcInfo->secret_file_ext_size=lsb_to_size(buffer,dcInfo);

    return e_success;
}


Status decode_secret_extn(DecodeInfo *dcInfo)
{
    char decoded_data[dcInfo->secret_file_ext_size + 1];
    image_to_data(dcInfo->secret_file_ext_size,dcInfo,decoded_data);
    strcat(dcInfo->output_file_fname,decoded_data);
    dcInfo->fptr_output_file=fopen(dcInfo->output_file_fname,"w");

    return e_success;
}


Status decode_secret_file_size(DecodeInfo *dcInfo)
{
    char buffer[32];
    fread(buffer,32,1,dcInfo->fptr_steged_image);
    dcInfo->secret_file_size=lsb_to_size(buffer,dcInfo);

    return e_success;
}


Status decode_secret_file_data(DecodeInfo *dcInfo)
{
    char decoded_data[dcInfo->secret_file_size+1];
    image_to_data(dcInfo->secret_file_size,dcInfo,decoded_data);
    fwrite(decoded_data,1,dcInfo->secret_file_size,dcInfo->fptr_output_file);

    return e_success;
}
    

Status image_to_data(int size,DecodeInfo *dcInfo,char * decoded_data)
{
    char buffer[8];
    int i = 0;
    for(i=0;i<size;i++)
    {
        fread(buffer,8,1,dcInfo->fptr_steged_image);
        char ch = lsb_to_byte(buffer,dcInfo);
        decoded_data[i]=ch;  
    }
    decoded_data[size]='\0';

    return e_success;
}


Status lsb_to_size(char *image_buffer,DecodeInfo *dcInfo)
{
    char ch=0;
    for(int i=31;i>=0;i--)
    {
        ch = (ch<<1) | (image_buffer[i]&1);
    }

     return ch;       
}


Status lsb_to_byte(char *image_buffer, DecodeInfo *dcInfo)
{
    char ch=0;
    for(int i=7;i>=0;i--)
    {
        ch = (ch<<1) | (image_buffer[i]&1);
    }

    return ch;
}


