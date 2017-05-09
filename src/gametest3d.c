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
	GLuint state = 2; //Main menu
	NavMesh* navmesh = createNavmesh("C:/Users/sharg_000/Documents/3D_Modeling_Projects/navMesh.obj");

	ResourceManager* manager = createScene();

	bool flyMode = false;
	GLfloat flyPressTime = 0.0f;

	bool toonMode = true;
	GLfloat toonPressTime = 0.0f;

	Camera* camera = createCamera(glm::vec3(0.0, 0.0, 0.0), 0.1f, 100.0f);

	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	GLint width = 800;
	GLint height = 600;

	GLint mouseX, mouseY;
	GLint mousePosX, mousePosY;

	rotateCamera(camera, 0.0f, 0.0f);
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

	createFramebuffer(manager);

	loadScene(manager, "C:/Users/sharg_000/Documents/IT485_3DGameLevel.txt");

	initScene(manager);

	UICanvas* hud = createUI();
	UIElement* icon = createUIElement(200, 20, glm::vec2(80.0f, 10.0f), false);
	setText(icon, "");
	setTextColor(icon, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	addUIElement(hud, icon);

	UICanvas* mainMenu = createUI();
	UIElement* continueButton = createUIElement(200, 30, glm::vec2(40.0f, 350.0f), true);
	setText(continueButton, "Continue");
	setTextColor(continueButton, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	addUIElement(mainMenu, continueButton);

	UIElement* settingsButton = createUIElement(200, 30, glm::vec2(40.0f, 300.0f), true);
	setText(settingsButton, "Settings");
	setTextColor(settingsButton, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	addUIElement(mainMenu, settingsButton);

	UIElement* quitButton = createUIElement(200, 30, glm::vec2(40.0f, 250.0f), true);
	setText(quitButton, "Quit");
	setTextColor(quitButton, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	addUIElement(mainMenu, quitButton);

	glm::mat4 proj = glm::perspective(glm::radians(60.0f), (GLfloat)(width / height), camera->nearPlane, camera->farPlane);
	glm::mat4 orthoProj = glm::ortho(0.0f, (GLfloat)width, 0.0f, (GLfloat)height, 1.0f, 100.0f);
	glm::mat4 view;
	GLint uniProj;
	GLint uniCam;
	GLint uniView;
	GLint x = 0;
	GLint y = 0;
	glutInit(&argc, argv);

	glm::vec3* lightPositions = (glm::vec3*)malloc(sizeof(glm::vec3) * manager->numLights);
	glm::vec3* lightColors = (glm::vec3*)malloc(sizeof(glm::vec3) * manager->numLights);
	glm::vec3* lightDirections = (glm::vec3*)malloc(sizeof(glm::vec3)*manager->numLights);
	GLfloat* lightRanges = (GLfloat*)malloc(sizeof(GLfloat) * manager->numLights);
	GLfloat* lightIntensities = (GLfloat*)malloc(sizeof(GLfloat) * manager->numLights);


	while (bGameLoopRunning)
	{
		if (state == 1)
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}

		Uint8 mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);
		Uint8 mousePositions = SDL_GetMouseState(&mousePosX, &mousePosY);

		if (mainMenu->uiElements[0].isHighlighted == true && (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseState))
		{
			state = 1;
		}
		else if (mainMenu->uiElements[1].isHighlighted == true && (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseState))
		{

		}
		else if (mainMenu->uiElements[2].isHighlighted == true && (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseState))
		{
			state = 0;
		}
		if (keystate[SDL_SCANCODE_ESCAPE] || state == 0)
		{
			bGameLoopRunning = 0;
		}

		//updateUI(hud);
		updateUI(mainMenu);

		if (state == 1)
		{
			updateScene(manager);
			if (manager->player->gameObject->health > 0)
			{
				hud->uiElements[0].width = manager->player->gameObject->health;
			}
			else
			{
				hud->uiElements[0].width = 0;
			}
			//Rotate camera using mouse
			rotateCamera(camera, mouseX*0.2f, mouseY*-0.2f);

			const Uint8 *keystate = SDL_GetKeyboardState(NULL);

			flyPressTime++;
			if (keystate[SDL_SCANCODE_Q] && flyPressTime >= 2.0f)
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
				camera->position = glm::vec3(manager->player->gameObject->position.x, 
					manager->player->gameObject->position.y + 1.5f, manager->player->gameObject->position.z);
			}
			//manager->player->gameObject->forward = camera->forward;
			//manager->player->gameObject->right = camera->right;
			////camera->position = manager->player->gameObject->position;
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

			if (keystate[SDL_SCANCODE_4])
			{
				for (GLuint i = 0; i < manager->numLights; i++)
				{
					moveEntity(manager->lights[i]->gameObject, glm::vec3(0.5f, 0.0f, 0.0f));
				}
			}
			else if (keystate[SDL_SCANCODE_6])
			{
				for (GLuint i = 0; i < manager->numLights; i++)
				{
					moveEntity(manager->lights[i]->gameObject, glm::vec3(-0.5f, 0.0f, 0.0f));
				}
			}
			else if (keystate[SDL_SCANCODE_5])
			{
				for (GLuint i = 0; i < manager->numLights; i++)
				{
					moveEntity(manager->lights[i]->gameObject, glm::vec3(0.0f, 0.5f, 0.0f));
				}
			}
			else if (keystate[SDL_SCANCODE_3])
			{
				for (GLuint i = 0; i < manager->numLights; i++)
				{
					moveEntity(manager->lights[i]->gameObject, glm::vec3(0.0f, -0.5f, 0.0f));
				}
			}
			for (GLuint i = 0; i < manager->numLights; i++)
			{
				lightPositions[i] = manager->lights[i]->gameObject->position;
				lightColors[i] = manager->lights[i]->color;
				lightIntensities[i] = manager->lights[i]->intensity;
				lightDirections[i] = manager->lights[i]->direction;
				lightRanges[i] = manager->lights[i]->range;
			}

			if (manager->player->gameObject->health <= 0)
			{
				UICanvas* loseScreen = createUI();
				UIElement* lost = createUIElement(0, 0, glm::vec2(400.0f, 50.0f), false);
				setText(lost, "You Lose!");
				setTextColor(lost, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				addUIElement(loseScreen, lost);
			}

			if (manager->numEnemies <= 0)
			{
				UICanvas* winScreen = createUI();
				UIElement* win = createUIElement(0, 0, glm::vec2(400.0f, 50.0f), false);
				setText(win, "You Win!");
				setTextColor(win, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				addUIElement(winScreen, win);
			}
			//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			if (toonMode)
			{
				//Toon Shader
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

				GLint uniLightDirections = glGetUniformLocation(get_toon_shader(), "lightDirections");
				glUniform3fv(uniLightDirections, manager->numLights, (const GLfloat*)lightDirections);

				GLint uniLightRanges = glGetUniformLocation(get_toon_shader(), "lightRanges");
				glUniform1fv(uniLightRanges, manager->numLights, (const GLfloat*)lightRanges);

				view = glm::lookAt(camera->position, camera->position + camera->forward, camera->worldUp);
				uniView = glGetUniformLocation(get_toon_shader(), "view");
				glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

				uniProj = glGetUniformLocation(get_toon_shader(), "proj");
				glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

				uniCam = glGetUniformLocation(get_toon_shader(), "camPos");
				glUniform3f(uniCam, camera->position.x, camera->position.y, camera->position.z);

				GLuint uniBaseColor = glGetUniformLocation(get_toon_shader(), "baseColor");
				glUniform1i(uniBaseColor, 0);

				renderToonScene(manager);

			}
			else
			{
				//Phong Shader
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glUseProgram(get_phong_shader());
				glEnable(GL_DEPTH_TEST);
				glDisable(GL_BLEND);
				glCullFace(GL_BACK);

				GLuint uniNumLights = glGetUniformLocation(get_phong_shader(), "numLights");
				glUniform1i(uniNumLights, manager->numLights);

				GLuint uniLightPositions = glGetUniformLocation(get_phong_shader(), "lightPositions");
				glUniform3fv(uniLightPositions, manager->numLights, (const GLfloat*)lightPositions);

				GLuint uniLightColors = glGetUniformLocation(get_phong_shader(), "lightColors");
				glUniform3fv(uniLightColors, manager->numLights, (const GLfloat*)lightColors);

				GLuint uniLightIntensities = glGetUniformLocation(get_phong_shader(), "lightIntensities");
				glUniform1fv(uniLightIntensities, manager->numLights, (const GLfloat*)lightIntensities);

				GLint uniLightDirections = glGetUniformLocation(get_phong_shader(), "lightDirections");
				glUniform3fv(uniLightDirections, manager->numLights, (const GLfloat*)lightDirections);

				GLint uniLightRanges = glGetUniformLocation(get_phong_shader(), "lightRanges");
				glUniform1fv(uniLightRanges, manager->numLights, (const GLfloat*)lightRanges);

				view = glm::lookAt(camera->position, camera->position + camera->forward, camera->worldUp);
				uniView = glGetUniformLocation(get_phong_shader(), "view");
				glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

				uniProj = glGetUniformLocation(get_phong_shader(), "proj");
				glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

				uniCam = glGetUniformLocation(get_phong_shader(), "camPos");
				glUniform3f(uniCam, camera->position.x, camera->position.y, camera->position.z);

				renderScene(manager);
			}

			//if (manager->numSpawners > 0)
			//{
			//	//Particle Shader
			//	glUseProgram(get_particle_shader());
			//	uniCam = glGetUniformLocation(get_particle_shader(), "camUp");
			//	glUniform3f(uniCam, camera->up.x, camera->up.y, camera->up.z);

			//	uniCam = glGetUniformLocation(get_particle_shader(), "camPos");
			//	glUniform3f(uniCam, camera->position.x, camera->position.y, camera->position.z);

			//	view = glm::lookAt(camera->position, camera->position + camera->forward, camera->worldUp);
			//	uniView = glGetUniformLocation(get_particle_shader(), "view");
			//	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

			//	proj = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, camera->nearPlane, camera->farPlane);
			//	uniProj = glGetUniformLocation(get_particle_shader(), "proj");
			//	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
			//}

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
		}

		if (state == 2)
		{
			//User Interface Shader
			glUseProgram(get_user_interface_shader());
			view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), camera->worldUp);
			uniView = glGetUniformLocation(get_user_interface_shader(), "view");
			glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

			uniProj = glGetUniformLocation(get_user_interface_shader(), "ortho");
			glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(orthoProj));

			//drawUI(hud);
			drawUI(mainMenu);
		}

		if (state == 1)
		{
			glUseProgram(get_user_interface_shader());
			view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), camera->worldUp);
			uniView = glGetUniformLocation(get_user_interface_shader(), "view");
			glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

			uniProj = glGetUniformLocation(get_user_interface_shader(), "ortho");
			glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(orthoProj));

			//drawUI(hud);
			drawUI(hud);

			if (toonMode)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, manager->fbo);
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glUseProgram(get_toon_shader());
				glEnable(GL_DEPTH_TEST);
				glDisable(GL_BLEND);

				view = glm::lookAt(camera->position, camera->position + camera->forward, camera->worldUp);
				GLuint uniView = glGetUniformLocation(get_toon_shader(), "view");
				glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

				GLuint uniProj = glGetUniformLocation(get_toon_shader(), "proj");
				glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

				GLuint uniCam = glGetUniformLocation(get_toon_shader(), "camPos");
				glUniform3f(uniCam, camera->position.x, camera->position.y, camera->position.z);

				GLuint uniBaseColor = glGetUniformLocation(get_toon_shader(), "baseColor");
				glUniform1i(uniBaseColor, 1);

				renderToonScene(manager);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//glDisable(GL_BLEND);

				glUseProgram(get_post_process_shader());
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				renderPostProcessEffects(manager);
			}
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glUseProgram(0);

		/* drawing code above here! */
		graphics3d_next_frame();
		if (state == 1)
		{
			SDL_WarpMouseInWindow(graphics3d_get_window(), width / 2, height / 2);
		}
		SDL_PumpEvents();
	}
	return 0;
}

/*eol@eof*/