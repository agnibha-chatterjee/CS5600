/*
* mpg2km.c / Mileage Conversion Functions
*
* Paws Husky / CS5600 / Northeastern University
* Fall 2024 / Sep 8, 2024
*
*/

#include "mpg2km.h"

// Conversion constants
#define KM_PER_MILE 1.60934
#define LITERS_PER_GALLON 3.78541

/**
 * Converts miles per gallon to kilometers per liter
 * @param mpg Miles per gallon value
 * @return Kilometers per liter equivalent
 */
double mpg2kml(double mpg) {
    if (mpg <= 0) return 0;  // Handle bad input
    return mpg * (KM_PER_MILE / LITERS_PER_GALLON);
}

/**
 * Converts miles per gallon to liters per 100 kilometers
 * @param mpg Miles per gallon value
 * @return Liters per 100 kilometers equivalent
 */
double mpg2lphm(double mpg) {
    if (mpg <= 0) return 0;  // Handle bad input
    return (100 * LITERS_PER_GALLON) / (mpg * KM_PER_MILE);
}

/**
 * Converts liters per 100 kilometers to miles per gallon
 * @param lph Liters per 100 kilometers value
 * @return Miles per gallon equivalent
 */
double lph2mpg(double lph) {
    if (lph <= 0) return 0;  // Handle bad input
    return (100 * LITERS_PER_GALLON) / (lph * KM_PER_MILE);
} 