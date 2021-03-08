// Collision_Functions.cpp  This file contains functions which handle collisions between particles and walls

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

double freepath(Particle part){    // Function for determining the distance travelled since the particles last collision
    return length(subtract(part.pos, part.lastcol));
}

Pair collision(Pair particles){    // Function for simulating collisions between particles
    Particle partA = particles.partA;
    Particle partB = particles.partB;
    Vector dirAB = direction(partA.pos, partB.pos);
    Vector dirBA = direction(partB.pos, partA.pos);
    
    double massfracA = 2 * partA.imass / (partA.imass + partB.imass);   // Calculates intermediate values for change in velocity, to keep the code clean
    double massfracB = 2 * partB.imass / (partA.imass + partB.imass);

    double velcoeffA = dot(subtract(partA.vel, partB.vel), dirBA);
    double velcoeffB = dot(subtract(partB.vel, partA.vel), dirAB);

    Vector newvelA = subtract(partA.vel, multiply(dirBA, massfracA * velcoeffA));   // Calculates the new velocities of the particles
    Vector newvelB = subtract(partB.vel, multiply(dirAB, massfracB * velcoeffB));

    double keA = 0.5 * length(newvelA) * length(newvelA);   // Calculates new values of other quantities which will change during the collision
    double keB = 0.5 * length(newvelB) * length(newvelB);

    Vector momA = multiply(newvelA, 1 / partA.imass);
    Vector momB = multiply(newvelB, 1 / partB.imass);

    Particle newpartA = partA;  // Puts the new information about the particles into a Pair struct, and returns it
    Particle newpartB = partB;
    Pair aftercollision;

    newpartA.vel = newvelA;
    newpartA.mom = momA;
    newpartA.lastcol = partA.pos;
    newpartA.ke = keA;

    newpartB.vel = newvelB;
    newpartB.mom = momB;
    newpartB.lastcol = partB.pos;
    newpartB.ke = keB;

    aftercollision.partA = newpartA;
    aftercollision.partB = newpartB;

    return aftercollision;
}

int checkcollision(Particle partA, Particle partB){    // Checks if two particles have collided. Returns 1 if they have, 0 otherwise
    if(length(subtract(partA.pos, partB.pos)) < (partA.rad + partB.rad)){
        return 1;
    }
    else{
        return 0;
    }
}

Particle collision(Particle part, Wall wall){   // Handles collisions between particles and walls
    double velcoeff = -dot(part.vel, wall.normal);
    Vector newvel = subtract(part.vel, multiply(wall.normal, -2 * velcoeff));   // Calculates the new velocity, in the limit that the wall has infinite mass
    Vector newmom = multiply(newvel, 1 / part.imass);
    
    Particle newpart = part;  // Returns a new particle with updated velocity and momentum

    newpart.vel = newvel;
    newpart.mom = newmom;
    newpart.lastcol = part.pos;

    return newpart;
}

int checkcollision(Particle part, Wall wall){   // Checks if a particle has collided with a wall
    
    Vector point1 = wall.point1;
    Vector point2 = wall.point2;
    Vector pos = part.pos;

    double distnum = (point2.x - point1.x) * (point1.y - pos.y) - (point1.x - pos.x) * (point2.y - point1.y);   // Calculates distance between wall and particle
    double distdem = length(subtract(point2, point1));
    double dist = distnum / distdem;

    if(dist * dist < part.rad * part.rad){
        return 1;
    }
    else{
        return 0;
    }
}