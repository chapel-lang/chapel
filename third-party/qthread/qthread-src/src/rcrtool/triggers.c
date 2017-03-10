#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "triggers.h"

/***********************************************************************
* definition for the trigger map; used by all methods
***********************************************************************/
Trigger** triggerMap  = NULL; // allocated and pupulated in buildTriggerMap()
int       numTriggers = 0;    // populated in buildTriggerMap()

/**************************************************************************************
Trim strings with white spaces
***************************************************************************************/
/* Remove leading whitespaces */
static char *ltrim(char *const s)
{
    size_t len;
    char *cur;

    if (s && *s) {
        len = strlen(s);
        cur = s;

        while (*cur && isspace(*cur))
            ++cur, --len;

        if (s != cur)
            memmove(s, cur, len + 1);
    }

    return s;
}

/* Remove trailing whitespaces */
static char *rtrim(char *const s)
{
    if (s && *s) {
        char *cur = s + strlen(s) - 1;

        while (cur != s && isspace(*cur))
            --cur;

        cur[isspace(*cur) ? 0 : 1] = '\0';
    }

    return s;
}

/* Remove leading and trailing whitespaces */
static char *trim(char *const s)
{
    rtrim(s);
    ltrim(s);

    return s;
}

/*!
 * Reads from a trigger file and builds the <i>triggerMap<i/>. <i>triggerMap</i>
 * is an array of TriggerMap structs .
 * 
 * \param fileName Name of the trigger definition file.
 */
void buildTriggerMap(const char *fileName){
    int triggerCount = 0; // counts the number of triggers in the triggerFile
    char line[BUFSIZ];
    char *nextField;

    //printf("Reading from trigger file: %s \n", fileName);

    // Check if file exists
    FILE *triggerFile = fopen(fileName, "r");
    if (triggerFile == NULL) {
        printf("Can't open trigger file !! \n");
        return;
    }

    // Read each line, allocate memory for triggerMap and populate elements of the triggerMap array
    while (fgets(line, BUFSIZ, triggerFile) != NULL) {

        // Allocate memory for the next trigger entry
        triggerMap = (Trigger **) qt_realloc(triggerMap,
                                             (triggerCount + 1) * sizeof(Trigger *));
        triggerMap[triggerCount] = (Trigger *) qt_malloc(sizeof(Trigger));

        // Parsing type
        nextField = (char *) strtok(line, ",");
        nextField = trim(nextField); // remove trailing and leading white spaces
        if (strcmp(nextField, "T_TYPE_CORE") == 0) {
            triggerMap[triggerCount]->meterType = T_TYPE_CORE;
        } else if (strcmp(nextField, "T_TYPE_SOCKET") == 0) {
            triggerMap[triggerCount]->meterType = T_TYPE_SOCKET;
        } else if (strcmp(nextField, "T_TYPE_NODE") == 0) {
            triggerMap[triggerCount]->meterType = T_TYPE_NODE;
        } else {
            printf("Unknown trigger type: type must be TYPE_CORE or TYPE_SOCKET or TYPE_NODE \n");
            printf("Ignoring the rest of the trigger file \n");
            break;
        }

        // Parsing id
        nextField = (char *) strtok(NULL, ",");
        triggerMap[triggerCount]->id = atoi(nextField);

        // Parsing trigger flag shared memory key
        nextField = (char *) strtok(NULL, ",");
        triggerMap[triggerCount]->flagShmKey = atoi(nextField);

        // Parsing application state shared memory key
        nextField = (char *) strtok(NULL, ",");
        triggerMap[triggerCount]->appStateShmKey = atoi(nextField);

        // Parsing meterName
        nextField = (char *) strtok(NULL, ",");
        nextField = trim(nextField);
        triggerMap[triggerCount]->meterName = (char *) qt_malloc((1 + strlen(nextField)) * sizeof(char));
        strcpy(triggerMap[triggerCount]->meterName , nextField);

        // Parsing threshold lower bound
        nextField = (char *) strtok(NULL, ",");
        triggerMap[triggerCount]->threshold_lb = atof(nextField);

        // Parsing threshold upper bound
        nextField = (char *) strtok(NULL, ",");
        triggerMap[triggerCount]->threshold_ub = atof(nextField);


        triggerCount++;

    } // End reading from trigger file

    fclose(triggerFile);

    numTriggers = triggerCount;

    //printTriggerMap();

}

