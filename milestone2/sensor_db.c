//
// Created by nilfer on 11/18/25.
//


#include <stdio.h>
#include <stdbool.h>
#include "sensor_db.h"
#include "logger.h"

FILE * open_db(char * filename, bool append)
{
    FILE *f; // variable f: pointer to a file

    if (append)
        f = fopen(filename, "a");
    else
        f = fopen(filename, "w");

    if (f == NULL) {
        perror("open_db failed");
        write_to_log_process("Error opening data file."); //It sends the text message through the pipe cuz i call the function
        return NULL;
    }

    write_to_log_process("Data file opened.");
    return f;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts)
{
    if (f == NULL)
        return -1;

    int result = fprintf(f, "%hu,%f,%ld\n", id, value, ts);

    if (result < 0) {
        write_to_log_process("Error inserting data.");
        return -1;
    }

    fflush(f);
    write_to_log_process("Data inserted.");
    return 0;
}

int close_db(FILE * f)
{
    if (f == NULL)
        return -1;

    int result = fclose(f);

    if (result == EOF) {
        write_to_log_process("Error closing data file.");
        return -1;
    }

    write_to_log_process("Data file closed.");
    end_log_process();


    return 0;
}
