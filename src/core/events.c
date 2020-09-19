#include "time.h"
#include "events.h"
#include "empire/city.h"
#include "empire/trade_route.h"
#include "city/message.h"

#include <string.h>


int get_months_passed() {
	return get_total_months();
}

int get_city_id_from_name(custom_event_data event_data) {
	for (int i = 0; i < MAX_CITIES; ++i) {
		if (strcmp(event_data.city_name, cities[i].display_name) == 0) {
			return i;
		}
	}

	return 0;
}

void fire_demand_change(custom_event_data event_data)
{
	int city_id = get_city_id_from_name(event_data);
	int route = empire_city_get_route_id(city_id);
    int resource = event_data.resource_id;
	int limit = event_data.amount;

	trade_route_change_limit(route, resource, limit);
	//city_custom_message_post(1, event_data.text, event_data.city_name, resource);
	city_message_post(1, MESSAGE_INCREASED_TRADING, city_id, resource);
}

static condition_value all_condition_values[] = {
	{CONDITION_VALUE_MONTHS_PASSED, "monthsPassed", get_months_passed},
	//{CONDITION_VALUE_RATING_CULTURE, "culture"},
 //   {CONDITION_VALUE_RATING_PEACE, "peace"},
	//{CONDITION_VALUE_RATING_PROSPERITY,	"prosperity"},
	//{CONDITION_VALUE_RATING_FAVOR,	"favor"},
	//{CONDITION_VALUE_MONEY,	"money"},
	//{CONDITION_VALUE_POPULATION, "population"},
	//{CONDITION_VALUE_TRADE_CITY_OPEN, "tradeRouteOpen"},
	//{CONDITION_VALUE_CITY_SENTIMENT, "sentiment"},
	//{CONDITION_VALUE_PATRICIANS, "numPatricians"},
	//{CONDITION_VALUE_PERCENT_IN_TENTS, "percentTents"},
	//{CONDITION_VALUE_PERCENT_PATRICIANS, "percentPatricians"},
	//{CONDITION_VALUE_PERCENT_UNEMPLOYED, "percentUnemployed"},
	//{CONDITION_VALUE_ARMY_STRENGTH,	"armyStrength"}
};

static custom_event_type all_custom_event_types[] = {
	{EVENT_TYPE_DEMAND_CHANGE, "demandChange", fire_demand_change}
};

custom_event_type get_event_type(custom_event_data event_data) {
	int valueType;
	for (int i = 0; i <= CONDITION_VALUE_MAX_KEY; ++i) {
		if (strcmp(event_data.type, all_custom_event_types[i].event_type_string) == 0) {
			valueType = i;
			return all_custom_event_types[i];
		}
	}
}

int test_condition(event_condition condition) {
	int valueType = 0;
	for (int i = 0; i <= CONDITION_VALUE_MAX_KEY; ++i) {
		if (strcmp(condition.value, all_condition_values[i].value_type_string) == 0) {
			valueType = i;
			break;
		}
	}

	if (all_condition_values[valueType].actual_value_function() >= condition.requirement) {
		return 1;
	}
	return 0;
}

int test_conditions_for_event(custom_event event) {
	for (int i = 0; i < MAX_CONDITIONS; ++i) {
		event_condition condition = event.conditions[i];
		if (!strlen(condition.value)) {
			break;
		}
		if (!test_condition(condition)) {
			return 0;
		}
	}

	return 1;
}

void custom_events_process() {
	for (int i = 0; i <= MAX_CUSTOM_EVENTS; ++i) {
		if (custom_events[i].in_use) {
			if (test_conditions_for_event(custom_events[i])) {
				custom_event_type event_type = get_event_type(custom_events[i].event_data);
				event_type.activation_function(custom_events[i].event_data);
			}
		}
		else {
			break;
		}
	}
}
