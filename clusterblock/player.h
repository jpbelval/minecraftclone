#ifndef PLAYER_H__
#define PLAYER_H__

#include "vector3.h"
#include "transformation.h"

class Player
{
private:
    Vector3f m_Position;
    float m_RotX;
    float m_RotY;
    bool m_isFly;
    bool m_isFalling;
    bool m_isJumping;
    float m_fallTime;
    float m_jumpTime;
    float m_jumpHeight;
    float m_speed;

public:
    Player(const Vector3f& position , float rotX = 0, float rotY = 0);
    ~Player();

    void ToggleisFly();
    float CheckFallState(const float &elapsedTime, BlockType bt);
    Vector3f GetPosition() const;
    void SetPosition(Vector3f pos);
    void SetFallTime(float fallTime);
    float GetFallTime() const;
    bool GetIsFalling() const;
    void SetIsFalling(bool isFalling);
    bool GetIsFlying();
    bool GetIsJumping() const;
    void SetIsJumping(bool isJumping);
    void SetMaxHeight();
    void SetSpeed(float speed);
    void Jump();
    void CheckGravity(float elaspedTime);
    void TurnLeftRight(float value);
    void TurnTopBottom(float value);
    void Move(bool front , bool back , bool left , bool right , float elapsedTime, float speed);
    void ApplyTransformation(Transformation& transformation) const;
    Vector3f SimulateMove(bool W, bool S, bool A, bool D, float elaspedTime);
};

#endif