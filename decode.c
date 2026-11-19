#include <stdio.h>
#include<stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include<error.h>
#include<string.h>

Status read_and_validate_decode_args(char * argv[],Decode_Info *decodeinfo){
    if((strcmp(strchr(argv[2],'.'),".bmp"))){
        printf("\033[31mError: Unsupported file type. Expected .bmp extension.\033[0m\n");
        
        return e_failure;
    }
    
    decodeinfo->src_stego_image_fname=argv[2];
    printf("\033[36m[INFO] ==> Source file extension validated.\033[0m\n");

    if(!argv[3]){
        printf("\033[36m[INFO] ==> No output filename specified. Using default: output.txt\033[0m\n");
        if(!(decodeinfo->output_fname=(char *)malloc(sizeof("output")))){
            printf("\033[31m[ERROR] Memory allocation failed for output filename.\033[0m\n");
            return e_failure;
        }
        
        strcpy(decodeinfo->output_fname,"output");
        // printf("%s\n",decodeinfo->output_fname);
        return e_success;
    }
    if(!(decodeinfo->output_fname=(char*)malloc(strlen(strtok(argv[3],"."))))){
        printf("\033[31m[ERROR]\033[0m Memory allocation failed for output filename.\n");
        return e_failure;
    }
    strcpy(decodeinfo->output_fname,argv[3]);
    // printf("%s\n",decodeinfo->output_fname);
    return e_success;
}

Status do_decoding(Decode_Info * decodeinfo){
    if(open_files_d(decodeinfo)){
        printf("\033[31mError while performing operation of open_files func...\n");
        return e_failure;
    }
    printf("\033[36m[INFO] ==> File handles initialized for read/write operations\033[0m\n");
    fseek(decodeinfo->fptr_src_stego_image,54,SEEK_SET);
    
    if(decode_magic_string_size(decodeinfo)){
        printf("\033[31mError : decode magic string size\n");
        return e_failure;
    }
    printf("\033[36m[INFO] ==> Extracted magic string length: %d\033[0m\n",decodeinfo->magic_string_size);

        if(decode_magic_string(decodeinfo)){
        printf("\033[31mError : fault at function decode magic string\n");
        return e_failure;
    }
    printf("\033[36m[INFO] ==> Recovered magic string: \"%s\"\033[0m\n",MAGIC_STRING);

        if(decode_secret_file_extn_size(decodeinfo)){
        printf("\033[31mError : fault at function decode secret file extension size\033[0m\n");
        return e_failure;
    }
    printf("\033[36m[INFO] ==> Extracted file extension length: %lu\n",decodeinfo->max_file_suffix);

        if(decode_secret_file_extn(decodeinfo)){
        printf("\033[31mError : fault at function decode secret file extension\n");
        return e_failure;
    }
    printf("\033[36m[INFO] ==> Recovered file extension: \"%s\"\n",decodeinfo->extn_op_file);

    if(open_output_file(decodeinfo)){
        printf("\033[31mError : fault at opening the output file in write mode\n");
    }

    printf("\033[36m[INFO] ==> Ready to write recovered data to %s\033[0m\n",decodeinfo->output_fname);

    if(decode_secret_data_size(decodeinfo)){
        printf("\033[31mError : fault at function decode secret data size\n");
        return e_failure;
    }
    printf("\033[36m[INFO] ==> Extracted payload size: %lu bytes\033[0m\n",decodeinfo->output_data_size);

    if(decode_secret_data(decodeinfo)){
        printf("\033[31mError : fault at function decode secret data\n");
        return e_failure;
    }
    printf("\n\n\n\033[32m[SUCCESS]---Secret data extraction complete---\033[0m\n\n\033[36m[INFO] ==> Recovered %lu bytes of hidden payload\n\n[INFO] ==> File saved as %s\033[0m\n\n\033[32m[SUCCESS] Decryption/Extraction finished\n\n\033[0m",decodeinfo->output_data_size,decodeinfo->output_fname);

return e_success;

}

