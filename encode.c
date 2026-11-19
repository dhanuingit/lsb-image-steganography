#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<error.h>
#include<string.h>
// #ifdef DEBUG
// #define DEBUG 
// #endif


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
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
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
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
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

OperationType check_operation_type(char *argv){
    if(!(strcmp(argv,"-e"))){
        return e_encode;
    }
    else if(!(strcmp(argv,"-d"))){
        return e_decode;
    }
    else{
        return e_unsupported;
    }
}

int bmp_file_validation(char * argv){
    FILE * fptr=fopen(argv,"r");
    if(!(strcmp((char*)(fptr),"BM"))){
        return 1;
    }
    else{
        return 0;
    }
}
int bmp_file_validation_str(char *argv){

#ifdef DEBUG 
printf("Entered the bmp_file_validation_str function\n");
#endif


    char * chr=strchr(argv,'.');
    if(chr==NULL){
        return 0;
    }
  
    if(!(strcmp(chr,".bmp"))){
   
        return 1;
    }
    else{
        return 0;
    }
}

Status read_and_validate_encode_args(char **argv, EncodeInfo *encInfo){
    if(!(bmp_file_validation_str(argv[2]))){
        printf("Invlaid .bmp file / the contents of the .bmp file is non-standard\n");
        return e_failure;
    }
    else{
        encInfo->src_image_fname=argv[2];
        printf("  :):)  Validation Source .bmp is success  :):)  \n");
    }

    if(fopen(argv[3],"r")==NULL){
        printf("Error : The %s file is not present in your current directory\n",argv[3]);
        return e_failure;
    }
    else{
        encInfo->secret_fname=argv[3];
        encInfo->extn_secret_file=strchr(encInfo->secret_fname,'.');
        encInfo->max_file_suffix=strlen(encInfo->extn_secret_file);
        #ifdef DEBUG
        printf("  :):)  Successfully fetched the %s file  :):)\n",argv[3]);
        printf("%s\n",encInfo->extn_secret_file);
        printf("size of file extension - %lu",encInfo->max_file_suffix);
        #endif
    }

    if(argv[4]!=NULL){
        if(bmp_file_validation_str(argv[4])){

            encInfo->stego_image_fname=argv[4];
        }
        else{
            printf("Warning : Invalid File extension,implicit file name will be choosed\n");
            encInfo->stego_image_fname="stego_image.bmp";
        }
    }
    else{
        encInfo->stego_image_fname="stego_image.bmp";
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo){

if(open_files(encInfo))
{
    printf("open_files function failed\n");
    return e_failure;
}

    printf("open file func as done its job\n");


if(check_capacity(encInfo)){
    printf("required amount of RGB Date is not present in the sorece.bmp file\n");
}

printf("The Required RGB data exists :):) \n");
if(copy_bmp_header(encInfo)){
    printf("Unable to copy the header\n");
    return e_failure;
}

printf("Header copied successfully\n");


#ifdef DEBUG
printf("src = %lx\n",ftell(encInfo->fptr_src_image));
printf("stego = %lx\n",ftell(encInfo->fptr_stego_image));
#endif


if(encode_magic_string_size(encInfo)){
    printf("Error : failure  at encoding the MAGIC STRING size...\n");
    return e_failure;
}
printf("MAGIC STRING size is encoded into the file successfully\n");


#ifdef DEBUG
printf("src = %lx\n",ftell(encInfo->fptr_src_image));
printf("stego = %lx\n",ftell(encInfo->fptr_stego_image));
#endif


if(encode_magic_string(MAGIC_STRING,encInfo)){
printf("Error : failure at the encoding of magic_string");
return e_failure;
}
printf("Enoding of MAGIC STRING is success :) :)\n");

#ifdef DEBUG
printf("src = %lx\n",ftell(encInfo->fptr_src_image));
printf("stego = %lx\n",ftell(encInfo->fptr_stego_image));
#endif


if(encode_secret_file_ext_size(encInfo)){
 printf("Error : failure at the encoding of scret file extension size");
 return e_failure;
}
printf("successfully encoded the Secret file extension size :) :)\n");

#ifdef DEBUG
printf("src = %lx\n",ftell(encInfo->fptr_src_image));
printf("stego = %lx\n",ftell(encInfo->fptr_stego_image));
#endif

if(encode_secret_file_extn(encInfo)){
 printf("Error : failure at the encoding of magic_string");
 return e_failure;
}
printf("successfully encoded the Secret file extension :) :)\n");


#ifdef DEBUG
printf("src = %lx\n",ftell(encInfo->fptr_src_image));
printf("stego = %lx\n",ftell(encInfo->fptr_stego_image));
#endif


if(encode_secret_file_size(encInfo)){
 printf("Error : Failed to encode the secret file's data size\n");
 return e_failure;
}
#ifdef DEBUG
printf("Successfully encoded the the size of the data present in the secret file :) :)\n");
printf("secret file size - %ld\n",encInfo->size_secret_file);
printf("src = %lx\n",ftell(encInfo->fptr_src_image));
printf("stego = %lx\n",ftell(encInfo->fptr_stego_image));
#endif

if(encode_secret_file_data(encInfo)){
 printf("Error : failed to encode the data of secret file");
 return e_failure;
}
printf("Successfully encoded the entire data present in the secret file :) :)\n");


#ifdef DEBUG
printf("src = %lx\n",ftell(encInfo->fptr_src_image));
printf("stego = %lx\n",ftell(encInfo->fptr_stego_image));
#endif


if(encInfo->image_capacity<=encInfo->estimated_size)
{
    return e_success;
}
if(copy_remaining_img_data(encInfo)){
    printf("Error : Failed to copy reamining Source Image data\n");
    return e_failure;
}
printf("Successfully copied remaining data\n");

#ifdef DEBUG
printf("src = %lx\n",ftell(encInfo->fptr_src_image));
printf("stego = %lx\n",ftell(encInfo->fptr_stego_image));
#endif

printf("Congrats you have implemented and encoded a secret message into %s file \ncheck the %s file for your output\n",encInfo->stego_image_fname,encInfo->stego_image_fname);

fclose(encInfo->fptr_src_image);
fclose(encInfo->fptr_stego_image);
fclose(encInfo->fptr_secret);
}

uint get_file_size(FILE *fptr){
    fseek(fptr,0,SEEK_END);
    uint file_size = ftell(fptr);
    rewind(fptr);
    return file_size;
}


Status check_capacity(EncodeInfo *encInfo){
    
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    fseek(encInfo->fptr_secret,0,SEEK_END);
    
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    
    encInfo->magic_string_size=strlen(MAGIC_STRING);

    #ifdef DEBUG
    printf("%ld\n",encInfo->size_secret_file);
    #endif
 
    encInfo->estimated_size = (32) + (strlen(MAGIC_STRING)*8) + (32) + ((strlen(encInfo->extn_secret_file))*8) + (32) + (encInfo->size_secret_file*8);
    if( encInfo->estimated_size<encInfo->image_capacity){
        return e_success;
    }
    else{
        return e_failure;
    }
}



Status copy_bmp_header(EncodeInfo *encInfo){
    rewind(encInfo->fptr_src_image);
    char buffer[9];
    for(int i=0;i<6;i++){
    if(!fread((char*)(buffer),1,9,encInfo->fptr_src_image))
           {
    	perror("fread");
    	fprintf(stderr, "ERROR: Unable to read file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    if(!fwrite((char*)(buffer),1,9,encInfo->fptr_stego_image))        
       {
    	perror("fwrite");
    	fprintf(stderr, "ERROR: Unable to write into file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
}
#ifdef DEBUG
printf("src = %lx\n",ftell(encInfo->fptr_src_image));
printf("stego = %lx\n",ftell(encInfo->fptr_stego_image));
#endif
return e_success;
}




Status encode_magic_string_size(EncodeInfo *encInfo){
    for(int i=0;i<4;i++){
    
    #ifdef DEBUG
    printf("proper value %#x \n",encInfo->magic_string_size);
    printf("ptr position = %lx\n",ftell(encInfo->fptr_src_image));
    #endif

    
    fread(encInfo->image_data,1,8,encInfo->fptr_src_image);


    encode_byte_to_lsb((encInfo->magic_string_size)>>(sizeof(int)*i)&0xFF,encInfo->image_data);

    if(!fwrite(encInfo->image_data,1,8,encInfo->fptr_stego_image))
           {
    	perror("fwrite");
    	fprintf(stderr, "ERROR: Unable to write into file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

}

return e_success;
}

Status encode_magic_string(const char* magic_string,EncodeInfo *encInfo){

for(int i=0;i<encInfo->magic_string_size;i++){

fread(encInfo->image_data,1,8,encInfo->fptr_src_image);

encode_byte_to_lsb(magic_string[i],encInfo->image_data);

fwrite(encInfo->image_data,1,8,encInfo->fptr_stego_image);
}

return e_success;
}

Status encode_byte_to_lsb(char data,unsigned char *image_buffer){
for(int i=0;i<8;i++){
    image_buffer[i]=((data&(1<<i))>>i)|(image_buffer[i]&(~(1)));
}
}


/* Encode secret file size */
Status encode_secret_file_ext_size(EncodeInfo *encInfo){
    for(int i=0;i<4;i++){
        if(!fread(encInfo->image_data,1,8,encInfo->fptr_src_image))
                   {
    	perror("fread");
    	fprintf(stderr, "ERROR: Unable to read file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
        encode_byte_to_lsb((encInfo->max_file_suffix)>>(sizeof(int)*i)&0xFF,encInfo->image_data);

        if(!fwrite(encInfo->image_data,1,8,encInfo->fptr_stego_image))
        {
    	perror("fwrite");
    	fprintf(stderr, "ERROR: Unable to write into file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    }
    return e_success;
}

/* Encode secret file extenstion */
Status encode_secret_file_extn(EncodeInfo *encInfo){
    for(int i=0;i<encInfo->max_file_suffix;i++){
        if(!fread(encInfo->image_data,1,8,encInfo->fptr_src_image))
                   {
    	perror("fread");
    	fprintf(stderr, "ERROR: Unable to read file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    encode_byte_to_lsb(*(encInfo->extn_secret_file+i),encInfo->image_data);
    
    if(!fwrite(encInfo->image_data,1,8,encInfo->fptr_stego_image))
           {
    	perror("fwrite");
    	fprintf(stderr, "ERROR: Unable to write into file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    }

    return e_success;
}

/* Encode secret file size */
Status encode_secret_file_size(EncodeInfo *encInfo){
        for(int i=0;i<4;i++){
                if(!fread(encInfo->image_data,1,8,encInfo->fptr_src_image))
                   {
    	perror("fread");
    	fprintf(stderr, "ERROR: Unable to read file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

        encode_byte_to_lsb(((encInfo->size_secret_file)>>(8*i))&0xFF,encInfo->image_data);
            if(!fwrite(encInfo->image_data,1,8,encInfo->fptr_stego_image))
           {
    	perror("fwrite");
    	fprintf(stderr, "ERROR: Unable to write into file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    }
    return e_success;
}

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo){
    rewind(encInfo->fptr_secret);
        for(int i=0;i<encInfo->size_secret_file;i++){
        
        if(!fread(encInfo->image_data,1,8,encInfo->fptr_src_image))
        {
    	perror("fread");
    	fprintf(stderr, "ERROR: Unable to read file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }   

        if(!fread(encInfo->secret_data,1,1,encInfo->fptr_secret)){
            perror("fread");
            fprintf(stderr,"Error: Failed to read secret data\n");
            return e_failure;
        }

        encode_byte_to_lsb(encInfo->secret_data[0],encInfo->image_data);
           if(!fwrite(encInfo->image_data,1,8,encInfo->fptr_stego_image))
           {
    	perror("fwrite");
    	fprintf(stderr, "ERROR: Unable to write into file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    }
    return e_success;
}

/* Encode function, which does the real encoding */
Status copy_remaining_img_data(EncodeInfo *encInfo){
    int buffer=0;
    while(buffer!=EOF){

        buffer=fgetc(encInfo->fptr_src_image);
        fputc(buffer,encInfo->fptr_stego_image);
    }
    return e_success;
}



// Status copy_remaining_img_data(EncodeInfo *encInfo){
//     int buffer; // Must be int to hold EOF
//     while ((buffer = fgetc(encInfo->fptr_src_image)) != EOF) {
//         fputc(buffer, encInfo->fptr_stego_image);
//     }
//     return e_success;
// }