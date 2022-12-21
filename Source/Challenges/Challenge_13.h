#pragma once

#include "Common/Challenge.h"

#include <queue>

using namespace std;

class CChallenge_13 : public CChallenge
{
    class Leaf;
    class Node;
    class NodeComposite;

private:
    enum class CompareResult
    {
        RightOrder,
        Inconclusive,
        WrongOrder
    };

    enum class NodeType
    {
        Invalid,
        NodeComposite,
        Leaf
    };

    class Node
    {
    public:
        virtual ~Node() {}

        virtual void Print() const = 0;
        virtual void ToString(string& _outString) const = 0;

        virtual CompareResult Compare(const Node* _other) const = 0;

        NodeType GetNodeType() const { return m_NodeType; }

    protected:
        NodeType m_NodeType = NodeType::Invalid;
#ifdef _DEBUG
        string m_DebugString;
#endif
    };

    class NodeComposite : public Node
    {
    public:
        NodeComposite() { m_NodeType = NodeType::NodeComposite; };
        NodeComposite(const Leaf& _other);
        virtual ~NodeComposite();

        virtual void Print() const override;
        virtual void ToString(string& _outString) const override;

        void Build(queue<char>& _packetStr);
        virtual CompareResult Compare(const Node* _other) const override;
        CompareResult Compare(const Leaf* _other) const;
        CompareResult Compare(const NodeComposite* _other) const;

        const vector<Node*>& GetChildren() const { return m_Children; }
        vector<Node*>& GetChildren() { return m_Children; }
        void AddChild(Node* _node) { m_Children.push_back(_node); };

    private:
        vector<Node*> m_Children;
    };

    class Leaf : public Node
    {
    public:
        Leaf() : m_Value(0) { m_NodeType = NodeType::Leaf; }
        Leaf(int _value);

        virtual void Print() const override;
        virtual void ToString(string& _outString) const override;

        virtual CompareResult Compare(const Node* _other) const override;
        CompareResult Compare(const Leaf* _other) const;
        CompareResult Compare(const NodeComposite* _other) const;

        const int& GetValue() const { return m_Value; }
        void SetValue(int _value) { m_Value = _value; }

    private:
        int m_Value = 0;
    };

    class Packet
    {
    public:
        Packet(const string& _packetStr);

        void Build(const string& _packetStr);
        CompareResult Compare(const Packet& _other) const;
        void Print() const;

        bool operator==(const Packet& _other) const;
        bool operator<(const Packet& _other) const;

    private:
        NodeComposite* m_Root;
#ifdef _DEBUG
        string m_DebugString;
#endif
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;


    static string const sm_inputFilePath;

    // Part 1
    typedef pair<Packet, Packet> PacketPair;
    vector<PacketPair> m_PacketPairs;

    // Part 2
    vector<Packet> m_Packets;
};
