#ifndef _SPRING_H
#define _SPRING_H

#include "GLIncludes.h"




    bool dampen = false;
	glm::vec3 objVelocity;
	float k=0.002; //Spring Constant(stiffness)
	float c = 0.01f;
	glm::vec3 springForce;



	glm::vec3 GetSpringForce(glm::vec3 p1, glm::vec3 p2)
	{
		//Hooke's Law F=-K*dx  where dx is the displacement of the spring from its natural equilibrium state
		if(!dampen)
			return springForce = -k * (p1-p2);
		else
		{	//Hooke's Law  with dampening is F=-K* dx -c * V where c is the dampening constant and v is the velocity
			return springForce = -k * (p1 - p2) - c * objVelocity;
		}
	}


#endif _SPRING_H
