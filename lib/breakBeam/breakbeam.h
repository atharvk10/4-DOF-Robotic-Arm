#ifndef breakbeam_h
#define breakbeam_h

#include <Arduino.h>

constexpr int sensorPin = 7;

/**
 * @brief Initializes the break beam sensor.
 * @details Configures the sensor pin as an input so it can be used to detect
 * objects passing through the beam.
 */
void setupBreakBeam();

/**
 * @brief Checks whether a cube has passed through the break beam sensor.
 * @details Compares the current and previous sensor states to detect a new
 * object while preventing multiple detections of the same cube.
 * @return True if a new cube has been detected; otherwise, false.
 */
bool checkCube();

#endif