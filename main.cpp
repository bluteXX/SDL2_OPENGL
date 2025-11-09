#include <SDL2/SDL.h>
#include <iostream>
#include <glad/glad.h>
#include <vector>


//--------------------------------------------------------------
// SHADERS
//--------------------------------------------------------------

const std::string gVertexShaderSource =
"#version 410 core\n"
"layout(location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 1.0);\n"
"}\n";

const std::string gFragmentshaderSource =
"#version 410 core\n"
"out vec4 outColor;\n"
"void main()\n"
"{\n"
"    outColor = vec4(1.0, 0.5, 0.0, 1.0);\n"
"}\n";


//--------------------------------------------------------------
// Global variables
//--------------------------------------------------------------

int width = 480;
int height = 640;

SDL_Window* ApplicationWindow = nullptr;
SDL_GLContext OpenGlContext = nullptr;
bool quit = false;

GLuint glVertexArrayObject = 0;
GLuint glVertexBuforObject = 0;
GLuint gGraphicsPipelineShaderProgram = 0;


//--------------------------------------------------------------
// SHADER UTILS
//--------------------------------------------------------------

GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint shaderObject = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);
    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vs, const std::string& fs)
{
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vs);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);

    glLinkProgram(programObject);
    glValidateProgram(programObject);

    return programObject;
}

void CreateGraphicsPipeline()
{
    gGraphicsPipelineShaderProgram =
        CreateShaderProgram(gVertexShaderSource, gFragmentshaderSource);
}


//--------------------------------------------------------------
// VERTEX SPECIFICATION
//--------------------------------------------------------------

void VertexSpecification()
{
    const std::vector<GLfloat> vertexPosition{
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f
    };

    glGenVertexArrays(1, &glVertexArrayObject);
    glBindVertexArray(glVertexArrayObject);

    glGenBuffers(1, &glVertexBuforObject);
    glBindBuffer(GL_ARRAY_BUFFER, glVertexBuforObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat),
                 vertexPosition.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // layout(location = 0)
        3,                  // vec3
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}


//--------------------------------------------------------------
// INIT
//--------------------------------------------------------------

void InitializeProgram()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Can't initialize SDL video subsystem";
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    ApplicationWindow = SDL_CreateWindow("first window",
                                         0, 0,
                                         width, height,
                                         SDL_WINDOW_OPENGL);

    if (!ApplicationWindow)
    {
        std::cout << "Can't create window";
        exit(1);
    }

    OpenGlContext = SDL_GL_CreateContext(ApplicationWindow);
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Can't initialize GLAD";
        exit(1);
    }
}


//--------------------------------------------------------------
// LOOP
//--------------------------------------------------------------

void Input()
{
    SDL_Event t;

    while (SDL_PollEvent(&t))
    {
        if (t.type == SDL_QUIT)
        {
            quit = true;
        }
    }
}

void PreDraw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, width, height);
    glClearColor(1.f, 1.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw()
{
    glBindVertexArray(glVertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MainLoop()
{
    while (!quit)
    {
        Input();
        PreDraw();
        Draw();
        SDL_GL_SwapWindow(ApplicationWindow);
    }
}


//--------------------------------------------------------------
// CLEAN
//--------------------------------------------------------------

void CleanUp()
{
    SDL_DestroyWindow(ApplicationWindow);
    SDL_Quit();
}


//--------------------------------------------------------------
// MAIN
//--------------------------------------------------------------

int main()
{
    InitializeProgram();
    VertexSpecification();
    CreateGraphicsPipeline();
    MainLoop();
    CleanUp();
}
