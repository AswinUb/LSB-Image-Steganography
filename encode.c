#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    // printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    // printf("height = %u\n", height);

    // Return image capacity
    rewind(fptr_image);

    return width * height * 3;
}


OperationType check_operation_type(char *argv[])        
{
    if(strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else 
    {
      return e_unsupported;
    }
}


uint get_file_size(FILE *fptr)
{
    uint size;
    fseek(fptr,0,SEEK_END);
    size=ftell(fptr);
    rewind(fptr);

    return size;
}


/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;

}


Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")!=NULL && strcmp(strstr(argv[2],".bmp"),".bmp")==0)
    {
        encInfo->src_image_fname=argv[2];
    }
    else
    {
        return e_failure;
    }

    if(strstr(argv[3],".txt")!=NULL && strcmp(strstr(argv[3],".txt"),".txt")==0)
    {
        encInfo->secret_fname=argv[3];
        strcpy(encInfo->extn_secret_file,".txt");
    }
    else
    {
        return e_failure;
    }

    if((argv[4])!=NULL)
    {
        if(strstr(argv[4],".bmp")!=NULL && strcmp(strstr(argv[4],".bmp"),".bmp")==0)
        {
            encInfo->stego_image_fname=argv[4];
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
    printf("INFO: Output File not mentioned. Creating steged_img.bmp as default\n");
    encInfo->stego_image_fname="steged_img.bmp"; 
    }

    return e_success;
       
}


Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
        usleep(30000);
        printf("INFO: Opening required files\n");
        usleep(30000);
        printf("INFO: Opened beautiful.bmp\n");
        usleep(30000);
        printf("INFO: Opened secret.txt\n");
        usleep(30000);
        printf("INFO: Opened %s\n",encInfo->stego_image_fname);
        usleep(30000);
        printf("INFO: Done\n");
        usleep(30000);
        printf("INFO: ## Encoding Procedure Started ##\n");
        usleep(30000);
        printf("INFO: Checking for secret.txt size\n");
        encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
        if(encInfo->size_secret_file)
        {   
            usleep(30000);
            printf("INFO: Done. Not Empty\n");
        }
        usleep(30000);
        printf("INFO: Checking for beautiful.bmp capacity to handle secret.txt\n");
        if(check_capacity(encInfo)==e_success)
        {
            usleep(30000);
            printf("INFO: Done. Found OK\n");
        }

    }
    usleep(10000);
    printf("INFO: Error in opening output file\n");
    usleep(30000);
    printf("INFO: Copying Image Header\n");
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image))
    {
      usleep(30000);
      printf("INFO: Done\n");
      usleep(30000);
      printf("INFO: Encoding Magic String Signature\n");
      if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
      {
        usleep(30000);
        printf("INFO: Done\n");
        usleep(30000);
        printf("INFO: Encoding secret.txt File Extension Size\n");
        if(encode_secret_file_extn_size(MAX_FILE_SUFFIX,encInfo)==e_success)
        {
          usleep(30000);
          printf("INFO: Done\n");
        }
        usleep(30000);
        printf("INFO: Encoding secret.txt File Extension\n");
        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
        {
            usleep(30000);
            printf("INFO: Done\n");
            usleep(30000);
            printf("INFO: Encoding secret.txt File Size\n");
            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
            {
                usleep(30000);
                printf("INFO: Done\n");
                usleep(30000);
                printf("INFO: Encoding secret.txt File Data\n");
                if(encode_secret_file_data(encInfo)==e_success)
                {
                    usleep(30000);
                    printf("INFO: Done\n");
                    usleep(30000);
                    printf("INFO: Copying Left Over Data\n");
                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
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


Status check_capacity(EncodeInfo *encInfo)
{
    if(encInfo->image_capacity>(54 + (encInfo->size_secret_file + 14)*8))
    {
        return e_success;
    }
    return e_failure;
}


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);
}


Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image((char*)magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);

    return e_success;
}

Status encode_secret_file_extn_size(long extn_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(extn_size,buffer);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    
    return e_success;

}


Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn,strlen(encInfo->extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image);

    return e_success;
}


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,buffer);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);

    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch[1];
    while(fread(ch,1,1,encInfo->fptr_secret) !=0)
    {
        encode_data_to_image(ch,1,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    }

     return e_success; 
}


Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    for(int i=0;i<size;i++)
    {
        fread(buffer,8,1,fptr_src_image);
        encode_byte_to_lsb(data[i],buffer);
        fwrite(buffer,8,1,fptr_stego_image);
    }

    return e_success;
}


Status encode_size_to_lsb(char data, char *image_buffer)
{
     for(int i=0;i<32;i++)
    {
        int res1=data&1<<i;
        int res2=image_buffer[i]&~1;
        image_buffer[i]=res2|res1>>i;
    }
}


Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        unsigned char res1=data & (1<<i);
        unsigned char res2=image_buffer[i] &(~1);
        image_buffer[i]=res2 | (res1>>i);
    }
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_src)!=0)
    {
        fwrite(&ch,1,1,fptr_dest);
    }

    return e_success;
}









