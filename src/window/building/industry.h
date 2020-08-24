#ifndef WINDOW_BUILDING_INDUSTRY_H
#define WINDOW_BUILDING_INDUSTRY_H

#include "common.h"

void draw_workshop(building_info_context *c, int help_id, const char *sound_file, int group_id, int resource, int input_resource);

void window_building_draw_wheat_farm(building_info_context *c);
void window_building_draw_vegetable_farm(building_info_context *c);
void window_building_draw_fruit_farm(building_info_context *c);
void window_building_draw_olive_farm(building_info_context *c);
void window_building_draw_vines_farm(building_info_context *c);
void window_building_draw_pig_farm(building_info_context *c);

void window_building_draw_marble_quarry(building_info_context *c);
void window_building_draw_iron_mine(building_info_context *c);
void window_building_draw_timber_yard(building_info_context *c);
void window_building_draw_clay_pit(building_info_context *c);

void window_building_draw_wine_workshop(building_info_context *c);
void window_building_draw_oil_workshop(building_info_context *c);
void window_building_draw_weapons_workshop(building_info_context *c);
void window_building_draw_furniture_workshop(building_info_context *c);
void window_building_draw_pottery_workshop(building_info_context *c);

void window_building_draw_shipyard(building_info_context *c);
void window_building_draw_wharf(building_info_context *c);

#endif // WINDOW_BUILDING_INDUSTRY_H
