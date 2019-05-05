//
//  Camera.h
//  GLtest
//
//  Created by 朱俊凯 on 2019/4/14.
//  Copyright © 2019年 朱俊凯. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


class Camera{
public:
    GLfloat pfov,pratio,pnear,pfar,Yaw,Pitch;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 WorldUp;
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f){
        cameraPos = position;
        WorldUp=up;
        Yaw=yaw;
        Pitch=pitch;
        updateCameraVectors();
    }
    void moveForward(GLfloat const distance){
        cameraPos += cameraFront * distance;
    }
    void moveBack(GLfloat const distance){
        cameraPos -= cameraFront * distance;
    }
    void moveRight(GLfloat const distance){
        cameraPos += cameraRight * distance;
    }
    void moveLeft(GLfloat const distance){
        cameraPos -= cameraRight * distance;
    }
    void rotate(GLfloat const pitch, GLfloat const yaw){
        Yaw   += yaw;
        Pitch += pitch;
        updateCameraVectors();
    }
    void updateCameraVectors(){
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        cameraFront = glm::normalize(front);
        cameraRight = glm::normalize(glm::cross(cameraFront, WorldUp));  // Normalize the vectors, because their length
        cameraUp    = glm::normalize(glm::cross(cameraRight, cameraFront));
    }
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
};


#endif /* Camera_h */
