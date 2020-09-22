#ifndef SCENARIO_INVASION_H
#define SCENARIO_INVASION_H

#include "core/buffer.h"

typedef struct {
    int in_use;
    int handled;
    int invasion_path_id;
    int warning_years;
    int x;
    int y;
    int image_id;
    int empire_object_id;
    int year_notified;
    int month_notified;
    int months_to_go;
    int invasion_id;
} invasion_warning;

#define MAX_INVASION_WARNINGS 101

struct {
    int last_internal_invasion_id;
    invasion_warning warnings[MAX_INVASION_WARNINGS];
} invasion_warning_data;

void scenario_invasion_clear(void);
void scenario_invasion_init(void);

int scenario_invasion_exists_upcoming(void);

void scenario_invasion_foreach_warning(void (*callback)(int x, int y, int image_id));

int scenario_invasion_count(void);

int scenario_invasion_start_from_mars(void);

int scenario_invasion_start_from_caesar(int size);

void scenario_invasion_start_from_cheat(void);

void scenario_invasion_start_from_console(int attack_type, int size, int invasion_point);

void scenario_invasion_process(void);

void scenario_invasion_save_state(buffer *invasion_id, buffer *warnings);

void scenario_invasion_load_state(buffer *invasion_id, buffer *warnings);

int start_invasion(int enemy_type, int amount, int invasion_point, int attack_type, int invasion_id);

#endif // SCENARIO_INVASION_H
