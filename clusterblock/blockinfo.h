#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include <string>
#include "define.h"

class BlockInfo
{
    public:
    BlockInfo(BlockType type, const std::string& name);
    ~BlockInfo();

    BlockType GetType() const;

    void SetDurability(int durability);
    int GetDurability() const;

    void SetWHUV(float w, float h, float u, float v);
    float GetW() const;
    float GetH() const;
    float GetU() const;
    float GetV() const;

    void Show() const;

    private:
    BlockType m_type;
    std::string m_name;
    int m_durability;
    float m_height;
    float m_width;
    float m_U;
    float m_V;

};

#endif // BLOCKINFO_H__
