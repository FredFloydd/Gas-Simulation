// 2D_Gas.cpp   This program simulates the time evolution of many particles in a 2D box, which models an ideal gas
// It outputs position data over time for one particle, that particle's KE, the average KE of all the particles, and the mean free path

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>

#include "Vector_Functions.cpp"
#include "Collision_Functions.cpp"

using namespace std;

int main(){

    int numparticles = 50;    // Initialise the system by creating a box, and particles inside the box
    double vmax = 1;
    double timestep = 0.001;
    int iterations = 5000000;

    vector<Particle> particles;
    vector<Wall> walls;

    double height = 1.0;
    double width = 1.0;
    Wall wall1, wall2, wall3, wall4;
    Vector corner1, corner2, corner3, corner4;  // Corners move anti-clockwise around the box, starting at the origin
    Vector norm1, norm2, norm3, norm4;
    Vector horiz, vert;

    horiz.x = 1.0;
    horiz.y = 0.0;
    horiz.x = 0.0;
    horiz.y = 0.0;

    corner1.x = 0.0;  // Define corners of the box
    corner1.y = 0.0;
    corner2.x = width;
    corner2.y = 0.0;
    corner3.x = width;
    corner3.y = height;
    corner4.x = 0.0;
    corner4.y = height;

    wall1.point1 = corner1; // Initialise walls
    wall1.point2 = corner2;
    wall1.normal.x = -subtract(corner2, corner1).y;
    wall1.normal.y = subtract(corner2, corner1).x;
    wall1.normal = multiply(wall1.normal, 1/length(wall1.normal));

    wall2.point1 = corner2;
    wall2.point2 = corner3;
    wall2.normal.x = -subtract(corner3, corner2).y;
    wall2.normal.y = subtract(corner3, corner2).x;
    wall2.normal = multiply(wall2.normal, 1/length(wall2.normal));

    wall3.point1 = corner3;
    wall3.point2 = corner4;
    wall3.normal.x = -subtract(corner4, corner3).y;
    wall3.normal.y = subtract(corner4, corner3).x;
    wall3.normal = multiply(wall3.normal, 1/length(wall3.normal));

    wall4.point1 = corner4;
    wall4.point2 = corner1;
    wall4.normal.x = -subtract(corner1, corner4).y;
    wall4.normal.y = subtract(corner1, corner4).x;
    wall4.normal = multiply(wall4.normal, 1/length(wall4.normal));

    walls.push_back(wall1);  // Add walls to the wall vector
    walls.push_back(wall2);
    walls.push_back(wall3);
    walls.push_back(wall4);    

    srand(20302042);

    for(int i=0; i<numparticles; i++){  // Generate particles with random positions and velocities within the box
        Particle part;
        Vector pos;
        Vector vel;
        double rad = 0.07;
        double imass = 100.0;

        pos.x = 2 * rad + ((double)(rand() % 100)) * (width - 4 * rad) / 100.0;
        pos.y = 2 * rad + ((double)(rand() % 100)) * (height - 4 * rad) / 100.0;
        vel.x = ((double)(rand() % 100)) * vmax / 100.0f;
        vel.y = ((double)(rand() % 100)) * vmax / 100.0f;

        part.pos = pos;
        part.lastcol = pos;
        part.vel = vel;
        part.mom = multiply(vel, 1 / imass);
        part.rad = rad;
        part.imass = imass;
        part.ke = 0.5 * length(vel) * length(vel) / imass;

        particles.push_back(part);
    }

    double meanfreepath = 0;
    int numcollisions = 0;

    ofstream fout;    // Prepare to write data to a file "data.txt"
    fout.open("data.txt");
    for(int i=0; i<iterations; i++){
        
        double avgke = 0;

        // Update information about the system by a single timestep
        for(int j=0; j<numparticles; j++){
            particles[j].pos = add(particles[j].pos, multiply(particles[j].vel, timestep));    // Increment the position of each particle
            for(int k=0; k<numparticles; k++){
                if(k!=j){
                    if(checkcollision(particles[j], particles[k]) == 1){    // Check for collisions with each other particle
                        meanfreepath *= numcollisions;
                        numcollisions += 1;
                        meanfreepath += freepath(particles[j]) + freepath(particles[k]);
                        meanfreepath *= 1/(double)numcollisions;
                        Pair colpair;
                        colpair.partA = particles[j];
                        colpair.partB = particles[k];
                        colpair = collision(colpair); 
                        particles[j] = colpair.partA;
                        particles[k] = colpair.partB;
                    }
                }
            }
            for(int l=0; l<4; l++){    // Check for collisions with each wall
                if(checkcollision(particles[j], walls[l]) == 1){
                    meanfreepath *= numcollisions;
                    numcollisions += 1;
                    meanfreepath += freepath(particles[j]);
                    meanfreepath *= 1/(double)numcollisions;
                    particles[j] = collision(particles[j], walls[l]);
                }
            }
            avgke += particles[j].ke / numparticles;
        }
        if(i % (iterations/1000) == 0){
            fout << i*timestep << "\t" << particles[0].pos.x << "\t" << particles[0].pos.y << "\t" 
            << particles[0].ke << "\t" << avgke << "\t" << numcollisions << "\t" << meanfreepath << endl;
        }
        if(i % (iterations/100) == 0){
            cout << "|";
        }
    }
    return 0;
}