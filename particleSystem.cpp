#pragma warning(disable : 4786)

#include "particleSystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <vector>
#include <FL/gl.h>
#include <FL/glu.h>
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerui.h"
#include "modelerdraw.h"
#include <random>

#define COLOR_WHITE 1.0f, 1.0f, 1.0f
#define COLOR_ORANGE 1.0f, 0.5f, 0.0f
#define COLOR_YELLOW 1.0f, 1.0f, 0.0f
#define COLOR_LIGHT_BLUE 0.0f, 1.0f, 1.0f
#define COLOR_GREY 0.5f, 0.5f, 0.5f

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-0.5f, 0.5f);

/*
Create a particle system that is attached a node of your model hierarchy other than the root node.
Implement the simulation solver using Euler's method
Create a particle system with at least two types of forces acting on it.  One force should be gravity, and the other should be something that is tied to a feature of your model.  For example, you can have a steam particle system that shoots particles up from the chimney of a moving train.  The initial velocity of these particles should reflect the velocity of the train at the moment they're created.
Using robotarm.cpp as an example, hook your particle system up to the application.
*/

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() {}

/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	particles.clear();
	bakedParticles.clear();
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	bake_start_time = t;
	bake_end_time = -1;
	simulate = true;
	dirty = true;
	lastUpdateTime = t;
	clearBaked();
}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
	bake_end_time = t;
	simulate = false;
	dirty = true;
	particles.clear();
}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
	// TODO
	// These values are used by the UI
	simulate = false;
	dirty = true;
	particles.clear();
	bakedParticles.clear();
}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	if (!simulate) return;
	
	bake_fps = t - lastUpdateTime;
	lastUpdateTime = t;

	for (int i = 0; i < particles.size(); i++) {
		Particle& p = particles[i];

		p.force = (gravity + wind) * bake_fps;

		p.position += p.velocity;
		p.velocity += p.force;
	}

	bakeParticles(t);
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	if (bakedParticles.find(t) != bakedParticles.end()) {
		std::vector<Particle> particles = bakedParticles[t];

		glPointSize(5.0f);
		glBegin(GL_POINTS);
		setDiffuseColor(COLOR_WHITE);

		for (int i = 0; i < particles.size(); i++) {
			Particle& p = particles[i];
			
			glVertex3f(p.position[0], p.position[1], p.position[2]);
		}
		glEnd();
	}
	else {

		if (!simulate) return;

		computeForcesAndUpdateParticles(t);

		glPointSize(5.0f);
		glBegin(GL_POINTS);
		setDiffuseColor(COLOR_WHITE);

		for (int i = 0; i < particles.size(); i++) {
			Particle& p = particles[i];
			glVertex3f(p.position[0], p.position[1], p.position[2]);
		}

		glEnd();
	}
}


/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	bakedParticles[t] = particles;
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	bakedParticles.clear();
}

void ParticleSystem::addNewParticle(Vec3f location)
{
	if (rand() / (float)RAND_MAX > 0.5) return;

	if (!simulate) return;

	float vx = distribution(generator);
	float vy = distribution(generator);
	float vz = distribution(generator);
	Vec3f initialVelocity(vx, vy, vz);

	Particle newParticle(location, initialVelocity, Vec3f(0.0f, 0.0f, 0.0f), 1.0f);
	particles.push_back(newParticle);
}

