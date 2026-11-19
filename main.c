#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

#ifdef DEBUG
#undef DEBUG 
#endif




int main(int argc,char * argv[])
{
    if(argc>2){
        if(check_operation_type(argv[1])==0){
            EncodeInfo encInfo;
        #ifdef DEBUG
printf("Going to enter read and validation function from main func\n");
#endif
            read_and_validate_encode_args(argv,&encInfo);
            do_encoding(&encInfo);
        }
        else if(check_operation_type(argv[1])==1){
            Decode_Info decInfo;

            read_and_validate_decode_args(argv,&decInfo);
            do_decoding(&decInfo);
        }
        else{
            printf("Error : Invlaid operation\n");
            return 1;
        }
        
    }
    else{
        printf("Error : Less Number of arguments passed\n");
        return 0;    
    }


    

    //ret = check operation tyoe
    /*
    ret => e_encode
    {
    EncodeInfo encInfo;
    ret => read_and_validate_encode_argv
}*/

    return 0;
}














    // uint img_size;

    // // Fill with sample filenames
    // encInfo.src_image_fname = "beautiful.bmp";
    // encInfo.secret_fname = "secret.txt";
    // encInfo.stego_image_fname = "stego_img.bmp";

    // // Test open_files
    // if (open_files(&encInfo) == e_failure)
    // {
    // 	printf("ERROR: %s function failed\n", "open_files" );
    // 	return 1;
    // }
    // else
    // {
    // 	printf("SUCCESS: %s function completed\n", "open_files" );
    // }

    // // Test get_image_size_for_bmp
    // img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    // printf("INFO: Image size = %u\n", img_size);
