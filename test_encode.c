#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

int main(int argc, char *argv[])
{ 
    uint img_size;
    if(argc<2 || check_operation_type(argv) == e_unsupported)
    {
        if (argc < 4)
        {
            usleep(30000);
            printf("ERROR: Operation type is not mentioned and also less number of arguments\n");
            usleep(30000);
            printf("INFO: For Encoding : ./lsb_steg -e <.bmp file> <.txt file> [output file] \n");
            usleep(30000);
            printf("INFO: For Decoding : ./lsb_steg -d <.bmp file> [output file]\n");
            return -1;
        }
    }
    if (check_operation_type(argv) == e_encode)
    {
        // Do Encoding

        EncodeInfo encInfo;
        if(argc>3)
        {
            usleep(30000);
            printf("<---------Selected Encoding--------->\n");
            if (read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                if (do_encoding(&encInfo) == e_success)                
                {
                    fclose(encInfo.fptr_src_image);
                    fclose(encInfo.fptr_secret);
                    fclose(encInfo.fptr_stego_image);
                    usleep(30000);
                    printf("INFO: ## Encoding Done Successfully ##\n");
                }
                else
                {
                    printf("INFO: Error in Encoding\n");
                }
            } 
            else
            {
                printf("INFO: Validation is Failed\n");
                return e_failure;
            }
        } 
        else
        {
            usleep(30000);
            printf("ERROR: Less number of arguments\n");
            usleep(30000);
            printf("INFO: For Encoding : ./lsb_steg -e <.bmp file> <.txt file> [output file] \n");
            return -1;
        }  
    }
    if(check_operation_type(argv) == e_decode)
    {
        // Do Decoding

        DecodeInfo dcInfo;
        if(argc>2)
        {
            if(read_and_validate_decode_args(argv,&dcInfo)==e_success)
            {
                usleep(30000);
                printf("<--------Selected Decoding--------->\n"); 
                if(do_decoding(&dcInfo)==e_success)
                {
                    fclose(dcInfo.fptr_steged_image);
                    fclose(dcInfo.fptr_output_file);
                    usleep(30000);
                    printf("INFO: ## Decoding Done Successfully ##\n");

                }
                 else
                {
                    usleep(30000);
                    printf("INFO: Error in Decoding\n");
                }
            }
             else
            {
                usleep(30000);
                printf("INFO: Validation is Failed\n");
                return e_failure;
            }

        }      
        else
        {
            usleep(30000);
            printf("ERROR: Less number of arguments\n");
            usleep(30000);
            printf("INFO: For Decoding : ./lsb_steg -d <.bmp file> [output file]\n");
            return -1;
        } 
    }
     return 0;
}    
