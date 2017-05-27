#ifndef MSERVO_H
#define MSERVO_H

class mServo {
  public: 
    mServo(); // default constructor
    float scan(int minAngle, int maxAngle);
    void setStartAngle(int _angle);
    void setResolution(float _r);
    //Servo servo; 
    float angle; // current angle
    float resolution;
 
  private: 
    bool dir; // scan direction (true=+, false=-)
};

#endif
