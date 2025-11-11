
#define GLM_ENABLE_EXPERIMENTAL
#include "CAMERA.HPP"
#include <iostream>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp> // glm::lookAt



    CAMERA::CAMERA()
    {
        mEye = glm::vec3(0.0f, 0.0f, 0.0f);
        mViewDIrection = glm::vec3(0.0f, 0.0f, -1.0f);
        mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    }



    glm::mat4 CAMERA::getViewMatrix() const
    {
        return glm::lookAt(mEye,mEye+ mViewDIrection, mUpVector);
    }

    void CAMERA::MoveForward(float delta)
    {
        mEye += (mViewDIrection*delta);
    }

    void CAMERA::MoveBackward(float delta)
    {
      mEye -= (mViewDIrection*delta);
    }
    void CAMERA::MoveRight(float delta)
    {
        glm::vec3 right = glm::cross(mViewDIrection, mUpVector);
        mEye += right * delta;
    }

    void CAMERA::MoveLeft(float delta)
    {
        glm::vec3 right = glm::cross(mViewDIrection, mUpVector);
        mEye -= right * delta;
    }

    void CAMERA::MouseLook(int x, int y)
    {
        std::cout<<"Mouse position: "<<x<<" "<<y<<std::endl;


        glm::vec2 currentMousePos = glm::vec2(x,y);
        glm::vec2 deltaMouse = currentMousePos - mLastMousePos;
        
        float xf = deltaMouse.x * 0.3f;
        float yf = deltaMouse.y * 0.3f;

        
        mViewDIrection  = glm::rotate(mViewDIrection,glm::radians(xf),mUpVector);

        mLastMousePos = currentMousePos;
    }