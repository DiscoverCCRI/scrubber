/**
 * @file constants.h
 * Constants for the scrubber. Mostly upper/lower bounds for outlier ranges
 */
/**
 * @brief Please see the SEEED SenseCAP900 Datasheet for intervals
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

/** set to 0 since we do not care about these ranges */
#define ID_LO 0.0
#define ID_HI 100000000.0
#define TIME_LO 0.0
#define TIME_HI 10000000000000000000000000.0
#define DUMP_LO 0.0
#define DUMP_HI 100000000.0

/** temperature & heating temperature */
#define TEMP_LO -50.0 /** Celsius */
#define TEMP_HI 85.0

/** humidity */
#define HUM_LO 0.0 /** Relative Humidity % */
#define HUM_HI 100.0

/** barometric pressure */
#define PRESS_LO 30000.0 /** Pa Pascals */
#define PRESS_HI 125000.0

/** light intensity */
#define LIGHT_LO 0.0 /** Lux */
#define LIGHT_HI 188000.0

/** min_wind_speed, max_wind_speed, avg_wind_speed */
#define WIND_LO 0.0 /** ms milliseconds */
#define WIND_HI 60.0

/** min_wind_direction, max_wind_direction, avg_wind_direction */
#define WIND_DIR_LO 0.0 /** degrees */
#define WIND_DIR_HI 360.0

// TODO: rethink accum duration lower/upper accum_rainfall,
// accum_rainfall_duration, rainfall_intensity, max_rainfall_intensity
/** RAIN INTENSITY */
#define RAIN_LO 0.0 /** mm/h millimeters/hour */
#define RAIN_HI 200.0

/** PM2_5 & PM10 */
#define PM_LO 0.0 /** µg/m^3 micrograms per cubic meter */
#define PM_HI 1000.0
// S1000 measurements? TODO

#endif
