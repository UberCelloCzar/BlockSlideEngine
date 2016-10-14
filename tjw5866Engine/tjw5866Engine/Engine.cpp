// Trevor Walden, IGME 209 Section 2, 17 March 2016
// This file has the code for the functions of the Engine class, which handles openGL and screen drawing; it also has the anonymous namespace used for glfw input handling

#include "Engine.h"
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <FreeImage.h>
#include <vector>
#include <map>
#include <iostream>
#include "GameObject.h"
#include "Texture.h"
#include "Camera.h"
#include "InputManager.h"

Engine::Engine(void)
{
}

bool Engine::init(void) // Initialize GLFW and GLEW tools
{
	if (glfwInit() == GL_FALSE) // Initialize GLFW or return failure
	{
		return false;
	}

	GLFWwindowPtr = glfwCreateWindow(1280, 960, "tjw5866 DSA1 Engine", NULL, NULL); // Create window

	if (GLFWwindowPtr != nullptr) // Make window current active,
	{
		glfwMakeContextCurrent(GLFWwindowPtr);
	}
	else // Or return failure
	{
		glfwTerminate();
		return false;
	}

	if (glewInit() != GLEW_OK) // Initialize GLEW or return failure
	{
		glfwTerminate();
		return false;
	}

	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick); // Set the click function
	glfwSetKeyCallback(GLFWwindowPtr, keyCallback); // Set keypress function
	glfwSetInputMode(GLFWwindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Hide the cursor

	camera = Camera({0,0,1}, {0,0,0}, 1280.f/960.f, .01f, 2, 1, 1280, 960); // Create camera: loc, rot, aspect, zNear, zFar, zoom, width, height
	textures["IceBig"] = Texture(); // Assign texture names
	textures["IceSmall"] = Texture();
	textures["Stone"] = Texture();
	textures["StoneTile"] = Texture();
	textures["Button"] = Texture();
	textures["Block"] = Texture();
	textures["Player"] = Texture();

	if (!textures["IceBig"].loadTexture("images/iceBig.png")) return false; // Load the textures from files to video card
	if (!textures["IceSmall"].loadTexture("images/iceSmall.png")) return false;
	if (!textures["Stone"].loadTexture("images/tiles.png")) return false;
	if (!textures["StoneTile"].loadTexture("images/stoneTile.png")) return false;
	if (!textures["Button"].loadTexture("images/button.png")) return false;
	if (!textures["Block"].loadTexture("images/blockTexture.png")) return false;
	if (!textures["Player"].loadTexture("images/playerTexture.png")) return false;

	gameObjects[0] = GameObject(textures["IceBig"].id, { -.1,0,0 }, { 0,0,0 }, { .5,.55,1 }, 1, 0, 1, 1); // Texture id, loc , rot, sca, mas, maxV, sur, collidertype
	gameObjects[1] = GameObject(textures["IceSmall"].id, { .55,.05,0 }, { 0,0,0 }, { .15,.1,1 }, 1, 0, 1, 1); 
	gameObjects[2] = GameObject(textures["Stone"].id, { -.05,0,0 }, { 0,0,0 }, { .95,.75,1 }, 1, 0, 0, 1); // Surface types are 0=stone, 1=ice, 2=block
	gameObjects[3] = GameObject(textures["StoneTile"].id, { -.35,.1,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1); // Collider types are 0=none, 1=aabb, 2=sphere
	gameObjects[4] = GameObject(textures["StoneTile"].id, { -.25,-.4,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[5] = GameObject(textures["StoneTile"].id, { -.15,.2,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[6] = GameObject(textures["StoneTile"].id, { -.05,0,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[7] = GameObject(textures["StoneTile"].id, { .05,.5,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[8] = GameObject(textures["StoneTile"].id, { .05,-.2,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[9] = GameObject(textures["StoneTile"].id, { .05,-.5,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[10] = GameObject(textures["StoneTile"].id, { .15,.1,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[11] = GameObject(textures["StoneTile"].id, { .25,-.3,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[12] = GameObject(textures["StoneTile"].id, { .35,.3,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[13] = GameObject(textures["StoneTile"].id, { .35,-.5,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[14] = GameObject(textures["Button"].id, { .65,.1,0 }, { 0,0,0 }, { .05,.05,1 }, 1, 0, 0, 1);
	gameObjects[15] = GameObject(textures["Block"].id, { -.551,0,.039 }, { 0,0,0 }, { .049,.049,.049 }, 1, .4f, 2, 1);
	gameObjects[16] = GameObject(textures["Player"].id, { -.9,0,.046 }, { 0,0,0 }, { .049,.049,.049 }, 1, .4f, 0, 2);
	glEnable(GL_CULL_FACE); // No weird faces plz
	glEnable(GL_DEPTH_TEST); // Yay

	for (int i = 0; i < 15; i++) // Initialize game objects (stone tiles and block and button and player)
	{
		if (!gameObjects[i].init("images/quad.obj")) { return false; }
	}
	if (!gameObjects[15].init("images/block.obj")) { return false; }
	if (!gameObjects[16].init("images/player.obj")) { return false; }

	return true; // Return success if no failures
}

bool Engine::gameLoop(void) // Game loop, actually draws to the screen
{
	glfwSetWindowSize(GLFWwindowPtr, 640, 480); // Readjust the window and camera for play
	glViewport(0, 0, 640, 480);
	camera.transform.lPosition = { -.5,-.1,.5 };
	camera.transform.lRotation = { .25,0,0 };
	camera.width = 640;
	camera.height = 480;
	glClearColor(0, 0, 0, 255);
	glfwSetTime(0); // Set up the frame timer
	currFrameT = 0;
	while (!glfwWindowShouldClose(GLFWwindowPtr)) // Loop until user closes window
	{
		prevFrameT = currFrameT;
		currFrameT = glfwGetTime(); // Update the frame timer
		deltaT = currFrameT - prevFrameT;

		/* Input */
		UpdateInput(); // Update input
		glfwPollEvents(); // Process queued window, mouse & keyboard callback events

		if (keyIsDown(GLFW_KEY_ESCAPE) && !keyWasDown(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE); // Exit with escape key
		}

		if (keyIsDown(GLFW_KEY_A)) gameObjects[16].transform.Rotate('z', .002f); // Rotate model CCW or CW depending on key; left-handed
		if (keyIsDown(GLFW_KEY_D)) gameObjects[16].transform.Rotate('z', -.002f);

		if (gameObjects[16].rigidBody.surfaceType == 1) // Handle ice movement
		{
			gameObjects[16].rigidBody.iceFriction(); // Add ice friction
			if (keyIsDown(GLFW_KEY_W)) // When pressing forward, apply thrust in the faced direction while on ice
			{
				gameObjects[16].moveForward((float)deltaT);
			}
			if (gameObjects[16].collider.isOffIce(gameObjects[16].transform))
			{
				gameObjects[16].rigidBody.surfaceType = 0; // If the player leaves the ice, change the surface to reflect that
				if (!keyIsDown(GLFW_KEY_W)) // If player hits stone while sliding
				{
					gameObjects[16].rigidBody.softStop((float)deltaT); // Stop them
				}
			}
		}
		else // Handle stone movement
		{
			if (keyIsDown(GLFW_KEY_W)) // Get player instantly up to speed when move is desired
			{
				gameObjects[16].moveForward((float)deltaT);
			}
			else if (!keyIsDown(GLFW_KEY_W)) // When not moving, bring player to a swift stop
			{
				gameObjects[16].rigidBody.softStop((float)deltaT);
			}

			if ((gameObjects[16].collidesWith(gameObjects[0]) || gameObjects[16].collidesWith(gameObjects[1])))
			{
				gameObjects[16].rigidBody.surfaceType = 1; // If the player is on the stone and hits the ice, move onto it 
			}
			else if (gameObjects[16].collider.isOutOfBounds(gameObjects[16].transform)) // Bounds only need to be checked on stone, as ice is enclosed by it
			{
				gameObjects[16].transform.lPosition -= gameObjects[16].rigidBody.velocity*.01f; // Stop the player from going out of bounds
			}
		}

		/* Collision Check */
		for (int i = 3; i < 15; i++) // Check against all stone tiles and the button
		{
			if (gameObjects[16].collidesWith(gameObjects[i])) // Detect and handle stone collision
			{
				if (!keyIsDown(GLFW_KEY_W)) // If player hits stone while sliding
				{
					gameObjects[16].rigidBody.softStop((float)deltaT); // Stop them
				}
				gameObjects[16].rigidBody.surfaceType = 0; // Surface changes to stone if stone is collided with
			}
		}
		
		if (gameObjects[16].collidesWith(gameObjects[15])) // When block isn't moving, check for player collision with block and handle results
		{
			gameObjects[15].rigidBody.moveForward((float)deltaT, gameObjects[16].transform.forward); // If they collide, send block flying; FUTURE: fix to send block in 1 of 4 directions
			gameObjects[16].transform.lPosition -= gameObjects[16].rigidBody.velocity*.01f; // Stop them from colliding again instantly
			gameObjects[16].rigidBody.fullStop(); // And halt player on edge of block
		}
		for (int i = 3; i < 14; i++) // Check if the block collides with any of the stone tiles
		{
			if (gameObjects[15].collidesWith(gameObjects[i]) && gameObjects[15].rigidBody.velocity != glm::vec3(0,0,0))
			{
				gameObjects[15].rigidBody.fullStop(); // If the block runs into any stone, it stops
			}
		}
		if (gameObjects[15].collider.isOffIce(gameObjects[15].transform) && gameObjects[15].rigidBody.velocity != glm::vec3(0, 0, 0))
		{
			gameObjects[15].rigidBody.fullStop(); // If the block runs into any stone, it stops
		}
		if (gameObjects[15].collidesWith(gameObjects[14])) // On button
		{
			std::cout << "You Win!"; // LOL, realized after testing that this is an infinite loop of this, but I'm gonna leave it in xD
		}

		/* Draw */
		camera.Update(GLFWwindowPtr, (float)deltaT); // Update the camera
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear canvas
		glDisable(GL_DEPTH_TEST); // The depth thing doesn't like the tiles for some reason
		for (int i = 0; i < 15; i++) // Render game objects
		{
			gameObjects[i].Update((float)deltaT);
		}
		glEnable(GL_DEPTH_TEST);
		gameObjects[15].Update((float)deltaT);
		gameObjects[16].Update((float)deltaT);
		glBindVertexArray(0); // Unbind after drawing

		glfwSwapBuffers(GLFWwindowPtr); // Swap display and draw buffers
	}
	return true; // Return success
}

bool Engine::useShaders(void) // Calls for creation of shaders and uses them
{
	if (shaders.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl")) // Make the shaders
	{
		glUseProgram(shaders.getProgram()); // Use the shader program
		return true;
	}
	else
	{
		return false;
	}
}

Engine::~Engine(void)
{
}
