#include    <stdio.h>
#include    <math.h>

#define RENDER_DIST 10
#define ASSERT_EQUALS_FLOAT(x,y) (fabs(x-y) < 0.01)?  printf("Test Passed: \t%f, \t%f\n",x,y) : fprintf(stderr, "Test failed: \t%f, \t%f\n",x,y)
#define ASSERT_TRUE(x)  x ? printf("Test Passed\n") : fprintf(stderr, "Test failed\n")
#include "../src/utils.c"

v2_f v1 = {1,3};
v2_f v2 = {1,10};
v2_f v3 = {6,3};
v2_f v4 = {1,-2};
v2_f v5 = {4,2};
v2_f v6 = {0,0};
v2_f v7 = {1,0};
v2_f v8 = {5,3};
v2_f v9 = {12,12};
v2_f v10 = {0,1};


void test_get_distance() {
    printf("Testing the get_distance_squared function :\n");
    ASSERT_EQUALS_FLOAT(25.0            , get_distance_squared(v1,v3));
    ASSERT_EQUALS_FLOAT(288.0           , get_distance_squared(v9,v6));
    ASSERT_EQUALS_FLOAT(16.0            , get_distance_squared(v1,v8));
    ASSERT_EQUALS_FLOAT(2.0             , get_distance_squared(v7,v10));
    ASSERT_EQUALS_FLOAT(50.0            , get_distance_squared(v3,v4));
}



void test_get_length() {
    printf("Testing the get_length function :\n");
    ASSERT_EQUALS_FLOAT(sqrt(10)    , get_length(v1));
    ASSERT_EQUALS_FLOAT(sqrt(101)   , get_length(v2));
    ASSERT_EQUALS_FLOAT(sqrt(45)    , get_length(v3));
    ASSERT_EQUALS_FLOAT(sqrt(5)     , get_length(v4));
}

void test_set_length() {
    printf("Testing the set_length function :\n");
    
}

void test_rotate() {
    printf("Testing the rotate function :\n");
}

void test_check_hit() {
    printf("Testing the check_hit function :\n");
    pos.x = v6.x;
    pos.y = v6.y;
    ASSERT_EQUALS_FLOAT(-1.0 ,check_hit(v10,v4,v8)); //no hit and upward view
    ASSERT_EQUALS_FLOAT(-1.0, check_hit(set_length(1,v9),v7,v8)); //no hit
    pos.x = 0;
    pos.y = 0;
    ASSERT_EQUALS_FLOAT(-1.0,check_hit((v2_f) {0,1},(v2_f) {0,13},(v2_f) {0,7}));// wall parralel and upward view
    ASSERT_EQUALS_FLOAT(-1.0,check_hit(set_length(1,(v2_f) {1,2}),(v2_f) {3,6},(v2_f) {2,4}));// wall parralel
    ASSERT_EQUALS_FLOAT(1.490,check_hit(set_length(1,(v2_f) {-1,-2}),(v2_f) {3,-1},(v2_f) {-8,-2}));// hit
    ASSERT_EQUALS_FLOAT(10.0,check_hit((v2_f) {0,1},(v2_f) {5,10},(v2_f) {-6,10}));// hit with upward view
    ASSERT_EQUALS_FLOAT(-1.0,check_hit((v2_f) {1,1},(v2_f) {0,-2},(v2_f) {-3,0}));// behind player's back
    pos.x = 4;
    pos.y = 4;
    ASSERT_EQUALS_FLOAT(-1.0,check_hit((v2_f) {0,1},(v2_f) {9,-6},(v2_f) {1,0}));// behind player's back with upward view
}

int main() {
    test_get_distance();
    test_check_hit();


    return 0;
}

