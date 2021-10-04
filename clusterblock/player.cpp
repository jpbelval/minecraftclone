#include "player.h"

Player::Player(const Vector3f& position, float rotX, float rotY) : m_RotX(rotX), m_RotY(rotY), m_isFly(false), m_isFalling(false)
{
    m_Position.x = position.x;
    m_Position.y = position.y;
    m_Position.z = position.z;
}


Player::~Player(){

}


void Player::ToggleisFly(){
    if(m_isFly)
        m_isFalling = true;
    else
        m_isFalling = false;
    m_isFly = !m_isFly;
}

void Player::CheckFallState(){
    if(m_isFalling && m_Position.y > 0){
        m_Position.y -= FALLSPEED;

    }
}

void Player::TurnLeftRight(float value){
    m_RotY += value;
}

void Player::TurnTopBottom(float value){
    m_RotX += value;
    if(m_RotX > 90)
        m_RotX = 90;
    if(m_RotX < -90)
        m_RotX = -90;
}

void Player::Move(bool front , bool back , bool left , bool right, float elapsedTime){

    const float vitesse = 10;

    if(front){
        float yrotrad;
        yrotrad = (m_RotY / 180 * 3.141592654f);
        
        m_Position.x += float(sin(yrotrad)) * elapsedTime * vitesse;
        
        m_Position.z -= float(cos(yrotrad))* elapsedTime * vitesse;

        if (m_isFly)
        {
            float xrotrad;
            xrotrad = (m_RotX / 180 * 3.141592654f);
            m_Position.y -= float(sin(xrotrad));
        }
    
    }
    if(back){
        float yrotrad;
        yrotrad = (m_RotY / 180 * 3.141592654f);        
        m_Position.x -= float(sin(yrotrad))* elapsedTime * vitesse;
        m_Position.z += float(cos(yrotrad))* elapsedTime * vitesse;

        if (m_isFly)
        {
            float xrotrad;
            xrotrad = (m_RotX / 180 * 3.141592654f);
            m_Position.y += float(sin(xrotrad))* elapsedTime * vitesse;
        }
    }

    if(right){
        float yrotrad;
        yrotrad = (m_RotY / 180 * 3.141592654f);
        m_Position.x += float(cos(yrotrad)) * elapsedTime * vitesse;
        m_Position.z += float(sin(yrotrad)) * elapsedTime * vitesse;
    }
    if(left){
        float yrotrad;
        yrotrad = (m_RotY / 180 * 3.141592654f);
        m_Position.x -= float(cos(yrotrad)) * elapsedTime * vitesse;
        m_Position.z -= float(sin(yrotrad)) * elapsedTime * vitesse;
    }
}

void Player::ApplyTransformation(Transformation& transformation) const{
    transformation.ApplyRotation(-m_RotX , 1.f, 0, 0);
    transformation.ApplyRotation(-m_RotY , 0, 1.f, 0);
    transformation.ApplyTranslation(-m_Position);
}
