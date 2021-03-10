#include "Vector_Functions.h"

struct Particle{    // Define a struct containing information about the properties of each particle
    Vector pos;
    Vector vel;
    Vector mom;
    Vector lastcol;
    double rad;
    double imass;
    double ke;
};

struct Pair{    // Collisions will be between pairs of particles, so create a struct for passing into collision functions
    Particle partA;
    Particle partB;
};

struct Wall{    // The gas will be in a confined space, so walls need to be dealt with
    Vector point1;
    Vector point2;
    Vector normal;  
}; 

double freepath(Particle part);

Pair collision(Pair particles);

int checkcollision(Particle partA, Particle partB);

Particle collision(Particle part, Wall wall);

int checkcollision(Particle part, Wall wall);