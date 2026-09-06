class Mouse {

    float lastX,lastY; 
    float xOffset,yOffset; 
    float sensitivity = 1.0;
    bool firstMouse = true; 
    public:
    void updateMouse(float x, float y) {
        if(firstMouse) {
            lastX = x;
            lastY = y; 
            firstMouse = false;
        }
        xOffset = (x-lastX)*sensitivity;
        yOffset = (lastY-y)*sensitivity; 
        lastX = x;
        lastY = y;
    }

    void setSensitivity(float newSensitity) {
        sensitivity = newSensitity;
    }

    void setSensitivityRelativly(float change) {
        sensitivity += sensitivity*change;
    }

    float getOffsetX() {
        float temp = xOffset;
        xOffset = 0.0f;
        return temp;
    }
    float getOffsetY() {
        float temp = yOffset;
        yOffset = 0.0f;
        return temp;
    }

};