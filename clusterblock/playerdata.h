#ifndef PLAYERDATA_H__
#define PLAYERDATA_H__

#include "vector3.h"

class PlayerData
{
    private:
        int m_id;
        Vector3f m_Position;
    public:
        PlayerData(int id);
        ~PlayerData();

        void SetId(int id);
        int GetId();
        void SetPosition(Vector3f position);
        Vector3f GetPosition();
};

#endif