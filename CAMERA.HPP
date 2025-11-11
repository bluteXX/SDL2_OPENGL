#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <iostream>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class CAMERA
{
    public:
      
    CAMERA();

    glm::mat4 getViewMatrix() const;

    void MouseLook(int x, int y);
 
    void MoveForward(float delta);
 
    void MoveBackward(float delta);
   
    void MoveRight(float delta);
 
    void MoveLeft(float delta);
 


    private:
    glm::vec3 mEye;
    glm::vec3 mViewDIrection;
    glm::vec3 mUpVector;
    glm::vec2 mLastMousePos;
        
    


};



#endif CAMERA_HPP