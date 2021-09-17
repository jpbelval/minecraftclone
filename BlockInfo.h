#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include "define.h"
#include <string>

class BlockInfo{
    private:
        std::string m_nom;
        BlockType m_type;
        int m_durability;

    public:
        BlockInfo(BlockType type, const std::string& name);
        ~BlockInfo();

        BlockType GetType() const;

        void SetDurability(int durability);
        int GetDurability() const;

        void Show() const;

};

#endif