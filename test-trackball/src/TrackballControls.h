#pragma once

/*
 * The MIT License
 *
 * Copyright (c) 2014-2015 Michael Ivanov
 * Copyright (c) 2010-2011 three.js Authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Initial @author Eberhard Graether / http://egraether.com/
// C++ port by Michael Ivanov (sasmaster) 
// https://github.com/sasmaster/TrackballControls

#ifndef TRACKBALL_CONTROLS_H
#define TRACKBALL_CONTROLS_H

#include <glm/glm.hpp>
#include <stdint.h>

struct GLFWwindow;

namespace sasmaster{

class Camera3D {
    public:
        glm::vec3 m_eye, m_up;
        glm::mat4 m_viewMatr;

        Camera3D(const glm::vec3& pos);

        void LookAt(const glm::vec3& target);
};

    /**
    * The class is a singleton.
    */
class TrackballControls {
    public:
        /**
        * @param cam = pointer to active camera object.
        * @param screenSize - size of the window screen.
        */
        static TrackballControls& GetInstance(Camera3D* cam,glm::vec4 screenSize);

        void Init(GLFWwindow *win);
        void Update();
        void MouseUp();
        void KeyUp();
        void MouseDown(int button, int action, int mods,int xpos,int ypos);
        void MouseMove(int xpos, int ypos);
        void KeyDown(int key);
        void MouseWheel(double xoffset ,double yoffset);

        float m_rotateSpeed;
        float m_zoomSpeed;
        float m_panSpeed;
        float m_dynamicDampingFactor;
        float m_minDistance;
        float m_maxDistance;
        bool m_enabled;
        bool m_noRotate;
        bool m_noZoom;
        bool m_noPan;
        bool m_noRoll;
        bool m_staticMoving;

    private:
        TrackballControls(Camera3D* cam, glm::vec4 screenSize);

        glm::vec3 GetMouseProjectionOnBall(int clientX, int clientY);
        glm::vec2 GetMouseOnScreen(int clientX, int clientY);
        void  RotateCamera();
        void ZoomCamera();
        void PanCamera();
        void CheckDistances();

        enum class TCB_STATE:uint8_t{
            NONE,
            ROTATE,
            ZOOM,
            PAN
        };

        Camera3D* m_pCam;
        glm::vec4 m_screen;

        glm::vec3 m_target;
        glm::vec3 m_lastPos;
        glm::vec3 m_eye;
        glm::vec3 m_rotStart;
        glm::vec3 m_rotEnd;
        glm::vec2 m_zoomStart;
        glm::vec2 m_zoomEnd;
        glm::vec2 m_panStart;
        glm::vec2 m_panEnd;
        TCB_STATE m_state;
        TCB_STATE m_prevState;

        static uint32_t m_keys[3];
    };

    inline glm::vec2 TrackballControls::GetMouseOnScreen(int clientX, int clientY) {
        return glm::vec2(
            (float)(clientX - m_screen.x) / m_screen.z,
            (float)(clientY - m_screen.y) / m_screen.w
            );
    }


    inline void TrackballControls::MouseUp() {
        if (!m_enabled) return;
        m_state = TCB_STATE::NONE;
    }

    inline void TrackballControls::KeyUp() {
        if (!m_enabled) { return; }
        m_state = m_prevState;
    }
}

#endif // TRACKBALL_CONTROLS_H    

