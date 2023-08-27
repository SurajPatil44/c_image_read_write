#include<errno.h>
#include<stdint.h>
#include<stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct my_image {
    int width;
    int height;
    int channel;
    unsigned char *buf;
} my_image;

my_image* new_image(void){
    my_image *res = (my_image*) malloc(sizeof(unsigned char) * sizeof(my_image));
    return res;
}

int load_image_from_path(const char* path,my_image *image) {
    //Image image;
    int width,height,n;
    unsigned char *data = stbi_load(path, &width,&height,&n, 0);
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

static unsigned char transform(unsigned char *buf,int sz){
    float res = 0;
    (void) sz;
    for(int i = 0;i < 3;i++){
      //printf("%d ",(int) buf[i]);
      res += (float) buf[i];
    }
    //(void) sz;
    return (unsigned char) (res / 3.0);
 }

int makebw(const my_image *src,my_image *dst) {

    //printf("%d and %d \n",dst->width,dst->height);
    dst->width = src->width;
    dst->height = src->height;
    dst->channel = 1;
    dst->buf = (unsigned char *) malloc(dst->width * dst->height * dst->channel);
    for(int i = 0;i < src->width * src->height * src->channel; i += src->channel){
        dst->buf[i / src->channel] = transform(src->buf + i,src->channel);
    }

    return 0;
}

void free_image(my_image *image) {
    if(image->buf != NULL) {
        stbi_image_free(image->buf);
    }
    free(image);
}
