#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

//--------------------------------------------------------------
// Global variables
//--------------------------------------------------------------

int width = 480;
int height = 640;

SDL_Window* ApplicationWindow = nullptr;
SDL_GLContext OpenGlContext = nullptr;
bool quit = false;

GLuint glVertexArrayObject = 0;
GLuint glVertexBufferObject = 0;
GLuint glIndexBuferObject2 = 0;
GLuint gGraphicsPipelineShaderProgram = 0;

//--------------------------------------------------------------
// SHADER UTILS
//--------------------------------------------------------------


std::string LoadShaderAsString(const std::string& filename)
{
   std::string result="";
   std::ifstream myfile(filename);


    if (myfile.is_open())
    {
        std::string line;
        while (std::getline(myfile, line))
        {
            result += line + "\n";
        }
        myfile.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }



    return result;


   
}



GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error: " << infoLog << std::endl;
    }

    return shader;
}

GLuint CreateShaderProgram(const std::string& vs, const std::string& fs)
{
    GLuint program = glCreateProgram();
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vs);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void CreateGraphicsPipeline()
{

    std::string vertexShaderCode = LoadShaderAsString("../src/shaders/vert.glsl");
    std::string fragmentShaderCode = LoadShaderAsString("../src/shaders/frag.glsl");


    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderCode, fragmentShaderCode);
}

//--------------------------------------------------------------
// VERTEX SPECIFICATION
//--------------------------------------------------------------

void VertexSpecification()
{
    std::vector<GLfloat> vertexPosition{
        // pos            // color
        
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, //ld vector 0
         0.5f,  -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, //pd vector 1
        -0.5f,   0.5f, 0.0f,   0.0f, .0f, 1.0f, //lu vector 2

         
        0.5f,  0.5f, 0.0f,      1.0f, 0.0f, 0.0f, //pu vector 3
         


    };

    glGenVertexArrays(1, &glVertexArrayObject);
    glBindVertexArray(glVertexArrayObject);

    glGenBuffers(1, &glVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, glVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat),
                 vertexPosition.data(), GL_STATIC_DRAW);

    



    std::vector<GLint> vertexindex{
        0,1,2,
        1,3,2
  
         


    };


    glGenBuffers (1, &glIndexBuferObject2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIndexBuferObject2);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexindex.size() * sizeof(GLfloat),
                 vertexindex.data(), GL_STATIC_DRAW);






    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),       // stride: 6 floatów
        (void*)0                   // offset: 0
    );

    // Atrybut 1: kolor
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),       // stride: 6 floatów
        (void*)(3 * sizeof(GLfloat)) // offset: pozycja ma 3 floaty
    );

    glBindVertexArray(0);
}

//--------------------------------------------------------------
// INIT
//--------------------------------------------------------------

void InitializeProgram()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Can't initialize SDL video subsystem" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    ApplicationWindow = SDL_CreateWindow("SDL2 OpenGL Window",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         width, height,
                                         SDL_WINDOW_OPENGL);

    if (!ApplicationWindow)
    {
        std::cerr << "Can't create window" << std::endl;
        exit(1);
    }

    OpenGlContext = SDL_GL_CreateContext(ApplicationWindow);
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cerr << "Can't initialize GLAD" << std::endl;
        exit(1);
    }
}

//--------------------------------------------------------------
// LOOP
//--------------------------------------------------------------

void Input()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            quit = true;
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
    glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0);
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
    SDL_GL_DeleteContext(OpenGlContext);
    SDL_DestroyWindow(ApplicationWindow);
    SDL_Quit();
}

//--------------------------------------------------------------
// MAIN
//--------------------------------------------------------------

int SDL_main(int argc, char* argv[])
{
    InitializeProgram();
    VertexSpecification();
    CreateGraphicsPipeline();
    MainLoop();
    CleanUp();
    return 0;
}
