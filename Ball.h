#ifndef PONG_BALL_H
#define PONG_BALL_H

class Paddle;

// Ball dimensions
const int BALL_DIAMETER = 10;

class Ball {
public:
    Ball(int x, int y);
    ~Ball() {}

    // Ball status
    enum Status {READY, LAUNCH, LAUNCHED};
    Status status_;

    // Ball position
    int x_;
    int y_;

    // Ball movement
    int dx_;
    int dy_;
    bool bounce_;
    int speed_;
    float angle_;
    int hits_;
    int predictedY_;

    void launchBall();
    void updateSpeed();
    bool wallCollision();
    bool collidesWith(Paddle* paddle);
    void bouncesOff(Paddle* paddle);
    void reset();
};


#endif //PONG_BALL_H
