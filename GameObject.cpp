#include "GameObject.h"



void GameObject::update(float dt, glm::mat4 PV)
{
	addForces();

	if (integType == 1)
		eulerIntegration(dt);
	else if (integType == 2)
		RK2Integration(dt);
	else if (integType == 3)
		RK4Integration(dt);

	translation = glm::translate(glm::vec3(position));

	calcTransform();
	MVP = PV * transformation;

	totalForce = acceleration = glm::vec3();


}

void GameObject::addForces()
{
	totalForce += GRAVITY * mass;

	acceleration = totalForce / mass;
}

//Calculates Transformation matrix -> T * R * S
void GameObject::calcTransform()
{
	transformation = translation * rotation * scale;
	
}

//Updates velocity based off pure Euler integration
void GameObject::eulerIntegration(float dt)
{

	position += velocity * dt;

	//Updating velocity
	velocity += acceleration * dt;

}

//Runge-Kutta Integration 2nd Level
void GameObject::RK2Integration(float dt)
{
	

// Get the acceleration at mid of the time step. This is the implementation of the funciton F in RK integrator literature.
// Since this is a velocity intergrator, It is independant of the displacement. If this were to integrate a spring, the
// current displacement would also be comuted at the point dt/2 (mid-point)
velocity += acceleration * dt/2.0f;

//Use the velocity at the mid point to compute the displacement during the timestep h
position += dt * velocity;

//Change the velocity to the value at the end of the timestep.
velocity += acceleration * dt/2.0f;


}

//Runge-Kutta Integration 4th Level
void GameObject::RK4Integration(float dt)
{
	/*
	K1 is the increment based on the slope at the beginning of the interval, uing y (euler's method)
	k2 is the increment based on the slope at the midpoint of the interval, using y + (h/2)k1
	k3 is the increment based on the slope at the midpoint of the interval, using y + (h/2)k2
	k4 is the increment based on the slope at the end of the interval, using y + h*k3

	k1-------------k2-----------------k3----------------k4
	|<--------------------- T -------------------------->|
	*/

	glm::vec3 k1, k2, k3, k4, vel;
	glm::vec3 k;

	vel = velocity;

	//Euler 1
	k = position + velocity * dt;
	vel += acceleration * dt;
	k1 = vel;

	//Euler 2
	k = (position + (dt*k1 / 2.0f)) + vel * dt / 2.0f;
	vel += acceleration * dt/2.0f;
	k2 = vel;

	//Euler 3
	k = (position + (dt*k2 / 2.0f)) + vel * dt / 2.0f;
	vel += acceleration * dt/2.0f;
	k3 = vel;

	//Euler 4
	k = (position + (dt*k3)) + vel * dt;
	vel += acceleration * dt;
	k4 = vel;

	//Use the velocity at the mid point to compute the displacement during the timestep h
	position += dt * (k1 + (2.0f * k2) + (2.0f * k3) + k4) / 6.0f;


	//Change the velocity to the value at the end of the timestep.
	velocity += acceleration * dt;

}


//Sets up the vertices in the vertex buffer
void GameObject::setupCircle( float r , float n)
{

	float radius = r;


	VertexFormat center(glm::vec3(0.0f, 0.0f, 0.0f), color);
	std::vector<VertexFormat> vertices;

	float theta = 360.0f / n;

	//Circle vertex generation
	//In this example we are not implementing the proper the code for indices. We are just going to produce redundant information in the buffer.
	//since we are only having a small number of objects on the screen currently, this redundancy should not matter.
	for (int i = 0; i < n; i++)
	{
		//In every iteration, the center, the point at angle theta and at angle (theta+delta) are fed into the buffer.
		vertices.push_back(center);
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(glm::radians(i*theta)), radius * sin(glm::radians(i*theta)), 0.0f), color));
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(glm::radians((i + 1)*theta)), radius * sin(glm::radians((i + 1)*theta)), 0.0f), color));
	}

	base.initBuffer(n * 3, &vertices[0]);
}


//Initalizing values
GameObject::GameObject(glm::mat4 mvp, glm::vec4 c)
{
	velocity = totalForce = acceleration = position = glm::vec3();

	translation=rotation=scale=transformation= glm::mat4();

	mass = 1.0f;

	MVP = mvp;

	color = c;
}


GameObject::~GameObject()
{
}
