#include "graphics3d.h"
#include "simple_logger.h"
#include "shader.h"

static SDL_GLContext __graphics3d_gl_context;
static SDL_Window  * __graphics3d_window = NULL;
static GLuint        toon_shader;
static GLuint		 phong_shader;
static GLuint        user_interface_shader;
static GLuint		 particle_shader;
static GLuint		xray_shader;
static Uint32        __graphics3d_frame_delay = 33;

void graphics3d_close();

GLuint get_toon_shader()
{
    return toon_shader;
}

GLuint get_phong_shader()
{
	return phong_shader;
}

GLuint get_user_interface_shader()
{
	return user_interface_shader;
}

GLuint get_particle_shader()
{
	return particle_shader;
}

GLuint get_xray_shader()
{
	return xray_shader;
}

void graphics3d_next_frame()
{
    static Uint32 then = 0;
    Uint32 now;
    SDL_GL_SwapWindow(__graphics3d_window);
    now = SDL_GetTicks();
    if ((now - then) < __graphics3d_frame_delay)
    {
        SDL_Delay(__graphics3d_frame_delay - (now - then));        
    }
    then = now;
}

int graphics3d_init(int sw,int sh,int fullscreen,const char *project,Uint32 frameDelay)
{
    const unsigned char *version;
    GLenum glew_status;
        
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        slog("failed to initialize SDL!");
        return -1;
    }
    atexit(SDL_Quit);
    __graphics3d_frame_delay = frameDelay;
    
    __graphics3d_window = SDL_CreateWindow(project?project:"gametest3d",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              sw, sh,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if  (__graphics3d_window == NULL)
	{
		slog("Fatal:Failed to create sdl window");
		return -1;
	}
    
    __graphics3d_gl_context = SDL_GL_CreateContext(__graphics3d_window);
    if (__graphics3d_gl_context == NULL)
    {
        slog("There was an error creating the OpenGL context!\n");
        return -1;
    }
    
    version = glGetString(GL_VERSION);
    if (version == NULL) 
    {
        slog("There was an error creating the OpenGL context!\n");
        return -1;
    }
    
    SDL_GL_MakeCurrent(__graphics3d_window, __graphics3d_gl_context);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    //MUST make a context AND make it current BEFORE glewInit()!
    glewExperimental = GL_TRUE;
    glew_status = glewInit();
    if (glew_status != 0) 
    {
        slog("Error: %s", glewGetErrorString(glew_status));
        return -1;
    }
    
    
    toon_shader = BuildShaderProgram("shaders/toon_shader_vs.glsl", "shaders/toon_shader_fs.glsl");
	phong_shader = BuildShaderProgram("shaders/phong_shader_vs.glsl", "shaders/phong_shader_fs.glsl");
	user_interface_shader = BuildShaderProgram("shaders/user_interface_vs.glsl", "shaders/user_interface_fs.glsl");
	particle_shader = BuildShaderProgram("shaders/particle_shader_vs.glsl", "shaders/particle_shader_fs.glsl");
	xray_shader = BuildShaderProgram("shaders/xray_shader_vs.glsl", "shaders/xray_shader_fs.glsl");

    if (toon_shader == -1)
    {
        return -1;
    }

	if (user_interface_shader == -1)
	{
		return -1;
	}

	if (particle_shader == -1)
	{
		return -1;
	}

	if (xray_shader == -1)
	{
		return -1;
	}

	if (phong_shader == -1)
	{
		return -1;
	}
    
    //slog("Using program %d", toon_shader_color);
        
    atexit(graphics3d_close);
    return 0;
}

SDL_Window* graphics3d_get_window()
{
	return __graphics3d_window;
}

void graphics3d_close()
{
    SDL_GL_DeleteContext(__graphics3d_gl_context);
}


/*eol@eof*/
