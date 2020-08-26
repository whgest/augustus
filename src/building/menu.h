#ifndef BUILDING_MENU_H
#define BUILDING_MENU_H

#include "building/type.h"

typedef enum {
    BUILD_MENU_VACANT_HOUSE = 0,
    BUILD_MENU_CLEAR_LAND = 1,
    BUILD_MENU_ROAD = 2,
    BUILD_MENU_WATER = 3,
    BUILD_MENU_HEALTH = 4,
    BUILD_MENU_TEMPLES = 5,
    BUILD_MENU_EDUCATION = 6,
    BUILD_MENU_ENTERTAINMENT = 7,
    BUILD_MENU_ADMINISTRATION = 8,
    BUILD_MENU_ENGINEERING = 9,
    BUILD_MENU_SECURITY = 10,
    BUILD_MENU_INDUSTRY = 11,
    BUILD_MENU_FARMS = 12,
    BUILD_MENU_RAW_MATERIALS = 13,
    BUILD_MENU_WORKSHOPS = 14,
    BUILD_MENU_SMALL_TEMPLES = 15,
    BUILD_MENU_LARGE_TEMPLES = 16,
    BUILD_MENU_FORTS = 17,
    BUILD_MENU_AESTHETICS = 18,
    BUILD_MENU_MAX = 19
} build_menu_group;

void building_menu_enable_all(void);

void building_menu_update(void);

int building_menu_count_items(int submenu);

int building_menu_next_index(int submenu, int current_index);

building_type building_menu_type(int submenu, int item);

/**
 * Checks whether the building menu has changed.
 * Also marks the change as 'seen'.
 * @return True if the building menu has changed
 */
int building_menu_has_changed(void);

#endif // BUILDING_MENU_H
