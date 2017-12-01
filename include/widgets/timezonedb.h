#ifndef INC_TIMEZONEDB_H
#define INC_TIMEZONEDB_H
//http://api.timezonedb.com/v2/get-time-zone?key=82UYFAXGCJ5X&format=json&by=position&lat=47.4607&lng=19.1148&fields=formatted
#define TZDB_BASEURL "http://api.timezonedb.com/v2/get-time-zone?key=82UYFAXGCJ5X"
#define TZDB_JSONFORMAT "&format=json"
#define TZDB_BY "&by=position"
#define TZDB_LAT "&lat="
#define TZDB_LNG "&lng="
#define TZDB_FIELDS "&fields=formatted,timestamp"
// {"status":"OK","message":"","formatted":"2017-11-30 21:03:58"}
#define TZDB_JSON_FORMATTED_DATE_TIME ":formatted"
#define TZDB_JSON_TIMESTAMP ":timestamp"

#endif