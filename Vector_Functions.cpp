// Vector_Functions.cpp    This file contains functions which perform operations on 2D vectors

struct Vector{  // Define a struct to deal with vectors, as many are used here
    double x;
    double y;
};

double length(Vector A){    // Various functions for performing mathematical operations on vectors
    return sqrt(A.x * A.x + A.y * A.y);
}

double dot(Vector A, Vector B){
    return A.x * B.x + A.y * B.y;
}

Vector add(Vector A, Vector B){
    Vector result;
    result.x = (double)A.x + (double)B.x;
    result.y = (double)A.y + (double)B.y;
    return result;
}

Vector subtract(Vector A, Vector B){
    Vector result;
    result.x = (double)A.x - (double)B.x;
    result.y = (double)A.y - (double)B.y;
    return result;
}

Vector multiply(Vector A, double C){
    Vector result;
    result.x = C * (double)A.x;
    result.y = C * (double)A.y;
    return result;
}

Vector direction(Vector A, Vector B){   // Finds the unit direction vector from A to B
    Vector difference = subtract(B, A);
    return multiply(difference, 1/(double)length(difference));
}