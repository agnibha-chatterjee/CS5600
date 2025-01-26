/*
* learnc.c / Mileage Conversion Assignment
*
* Agnibha Chatterjee / CS5600 / Northeastern University
* Spring 2025 / Jan 26, 2025
*
*/

#include <stdio.h>
#include "mpg2km.h"

int main() {
    // Test cases for mpg2kml
    printf("20 MPG = %.2f km/l\n", mpg2kml(20));
    printf("0 MPG = %.2f km/l\n", mpg2kml(0));  // Bad input
    printf("-5 MPG = %.2f km/l\n", mpg2kml(-5)); // Bad input

    // Test cases for mpg2lphm
    printf("30 MPG = %.2f l/100km\n", mpg2lphm(30));
    printf("0 MPG = %.2f l/100km\n", mpg2lphm(0));  // Bad input

    // Test cases for lph2mpg
    printf("8 l/100km = %.2f MPG\n", lph2mpg(8));
    printf("0 l/100km = %.2f MPG\n", lph2mpg(0));  // Bad input

    return 0;
} 