#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "image_utils.c"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define SV_Fmt "%.*s"
#define SV_arg(sv) (int) (sv).len, (sv).buf

#define ARR_LEN(ar) sizeof((ar))/sizeof((ar[0]));

typedef enum ArgErr {
    no_error = 0,
    first_arg_missing = -1,
    format_not_supported = -2,
    memory_failures = -3,
} ArgErr;

typedef struct SV {
    size_t len;
    const char *buf;
} SV;

SV SV_from_cstr(const char *str) {
    //SV ret;
    return (SV) { strlen(str), str};
}

void partition_cstr(const char* src,const char pattern,SV *first,SV *second) {
   size_t i = 0;

   while(src[i] != pattern && src[i] != '\0'){
       i++;
   }
   first->len = i;
   first->buf = src;
   second->len = strlen(src) - i - 1;
   second->buf = src + i + 1;
}

bool SVcompare(SV self,SV other) {
    int ret = 0;
    if (self.len == other.len) {
        ret = memcmp(self.buf,other.buf,self.len);
        return ret == 0;
    }
    return false;
} 

bool validate_extension(const char* src,SV *found) {
    const SV supported_ext[] = { SV_from_cstr("jpg"),
      SV_from_cstr("bmp"),
      SV_from_cstr("jpeg"),
      SV_from_cstr("png"),
      SV_from_cstr("gif") 
    };
    SV name,ext;
    partition_cstr(src,'.',&name,&ext);
    bool valid = false;
    size_t n = ARR_LEN(supported_ext);
    for(size_t i = 0;i < n ;i++){
        if(SVcompare(ext,supported_ext[i])) {
             valid = true;
             break;
        }
    } 
    printf("ext : "SV_Fmt"\n",SV_arg(ext));
    *found = ext;
    return valid;
}

ArgErr read_args(int argc,char* argv[],SV *args,SV *ext_found){
   if(argc < 2) return first_arg_missing;
   char* src_name = argv[1];
   if (validate_extension(src_name,ext_found)) {
        args[0] = SV_from_cstr(src_name);
   } else {
       return format_not_supported;
   }
   return no_error;
}

int process_and_save(my_image *src) {

    my_image *dst = new_image();
    int res = makebw(src,dst);
    //(void) res;
    if (res != 0) {
        printf("report failure \n");
    }
    free_image(src);
    free_image(dst);
    return 0;
}



int main(int argc,char* argv[]) {
    SV args[2]; 
    SV ext;
    ArgErr res = read_args(argc,argv,args,&ext);
    int result = EXIT_SUCCESS;
    switch(res){
        case(no_error): 
            {
                my_image *rgb_image = new_image();
                char *path = argv[1];
                if(load_image_from_path(path,rgb_image) != 0) {
                    result = EXIT_FAILURE;
                } else {
                    int nres = process_and_save(rgb_image); 
                    (void) nres;
                }//free_image(rgb_image);
                break;
            }
        case(first_arg_missing) :
            printf("filename is missing\n");
            result = EXIT_FAILURE;
            break;
        case(format_not_supported) :
            printf("format "SV_Fmt" not supported\n",SV_arg(ext));
            result = EXIT_FAILURE;
            break;
       default:
            result = EXIT_FAILURE;
            assert(false && "unreachable");
    }
    return result;
}
