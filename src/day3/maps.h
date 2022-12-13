#ifndef __MAPS_H__
#define __MAPS_H__

#include <stdlib.h>

typedef struct map map_t;

map_t *new_map(size_t key_size, size_t val_silze,
               int (*comp)(const void *, const void *), void *(*factory)(void));
void delete_map(map_t *map);

size_t map_size(map_t *map);

void map_add(map_t *map, const void *key, const void *val);
void map_remove(map_t *map, const void *key);

int map_has_key(map_t *map, const void *key);
void *map_get(map_t *map, const void *key);

#endif
