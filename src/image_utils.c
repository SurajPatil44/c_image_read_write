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


int process_image(my_image *image) {

    printf("%d and %d \n",image->width,image->height);
    return 0;
}

void free_image(my_image *image) {
    if(image->buf != NULL) {
        stbi_image_free(image->buf);
    }
    free(image);
}
