#include "mods.h"

#include "core/calc.h"
#include "core/dir.h"
#include "core/file.h"
#include "core/log.h"
#include "core/png_read.h"
#include "core/string.h"

#include "empire/object.h"

#include "scenario/data.h"

#include "expat.h"

#include <string.h>

static struct {
    struct {
        char file_name[FILE_NAME_MAX];
        size_t file_name_position;
        int depth;
        int error;
        int finished;
        struct scenario_t *scenario;
    } xml;
} scenario_data;


static const char SCEN_XML_FILE_ELEMENTS[XML_MAX_DEPTH][XML_MAX_ELEMENTS_PER_DEPTH][XML_TAG_MAX_LENGTH] = { { "scenario" }, { "description" } };
static const char SCEN_XML_FILE_ATTRIBUTES[XML_MAX_DEPTH][XML_MAX_ELEMENTS_PER_DEPTH][XML_MAX_ATTRIBUTES][XML_TAG_MAX_LENGTH] = {
    { { "name", "startDate", "author" } }, // scenario
    { { "tagline", "text" } } // description
};


static void scen_xml_start_scenario_element(const char** attributes);
static void scen_xml_start_description_element(const char** attributes);
static void scen_xml_end_scenario_element(void);
static void scen_xml_end_description_element(void);

static void (*scen_xml_start_element_callback[XML_MAX_DEPTH][XML_MAX_ELEMENTS_PER_DEPTH])(const char** attributes) = {
    { scen_xml_start_scenario_element }, { scen_xml_start_description_element }
};

static void (*scen_xml_end_element_callback[XML_MAX_DEPTH][XML_MAX_ELEMENTS_PER_DEPTH])(void) = {
    { scen_xml_end_scenario_element }, { scen_xml_end_description_element }
};

// import?
static int count_xml_attributes(const char** attributes)
{
    int total = 0;
    while (attributes[total]) {
        ++total;
    }
    return total;
}


static void scen_xml_start_scenario_element(const char** attributes)
{
    scenario_data.xml.scenario = &scenario;

    int total_attributes = count_xml_attributes(attributes);
    for (int i = 0; i < total_attributes; i += 2) {

        if (strcmp(attributes[i], SCEN_XML_FILE_ATTRIBUTES[0][0][0]) == 0) {
            strcpy(scenario_data.xml.scenario->scenario_name, string_from_ascii(attributes[i + 1]));
        }
        if (strcmp(attributes[i], SCEN_XML_FILE_ATTRIBUTES[0][0][1]) == 0) {
            scenario_data.xml.scenario->start_year = string_to_int(string_from_ascii(attributes[i + 1]));
        }
        if (strcmp(attributes[i], SCEN_XML_FILE_ATTRIBUTES[0][0][2]) == 0) {
            strcpy(scenario_data.xml.scenario->author, string_from_ascii(attributes[i + 1]));
        }
    }
}

static void scen_xml_start_description_element(const char** attributes)
{
    scenario_data.xml.scenario = &scenario;
    
    int total_attributes = count_xml_attributes(attributes);
    for (int i = 0; i < total_attributes; i += 2) {

        if (strcmp(attributes[i], SCEN_XML_FILE_ATTRIBUTES[1][0][0]) == 0) {
            strcpy(scenario_data.xml.scenario->brief_description, string_from_ascii(attributes[i + 1]));
        }
        if (strcmp(attributes[i], SCEN_XML_FILE_ATTRIBUTES[1][0][1]) == 0) {
            strcpy(scenario_data.xml.scenario->briefing, string_from_ascii(attributes[i + 1]));
        }
    }
}

static void scen_xml_end_scenario_element(void)
{
    scenario_data.xml.finished = 1;
}

static void scen_xml_end_description_element(void)
{
}

// import?
static int get_element_index(const char* name)
{
    for (int i = 0; i < XML_MAX_ELEMENTS_PER_DEPTH; ++i) {
        if (SCEN_XML_FILE_ELEMENTS[scenario_data.xml.depth][i][0] == 0) {
            continue;
        }
        if (strcmp(SCEN_XML_FILE_ELEMENTS[scenario_data.xml.depth][i], name) == 0) {
            return i;
        }
    }
    return -1;
}

//import?
static void XMLCALL xml_start_element(void* unused, const char* name, const char** attributes)
{
    if (scenario_data.xml.error) {
        return;
    }
    if (scenario_data.xml.finished || scenario_data.xml.depth == XML_MAX_DEPTH) {
        scenario_data.xml.error = 1;
        log_error("Invalid XML parameter", name, 0);
        return;
    }
    int index = get_element_index(name);
    if (index == -1) {
        scenario_data.xml.error = 1;
        log_error("Invalid XML parameter", name, 0);
        return;
    }
    (*scen_xml_start_element_callback[scenario_data.xml.depth][index])(attributes);
    scenario_data.xml.depth++;
}

//import?
static void XMLCALL xml_end_element(void* unused, const char* name)
{
    if (scenario_data.xml.error) {
        return;
    }
    scenario_data.xml.depth--;
    int index = get_element_index(name);
    if (index == -1) {
        scenario_data.xml.error = 1;
        log_error("Invalid XML parameter", name, 0);
        return;
    }
    (*scen_xml_end_element_callback[scenario_data.xml.depth][index])();
}


//import?
static void clear_xml_info(void)
{
    scenario_data.xml.error = 0;
    scenario_data.xml.depth = 0;
    scenario_data.xml.finished = 0;
}


static void process_file(const char* xml_file_name)
{
    log_info("Loading xml file", xml_file_name, 0);

    FILE* xml_file = file_open(xml_file_name, "r");

    if (!xml_file) {
        log_error("Error opening xml file", xml_file_name, 0);
        return;
    }

    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetElementHandler(parser, xml_start_element, xml_end_element);

    char buffer[XML_BUFFER_SIZE];
    int done = 0;

    do {
        size_t bytes_read = fread(buffer, 1, XML_BUFFER_SIZE, xml_file);
        done = bytes_read < sizeof(buffer);
        if (XML_Parse(parser, buffer, (int)bytes_read, done) == XML_STATUS_ERROR || scenario_data.xml.error) {
            log_error("Error parsing file", xml_file_name, 0);
            break;
        }
    } while (!done);

    if (scenario_data.xml.error || !scenario_data.xml.finished) {
        log_info("scenario xml load failed", "", 0);
    }

    clear_xml_info();

    XML_ParserFree(parser);
    file_close(xml_file);
}


void scenario_load_from_file(const char* file_path)
{
    process_file(file_path);

}
