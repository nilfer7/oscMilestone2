//
// Created by nilfer on 11/16/25.
//
#include <stdio.h>
#include <stdbool.h>
#include "sensor_db.h"


FILE * open_db(char * filename, bool append)
{
    FILE *f; // variable f: pointer to a file
    if (append)
        f = fopen(filename, "a"); // append mode
    else
        f = fopen(filename, "w"); // overwrite mode

    if (f == NULL) { // first i wanted to do this test but without calling f without opening it i can't check if it is null or not
        perror("open_db failed");
        return NULL;
    }

    return f;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if (f == NULL)
        return -1;

    int result = fprintf(f, "%hu,%f,%ld\n", id, value, ts); //hu unsigned 16 bit int

    if(result < 0) // result: aantal characters
        return -1;

    return 0;
}
    int close_db(FILE * f)
    {
        if (f == NULL)
            return -1;

        int result = fclose(f);

        if (result == EOF)
            return -1;

        return 0;
    }
