#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

#define MAX_CUSTOM_EVENTS 180
#define MAX_CONDITIONS 9

uint8_t MESSAGE_TEXT_OVERRIDE[512];

typedef enum {
	CONDITION_VALUE_MONTHS_PASSED,
	CONDITION_VALUE_RATING_CULTURE,
	CONDITION_VALUE_RATING_PEACE,
	CONDITION_VALUE_RATING_PROSPERITY,
	CONDITION_VALUE_RATING_FAVOR,
	CONDITION_VALUE_MONEY,
	CONDITION_VALUE_POPULATION,
	CONDITION_VALUE_TRADE_CITY_OPEN,
	CONDITION_VALUE_CITY_SENTIMENT,
	CONDITION_VALUE_PATRICIANS,
	CONDITION_VALUE_PERCENT_IN_TENTS,
	CONDITION_VALUE_PERCENT_PATRICIANS,
	CONDITION_VALUE_PERCENT_UNEMPLOYED,
	CONDITION_VALUE_ARMY_STRENGTH,
	CONDITION_VALUE_MAX_KEY
} condition_key;

typedef void (*EventActivationFunction)();
typedef int (*GetValueFunction)();

typedef enum {
	EVENT_TYPE_DEMAND_CHANGE,
	EVENT_TYPE_PRICE_CHANGE,
	EVENT_TYPE_REQUEST,
	EVENT_TYPE_INVASION,
	EVENT_TYPE_UPRISING,
	EVENT_TYPE_DISTANT_BATTLE,
	EVENT_TYPE_WAGE_CHANGE,
	EVENT_TYPE_CITY_NOW_TRADES,
	EVENT_TYPE_MAX_KEY
} event_key;

typedef struct {
	condition_key key;
	uint8_t value_type_string[24];
	GetValueFunction actual_value_function;
} condition_value;

typedef struct {
	uint8_t value[24];
	int requirement;
} event_condition;

typedef struct {
	event_key key;
	uint8_t event_type_string[24];
	EventActivationFunction activation_function;
} custom_event_type;

typedef struct {
	uint8_t text[512];
	int resource_id;
	int amount;
	int deadline_months;
	int favor_gained;
	uint8_t city_name[24];
	uint8_t type[24];
	int months_warning;
	int entrypoint_id;
} custom_event_data;

typedef struct {
	custom_event_data event_data;
	event_condition conditions[MAX_CONDITIONS];
	int in_use;
	int fired;
	int chance_to_fire;
} custom_event;

custom_event custom_events[MAX_CUSTOM_EVENTS];

void custom_events_process();

#endif