#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

class BlockInfo{
    private:

    public:
        BlockInfo(BlockType type, const std::string& name);
        ~BlockInfo();

        BlockType GetType() const;

        void SetDurability(int durability);
        int GetDurability() const;

        void Show() const;

};

#endif