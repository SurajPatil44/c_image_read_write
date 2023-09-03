#include<errno.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define ARRLEN(s) ((sizeof(s)) / (sizeof(s[0])))

typedef struct my_image {
    int width;
    int height;
    int channel;
    uint8_t *buf;
} my_image;

my_image* new_image(void){
    my_image *res = (my_image*) malloc(sizeof(uint8_t) * sizeof(my_image));
    return res;
}

int load_image_from_path(const char* path,my_image *image) {
    //Image image;
    int width,height,n;
    uint8_t *data = stbi_load(path, &width,&height,&n, 0);
    if (data == NULL) {
        errno = EINVAL;
        return EINVAL;
    }
    image->width = width;
    image->height = height;
    image->channel = n;
    image->buf = data;

    return 0;
}

void transform_3_to_1(const uint8_t *buf,uint8_t* result,float *multi,int msz,int res_sz,int sz){
    //*result = 0;
    float v = 0;
    (void) sz;
    (void) res_sz;
    for(int i = 0;i < msz;i++){
        v +=  (float) buf[i] * multi[i];
    }
    *result = (uint8_t) v;
 }

void transform_3_to_3(const uint8_t *buf,uint8_t* result,float *multi,int msz,int res_sz,int sz){
    (void) sz;
    (void) res_sz;
    for(int i = 0;i < msz;i++){
        result[i] = (uint8_t) ((float) buf[i] * multi[i]);
    }
}

int save_image_to_jpg(const char* filename,const my_image* im){
    return stbi_write_jpg(filename,im->width,im->height,im->channel,im->buf,95); 
}


int makebw(const my_image *src,my_image *dst,int channel,void (*f)(const uint8_t*,uint8_t*,float*,int,int,int)) {
    dst->width = src->width;
    dst->height = src->height;
    dst->channel = channel;
    dst->buf = (uint8_t *) malloc(dst->width * dst->height * dst->channel);
    if(dst->buf == NULL){
        assert(false && "failed memory alloc");
        return 1;
    }
    float mplier[] = {0.299,.587,.114};
    for(int i = 0;i < src->width * src->height * src->channel; i += src->channel){
         f(src->buf + i,dst->buf + i,mplier,ARRLEN(mplier),dst->channel,src->channel);
    }
    int r = save_image_to_jpg("src.jpg",dst);
    printf("%d \n",r);
    return 0;
}

void free_image(my_image *image) {
    if(image->buf != NULL) {
        stbi_image_free(image->buf);
    }
    free(image);
}