Status open_files_d(Decode_Info *decodeinfo){
    if(!(decodeinfo->fptr_src_stego_image=fopen(decodeinfo->src_stego_image_fname,"r"))){
        fprintf(stderr, "ERROR:fault occurred while opening the stego source file in read mode\n");
        return e_failure;
    }
    printf("\033[36m[INFO] ==> Loaded source stego image: [%s]\033[0m\n",decodeinfo->src_stego_image_fname);

return e_success;
//    if(!(decodeinfo->fptr_op=fopen()))
}
Status decode_magic_string_size(Decode_Info *decodeinfo){
    decodeinfo->magic_string_size=0;
    for(int i=0;i<4;i++){
        fread(decodeinfo->image_data,1,8,decodeinfo->fptr_src_stego_image);
        //printf("position - %ld\n",ftell(decodeinfo->fptr_src_stego_image));
        //for(int j=0;j<8;j++){
         //   printf("%x ",decodeinfo->image_data[j]);
       // }
       // printf("\n");
        LSB_to_byte(decodeinfo->decoded_data,decodeinfo->image_data);


        unsigned int buffer = (unsigned int)*(decodeinfo->decoded_data);
       // printf("decoded data %x\n",decodeinfo->decoded_data[0]);
     //   printf("after shifting - %d\n",buffer<<(i*8));
       // Example: Reconstructing a 32-bit integer from 4 bytes
decodeinfo->magic_string_size |= (buffer << (i * 8));
}

 return e_success;   
}
Status decode_magic_string(Decode_Info *decodeinfo){
    for(int i=0;i<decodeinfo->magic_string_size;i++){
        fread(decodeinfo->image_data,1,8,decodeinfo->fptr_src_stego_image);
        LSB_to_byte(decodeinfo->decoded_data,decodeinfo->image_data);
        if(!(MAGIC_STRING[i]==*(decodeinfo->decoded_data))){
            
              fprintf(stderr, "Invlaid secret mesage\n");
            return e_failure;
        }
    }
    return e_success;
}
Status decode_secret_file_extn_size(Decode_Info *decodeinfo){
    decodeinfo->max_file_suffix=0;
    unsigned int buffer;
    for(int i=0;i<4;i++){
        fread(decodeinfo->image_data,1,8,decodeinfo->fptr_src_stego_image);
        LSB_to_byte(decodeinfo->decoded_data,decodeinfo->image_data);
        buffer=decodeinfo->decoded_data[0];
        
        decodeinfo->max_file_suffix|=buffer<<(i*8);
    }
    //printf("size of suffix - %ld\n",decodeinfo->max_file_suffix);
    return e_success;
}
Status decode_secret_file_extn(Decode_Info *decodeinfo){
    decodeinfo->extn_op_file=(char*)malloc((decodeinfo->max_file_suffix)+1);
    int i;
    for(i=0;i<decodeinfo->max_file_suffix;i++){
        fread(decodeinfo->image_data,1,8,decodeinfo->fptr_src_stego_image);
        LSB_to_byte(decodeinfo->decoded_data,decodeinfo->image_data);
        decodeinfo->extn_op_file[i]=*(decodeinfo->decoded_data);
    }
    decodeinfo->extn_op_file[i]='\0';
  //  printf("extension is %s\n",decodeinfo->extn_op_file);
    return e_success;
}
Status open_output_file(Decode_Info * decodeinfo)
{   
    int str_len=strlen(decodeinfo->output_fname);
    // printf("suffix %d\n",decodeinfo->max_file_suffix);
    // printf("strlen %d\n",str_len);
    decodeinfo->output_fname=(char*)realloc(decodeinfo->output_fname,(decodeinfo->max_file_suffix+str_len+1));
    strcat(decodeinfo->output_fname,decodeinfo->extn_op_file);
    // printf("%s\n",decodeinfo->output_fname);
    if(!(decodeinfo->fptr_op=fopen(decodeinfo->output_fname,"w"))){

        fprintf(stderr, "Error : unable to open the output in write mode\n");
        return e_failure;
    }
    return e_success;

}
Status decode_secret_data_size(Decode_Info *decodeinfo){
    unsigned buffer=0;
    decodeinfo->output_data_size=0;
    for(int i=0;i<4;i++){
        fread(decodeinfo->image_data,1,8,decodeinfo->fptr_src_stego_image);
        LSB_to_byte(decodeinfo->decoded_data,decodeinfo->image_data);
        buffer=decodeinfo->decoded_data[0];
        decodeinfo->output_data_size|=buffer<<(i*8);
    }
   // printf("%ld\n",decodeinfo->output_data_size);
    return e_success;
}
Status decode_secret_data(Decode_Info *decodeinfo){
    for(int i=0;i<decodeinfo->output_data_size;i++){
        fread(decodeinfo->image_data,1,8,decodeinfo->fptr_src_stego_image);
        LSB_to_byte(decodeinfo->decoded_data,decodeinfo->image_data);
        fputc(decodeinfo->decoded_data[0],decodeinfo->fptr_op);
    }
    return e_success;
}

Status LSB_to_byte(unsigned char *data,unsigned char *image_buffer){
*data=0;
for(int i=0;i<8;i++){
    *data |= (image_buffer[i]&1)<<i;
   // printf("data = %d\n",data);
}

}