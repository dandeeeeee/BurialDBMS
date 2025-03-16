#pragma once
#include "raylib.h"


class GUI {
protected:
    float opacity;        
    float fadeSpeed;        
    bool fadingIn;        
    bool fadingOut;        
    bool isVisible;       

public:

    GUI(float initialOpacity = 1.0f) 
        : opacity(initialOpacity),
          fadeSpeed(1.0f), fadingIn(false), 
          fadingOut(false), isVisible(true) {}

    virtual ~GUI() {}


    void fadeIn(float speed = 1.5f) {
        fadeSpeed = speed;
        fadingIn = true;
        fadingOut = false;
        isVisible = true; 
    }


    void fadeOut(float speed = 1.0f) {
        fadeSpeed = speed;
        fadingOut = true;
        fadingIn = false;
    }

    virtual void updateFade(float originalOpacity) {
        float dt = GetFrameTime();
        if (fadingIn) {
            opacity += fadeSpeed * dt;
            if (opacity >= originalOpacity) {
                opacity = originalOpacity;
                fadingIn = false;
            }
        } else if (fadingOut) {
            opacity -= fadeSpeed * dt;
            if (opacity <= 0.0f) {
                opacity = 0.0f;
                fadingOut = false;
                isVisible = false;
            }
        }
    }


    bool isVisibleAndInteractable() const {
        return isVisible && opacity > 0.0f;
    }

    float getOpacity() const { return opacity; }

    void setOpacity(float op) { 
        opacity = op; 
    }
};