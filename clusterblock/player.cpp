#include "player.h"

Player::Player(const Vector3f& position, float rotX, float rotY) : m_RotX(rotX), m_RotY(rotY)
{
    m_Position.x = position.x;
    m_Position.y = position.y;
    m_Position.z = position.z;
}


Player::~Player(){

}

void Player::TurnLeftRight(float value){

}

void Player::TurnTopBottom(float value){

}

void Player::Move(bool front , bool back , bool left , bool right , float elapsedTime){
    if(front){
        float xrotrad, yrotrad;
        yrotrad = (m_RotY / 180 * 3.141592654f);
        xrotrad = (m_RotX / 180 * 3.141592654f);
        m_Position.x += float(sin(yrotrad));
        m_Position.y += float(sin(xrotrad));
        m_Position.z += float(sin(yrotrad));
    }
    else if(back){
        float xrotrad, yrotrad;
        yrotrad = (m_RotY / 180 * 3.141592654f);
        xrotrad = (m_RotX / 180 * 3.141592654f);
        m_Position.x -= float(sin(yrotrad));
        m_Position.z += float(cos(yrotrad)) ;
        m_Position.y += float(sin(xrotrad));
    }
    else if(right){
        float yrotrad;
        yrotrad = (m_RotY / 180 * 3.141592654f);
        m_Position.x += float(cos(yrotrad)) * 0.2;
        m_Position.z += float(sin(yrotrad)) * 0.2;
    }
    else if(right){
        float yrotrad;
        yrotrad = (m_RotY / 180 * 3.141592654f);
        m_Position.x -= float(cos(yrotrad)) * 0.2;
        m_Position.z -= float(sin(yrotrad)) * 0.2;
    }
}

void Player::ApplyTransformation(Transformation& transformation) const{

}
