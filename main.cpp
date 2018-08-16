/*
File Name: main.cpp
Copyright © 2018
Original authors: Srinivasan Thiagarajan, Sanketh Bhat
Refactored by Sanketh Bhat
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Description:
This is the skeleton for a basic 2D engine in OpenGL.
*/


#include "GLRender.h"
#include "GameObject.h"

#include "Spring.h"





#pragma region program specific Data members

float timestep = .016;
//the number of disvision used to make the structure of the circle
int NumberOfDivisions = 20;


glm::vec3 mousePos;

GameObject circle(MVP, glm::vec4(1,0,0,1));



// vector of scene bodies
std::vector<GameObject*> bodies;


std::vector<VertexFormat> lines;
VertexBuffer lineBuffer;

#pragma endregion



glm::vec3 GetMousePos()
{
	
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	//normalized device coordinates
	float NDCX = (x / 400.0f) - 1.0f;
	float NDCY = -(y / 400.0f) + 1.0f;

	return glm::vec3(NDCX, NDCY, 0.0f);	


}

#pragma region util_functions

//Adds the position of the gameobject to the vector of vertex format and then feeds the buffer
void loadLines()
{


	lines.clear();
	

	lines.push_back(VertexFormat(circle.Position(), circle.color));
	lines.push_back(VertexFormat(GetMousePos(), circle.color));
	
	lineBuffer.initBuffer(lines.size(), &lines[0]);
}


//Similar code to renderBody(), however here we use GL_LINES instead of GL_TRIANGLES
void renderLines()
{
	glLineWidth(5/(circle.Position() - GetMousePos()).length());


	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(MVP));
	glBindBuffer(GL_ARRAY_BUFFER, lineBuffer.vbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)16);

	glDrawArrays(GL_LINES, 0, lineBuffer.numberOfVertices);


}

void update()
{
	objVelocity = circle.Velocity();
	circle.totalForce += GetSpringForce(circle.Position(), GetMousePos());
	

	for each (GameObject *body in bodies)
			body->update(timestep, PV);

	circle.Position() = GetMousePos();

}





// This function is used to handle key inputs.
// It is a callback funciton. i.e. glfw takes the pointer to this function (via function pointer) and calls this function every time a key is pressed in the during event polling.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Sets the current window to a close flag when ESC is pressed
	if (key == GLFW_KEY_ESCAPE && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
	{
		glfwSetWindowShouldClose(window, 1);
	}

	if (key == GLFW_KEY_SPACE && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
	{
		dampen = true;
	}
	else
	{
		dampen = false;
	}



}
#pragma endregion


void main()
{
	// Initializes most things needed before the main loop
	init();

	// Sends the funtion as a funtion pointer along with the window to which it should be applied to.
	glfwSetKeyCallback(window, key_callback);

	//Sets up bodies in the scene
	circle.setupCircle(.05f, NumberOfDivisions);
	circle.integType = 3;
	circle.Position(glm::vec3(0, 0, 0)); //Spawning off screen
	bodies.push_back(&circle);

	std::cout << "Space to apply damping";

	
	// Enter the main loop.
	while (!glfwWindowShouldClose(window))
	{

		// Call to update() which will update the gameobjects.
		update();
		
		loadLines();
		// Call the render function(s).
		renderScene();
		renderLines();
		//Rendering each body after the scene
		for each (GameObject *body in bodies)
			renderBody(body);

		// Swaps the back buffer to the front buffer
		// Remember, you're rendering to the back buffer, then once rendering is complete, you're moving the back buffer to the front so it can be displayed.
		glfwSwapBuffers(window);

		// Checks to see if any events are pending and then processes them.
		glfwPollEvents();

		glDeleteBuffers(1, &lineBuffer.vbo);
	}

	//Cleans shaders and the program and frees up GLFW memory
	cleanup();

	return;
}