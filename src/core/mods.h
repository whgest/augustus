#ifndef CORE_MODS_H
#define CORE_MODS_H

#include "core/image.h"

#define MAX_MODDED_IMAGES 1000

#define XML_BUFFER_SIZE 1024
#define XML_MAX_DEPTH 4
#define XML_MAX_ELEMENTS_PER_DEPTH 4
#define XML_MAX_ATTRIBUTES 9
#define XML_TAG_MAX_LENGTH 24
#define XML_STRING_MAX_LENGTH 32

void mods_init(void);

int mods_get_group_id(const char *mod_author, const char *mod_name);

int mods_get_image_id(int mod_group_id, const char *image_name);

const image *mods_get_image(int image_id);

const color_t *mods_get_image_data(int image_id);

#endif // CORE_MODS_H
