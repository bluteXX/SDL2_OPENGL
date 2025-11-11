#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "CAMERA.HPP"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/matrix_transform.hpp>
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

float g_uOffset = -2.0f;
float g_uRotate = 0.0f;
float g_uScale = 0.5f;


CAMERA gCamera;
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
        2,0,1,
        3,2,1
        
  
         


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
    int mousdeX = width / 2;
            int mouseY = height / 2;
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {

        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            mousdeX = e.motion.x;
            mouseY = e.motion.y;
            gCamera.MouseLook(mousdeX, mouseY);
        }
    }

    g_uRotate += 1.0f;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP])
    {

        gCamera.MoveForward(0.1f);



        //g_uOffset += 0.01f;
        //std::cout << "Offset: " << g_uOffset << std::endl;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        gCamera.MoveBackward(0.1f);
        //g_uOffset -= 0.01f;
        //std::cout << "Offset: " << g_uOffset << std::endl;
    }
    if (state[SDL_SCANCODE_LEFT])
    {
        gCamera.MoveLeft(0.1f);
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        gCamera.MoveRight(0.1f);
    }
    
  
    
    


}


void PreDraw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, width, height);
    glClearColor(1.f, 1.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    //model matrix, make square speaning, rotating and scaling
    glUseProgram(gGraphicsPipelineShaderProgram);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));//We set to zero translation for camera view testing
    model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f, 1.0f, 0.0f));//rotate around y axis 
    model = glm::scale(model, glm::vec3(g_uScale, g_uScale, g_uScale));//uniform scaling
    GLint u_ModelMatrixlocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");

       if(u_ModelMatrixlocation>=0)
    {
        glUniformMatrix4fv(u_ModelMatrixlocation,1,GL_FALSE,&model[0][0]);
    }else
    {
        std::cerr<<"Uniform u_ModelMatrix not found!"<<std::endl;
    }

    //pCamera view matrix
    glm::mat4 view = gCamera.getViewMatrix();

    GLint u_ViewMatrixlocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ViewMatrix");
    if(u_ViewMatrixlocation>=0)
    {
        glUniformMatrix4fv(u_ViewMatrixlocation,1,GL_FALSE,&view[0][0]);
    }else
    {
        std::cerr<<"Uniform u_ViewMatrix not found!"<<std::endl;
    }

    //perspective projection matrix
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),(float)width/(float)height,0.1f,10.0f);
    GLint u_Perspectivelocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_PerspectiveMatrix");
    if(u_Perspectivelocation>=0)
    {
        glUniformMatrix4fv(u_Perspectivelocation,1,GL_FALSE,&perspective[0][0]);
    }else
    {
        std::cerr<<"Uniform u_PerspectiveMatrix not found!"<<std::endl;
    }
   
}

void Draw()
{
    glBindVertexArray(glVertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0);
}

void MainLoop()
{

    SDL_WarpMouseInWindow(ApplicationWindow, width / 2, height / 2);
    SDL_SetRelativeMouseMode(SDL_TRUE);
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
