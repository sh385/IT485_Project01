/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include <stdio.h>
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "user_interface.h"
#include "resource_manager.h"
#include "particle_spawner.h"
#include "ai.h"

int main(int argc, char *argv[])
{
	NavMesh* navmesh = createNavmesh("C:/Users/sharg_000/Documents/3D_Modeling_Projects/navMesh.obj");
	AI* ai = createAI(navmesh);
	ParticleSpawner* particleSpawner = createParticles();
	HUD* hud = createHUD();
	ResourceManager* manager = createScene();
	bool updateParts = false;
	bool flyMode = false;
	GLfloat flyPressTime = 0.0f;

	bool toonMode = false;
	GLfloat toonPressTime = 0.0f;

	Camera* camera = createCamera(glm::vec3(0.0, 0.0, 0.0), 0.1f, 100.0f);
	
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	GLint width = 800;
	GLint height = 600;

	GLint mouseX, mouseY;

	rotateCamera(camera, -90.0f, 0.0f);
	char bGameLoopRunning = 1;
	SDL_Event e;

	bool xrayMode = false;
	float xrayPressTime = 0.0f;

	init_logger("gametest3d.log");
	if (graphics3d_init(width, height, 1, "gametest3d", 33) != 0)
	{
		return -1;
	}
	slog("glError: %d", glGetError());

	glEnable(GL_DEPTH_TEST);

	initHUD(hud);
	loadScene(manager, "C:/Users/sharg_000/Documents/blahblahblah.txt");
	initScene(manager);
	initParticles(particleSpawner);

	glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, camera->nearPlane, camera->farPlane);
	glm::mat4 orthoProj = glm::ortho(0.0f, (GLfloat)width, 0.0f, (GLfloat)height, 1.0f, 100.0f);
	glm::mat4 view;
	GLint uniProj;
	GLint uniCam;
	GLint uniView;
	GLint uniKernel;
	glutInit(&argc, argv);
	
	glm::vec3* lightPositions = (glm::vec3*)malloc(sizeof(glm::vec3) * manager->numLights);
	glm::vec3* lightColors = (glm::vec3*)malloc(sizeof(glm::vec3) * manager->numLights);
	GLfloat* lightIntensities = (GLfloat*)malloc(sizeof(GLfloat) * manager->numLights);
	for (GLuint i = 0; i < manager->numLights; i++)
	{
		lightPositions[i] = manager->lights[i]->gameObject->position;
		lightColors[i] = manager->lights[i]->color;
		lightIntensities[i] = manager->lights[i]->intensity;
	}

	while (bGameLoopRunning)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_PumpEvents();
		Uint8 mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);
		updateScene(manager);
		//NavNode* aiPath = findPath(ai, manager->player->gameObject);
		////if (ai->pathSize != 0)
		////{
		//	//printf("yes");
		//	for (GLuint i = 0; i < ai->pathSize; i++)
		//	{
		//		Entity* ent = createEntity();
		//		ent->model = loadModel("C:/Users/sharg_000/Documents/3D_Modeling_Projects/cubeTest.obj");
		//		loadTexture(&ent->model->meshes[0], "C:/Users/sharg_000/Documents/tileable_checkered_floor.jpg");
		//		moveEntity(ent, glm::vec3(aiPath[i].position.x, aiPath[i].position.y, aiPath[i].position.z));
		//		addEntity(manager, ent);
		//	}
		////}
		if (updateParts)
		{
			follow(particleSpawner, manager->entityList[0]->position);
		}
		if (keystate[SDL_SCANCODE_ESCAPE])
		{
			bGameLoopRunning = 0;
		}
		//Rotate camera using mouse
		rotateCamera(camera, mouseX*0.2f, mouseY*-0.2f);

		if (keystate[SDL_SCANCODE_L])
		{
			updateParts = true;
		}

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);

		flyPressTime++;
		if (keystate[SDL_SCANCODE_Q] && flyPressTime >= 5.0f)
		{
			if (flyMode)
			{
				flyMode = false;
			}
			else
			{
				flyMode = true;
			}
			flyPressTime = 0.0f;
		}
		if (flyMode)
		{
			if (keystate[SDL_SCANCODE_W]) //Move camera forward if the W key is pressed
			{
				moveCameraForward(camera, 1.0f);
			}
			else if (keystate[SDL_SCANCODE_S]) //Move camera backward if the W key is pressed
			{
				moveCameraBackward(camera, 1.0f);
			}

			if (keystate[SDL_SCANCODE_A]) //Move camera left if the W key is pressed
			{
				moveCameraLeft(camera, 1.0f);
			}
			else if (keystate[SDL_SCANCODE_D]) //Move camera right if the W key is pressed
			{
				moveCameraRight(camera, 1.0f);
			}
		}
		else
		{
			manager->player->gameObject->forward = camera->forward;
			manager->player->gameObject->right = camera->right;
			camera->position = manager->player->gameObject->position;
		}
		xrayPressTime++;
		if (keystate[SDL_SCANCODE_X] && xrayPressTime >= 5.0f)
		{
			if (xrayMode)
			{
				xrayMode = false;
			}
			else
			{
				xrayMode = true;
			}
			xrayPressTime = 0.0f;
		}
		toonPressTime++;
		if (keystate[SDL_SCANCODE_T] && toonPressTime >= 5.0f)
		{
			if (toonMode)
			{
				toonMode = false;
			}
			else
			{
				toonMode = true;
			}
			toonPressTime = 0.0f;
		}

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		if (toonMode)
		{
			//Toon Shader
			glUseProgram(get_toon_shader());
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			glCullFace(GL_BACK);

			GLint uniNumLights = glGetUniformLocation(get_toon_shader(), "numLights");
			glUniform1i(uniNumLights, manager->numLights);

			GLint uniLightPositions = glGetUniformLocation(get_toon_shader(), "lightPositions");
			glUniform3fv(uniLightPositions, manager->numLights, (const GLfloat*)lightPositions);

			GLint uniLightColors = glGetUniformLocation(get_toon_shader(), "lightColors");
			glUniform3fv(uniLightColors, manager->numLights, (const GLfloat*)lightColors);

			GLint uniLightIntensities = glGetUniformLocation(get_toon_shader(), "lightIntensities");
			glUniform1fv(uniLightIntensities, manager->numLights, (const GLfloat*)lightIntensities);

			view = glm::lookAt(camera->position, camera->position + camera->forward, camera->worldUp);
			uniView = glGetUniformLocation(get_toon_shader(), "view");
			glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

			uniProj = glGetUniformLocation(get_toon_shader(), "proj");
			glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

			uniCam = glGetUniformLocation(get_toon_shader(), "camPos");
			glUniform3f(uniCam, camera->position.x, camera->position.y, camera->position.z);

			renderToonScene(manager);
		} 
		else
		{
			//Phong Shader
			glUseProgram(get_phong_shader());
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			glCullFace(GL_BACK);

			GLint uniNumLights = glGetUniformLocation(get_phong_shader(), "numLights");
			glUniform1i(uniNumLights, manager->numLights);

			GLint uniLightPositions = glGetUniformLocation(get_phong_shader(), "lightPositions");
			glUniform3fv(uniLightPositions, manager->numLights, (const GLfloat*)lightPositions);

			GLint uniLightColors = glGetUniformLocation(get_phong_shader(), "lightColors");
			glUniform3fv(uniLightColors, manager->numLights, (const GLfloat*)lightColors);

			GLint uniLightIntensities = glGetUniformLocation(get_phong_shader(), "lightIntensities");
			glUniform1fv(uniLightIntensities, manager->numLights, lightIntensities);

			view = glm::lookAt(camera->position, camera->position + camera->forward, camera->worldUp);
			uniView = glGetUniformLocation(get_phong_shader(), "view");
			glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

			uniProj = glGetUniformLocation(get_phong_shader(), "proj");
			glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

			uniCam = glGetUniformLocation(get_phong_shader(), "camPos");
			glUniform3f(uniCam, camera->position.x, camera->position.y, camera->position.z);

			renderScene(manager);
		}

		//Particle Shader
		glUseProgram(get_particle_shader());
		uniCam = glGetUniformLocation(get_particle_shader(), "camUp");
		glUniform3f(uniCam, camera->up.x, camera->up.y, camera->up.z);

		uniCam = glGetUniformLocation(get_particle_shader(), "camPos");
		glUniform3f(uniCam, camera->position.x, camera->position.y, camera->position.z);

		view = glm::lookAt(camera->position, camera->position + camera->forward, camera->worldUp);
		uniView = glGetUniformLocation(get_particle_shader(), "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		proj = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, camera->nearPlane, camera->farPlane);
		uniProj = glGetUniformLocation(get_particle_shader(), "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		drawParticles(particleSpawner);

		//X-Ray Shader
		if (xrayMode)
		{
			glUseProgram(get_xray_shader());
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			uniCam = glGetUniformLocation(get_xray_shader(), "camDir");
			glUniform3f(uniCam, camera->forward.x, camera->forward.y, camera->forward.z);

			view = glm::lookAt(camera->position, camera->position + camera->forward, camera->worldUp);
			uniView = glGetUniformLocation(get_xray_shader(), "view");
			glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

			uniProj = glGetUniformLocation(get_xray_shader(), "proj");
			glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

			renderXrayScene(manager);
		}

		//User Interface Shader
		glUseProgram(get_user_interface_shader());
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), camera->worldUp);
		uniView = glGetUniformLocation(get_user_interface_shader(), "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		uniProj = glGetUniformLocation(get_user_interface_shader(), "ortho");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(orthoProj));

		drawHUD(hud);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glUseProgram(0);

		/* drawing code above here! */
		graphics3d_next_frame();
		SDL_WarpMouseInWindow(graphics3d_get_window(), width / 2, height / 2);
	}
	return 0;
}

/*eol@eof*/