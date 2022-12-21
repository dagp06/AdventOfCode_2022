#include "Challenge_13.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_13::sm_inputFilePath = "Inputs/Input_Challenge_13.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_13::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (int i = 0; i < lines.size(); i += 3)
    {
        m_PacketPairs.push_back(PacketPair(Packet(lines[i]), Packet(lines[i + 1])));
    }

#ifdef _DEBUG
    for (const PacketPair& packetPair : m_PacketPairs)
    {
        packetPair.first.Print();
        cout << endl;
        packetPair.second.Print();
        cout << endl << endl;
    }
#endif

    return EErrorCode::Success;
}

EErrorCode CChallenge_13::Run_FirstPart()
{
    int packetPairsInRightOrder = 0;
    for (int i = 0; i < m_PacketPairs.size(); ++i)
    {
        const PacketPair& packetPair = m_PacketPairs[i];
        if (packetPair.first.Compare(packetPair.second) == CompareResult::RightOrder)
            packetPairsInRightOrder += i + 1;
    }

#ifndef BENCHMARK
    cout << "The sum of the indices of packets that are in the right order is " << packetPairsInRightOrder << "." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_13::CleanUp_FirstPart()
{
    m_PacketPairs.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_13::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines[i].empty())
            continue;

        m_Packets.push_back(Packet(lines[i]));
    }

    m_Packets.push_back(Packet("[[2]]"));
    m_Packets.push_back(Packet("[[6]]"));
    
    return EErrorCode::Success;
}

EErrorCode CChallenge_13::Run_SecondPart()
{
    sort(m_Packets.begin(), m_Packets.end());

    int firstIdx = (int)(find(m_Packets.begin(), m_Packets.end(), Packet("[[2]]")) - m_Packets.begin()) + 1;
    int secondIdx = (int)(find(m_Packets.begin(), m_Packets.end(), Packet("[[6]]")) - m_Packets.begin()) + 1;

    int decoderKey = firstIdx * secondIdx;

#ifdef _DEBUG
    cout << "Packet list after sorting :" << endl;
    for (int i = 0; i < m_Packets.size(); ++i)
    {
        cout << i + 1 << " - ";
        m_Packets[i].Print();
        cout << endl;
    }

    cout << endl;
    cout << "[[2]]'s index : " << firstIdx << endl;
    cout << "[[6]]'s index : " << secondIdx << endl;
    cout << endl;
#endif

#ifndef BENCHMARK
    cout << "The decoder key for the distress signal is " << decoderKey << "." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_13::CleanUp_SecondPart()
{
    m_Packets.clear();
    return EErrorCode::Success;
}

CChallenge_13::NodeComposite::NodeComposite(const Leaf& _other)
{
    m_Children.push_back(new Leaf(_other.GetValue())); 
    m_NodeType = NodeType::NodeComposite;

#ifdef _DEBUG
    ToString(m_DebugString);
#endif // _DEBUG
}

CChallenge_13::NodeComposite::~NodeComposite()
{
    for (Node* child : m_Children)
    {
        delete child;
    }

    m_Children.clear();
}

void CChallenge_13::NodeComposite::Print() const
{
    cout << "[";

    string separator = "";
    for (const Node* child : m_Children)
    {
        cout << separator;

        child->Print();

        separator = ",";
    }

    cout << "]";
}


void CChallenge_13::NodeComposite::ToString(string& _outString) const
{
    _outString += "[";

    string separator = "";
    for (const Node* child : m_Children)
    {
        _outString += separator;

        child->ToString(_outString);

        separator = ",";
    }

    _outString += "]";
}

void CChallenge_13::NodeComposite::Build(queue<char>& _packetStr)
{
    char currentChar = _packetStr.front();

    while (currentChar != ']')
    {
        if (currentChar == ',')
        {
            _packetStr.pop();
            currentChar = _packetStr.front();
        }

        if (currentChar == '[')
        {
            _packetStr.pop();

            NodeComposite* newChildComposite = new NodeComposite();
            newChildComposite->Build(_packetStr);
            m_Children.push_back(newChildComposite);

            currentChar = _packetStr.front();
        }
        else if (isdigit(currentChar))
        {
            string number;
            do
            {
                number.push_back(currentChar);
                _packetStr.pop();
                currentChar = _packetStr.front();
            } while (isdigit(currentChar));

            Leaf* newLeaf = new Leaf(stoi(number));
            m_Children.push_back(newLeaf);
        }
    }

    _packetStr.pop();

#ifdef _DEBUG
    ToString(m_DebugString);
#endif // _DEBUG
}


CChallenge_13::CompareResult CChallenge_13::NodeComposite::Compare(const NodeComposite* _other) const
{
    vector<Node*>::const_iterator _leftIt = m_Children.begin();
    vector<Node*>::const_iterator _rightIt = _other->GetChildren().begin();

    CompareResult result = CompareResult::Inconclusive;
    while (result == CompareResult::Inconclusive && _leftIt != m_Children.end() && _rightIt != _other->GetChildren().end())
    {
        const Node* leftNode = *_leftIt;
        const Node* rightNode = *_rightIt;
        result = leftNode->Compare(rightNode);

        ++_leftIt;
        ++_rightIt;
    }

    if (result != CompareResult::Inconclusive)
    {
        return result;
    }

    // Left still has numbers to process, right doesn't. Wrong order.
    if (_leftIt != m_Children.end())
    {
        result = CompareResult::WrongOrder;
    }
    // Right still has numbers to process, left doesn't. Right order.
    else if (_rightIt != _other->GetChildren().end())
    {
        result = CompareResult::RightOrder;
    }

    // If still inconclusive, both left and right have run out of numbers to process. They are the same.

    return result;
}


CChallenge_13::CompareResult CChallenge_13::NodeComposite::Compare(const Leaf* _other) const
{
    NodeComposite* convertedLeaf = new NodeComposite(*_other);

    CompareResult result = Compare(convertedLeaf);

    delete convertedLeaf;
    return result;
}


CChallenge_13::CompareResult CChallenge_13::NodeComposite::Compare(const Node* _other) const
{
    switch (_other->GetNodeType())
    {
    case NodeType::NodeComposite:
        return Compare(static_cast<const NodeComposite*>(_other));
        break;
    case NodeType::Leaf:
        return Compare(static_cast<const Leaf*>(_other));
        break;
    }

    return CompareResult::Inconclusive;
}

CChallenge_13::CompareResult CChallenge_13::Leaf::Compare(const Leaf* _other) const
{
    if (m_Value < _other->GetValue())
    {
        return CompareResult::RightOrder;
    }
    else if (m_Value > _other->GetValue())
    {
        return CompareResult::WrongOrder;
    }

    return CompareResult::Inconclusive;
}


CChallenge_13::CompareResult CChallenge_13::Leaf::Compare(const NodeComposite* _other) const
{
    NodeComposite* convertedLeaf = new NodeComposite(*this);

    CompareResult result = convertedLeaf->Compare(_other);

    delete convertedLeaf;
    return result;
}

CChallenge_13::CompareResult CChallenge_13::Leaf::Compare(const Node* _other) const
{
    switch (_other->GetNodeType())
    {
    case NodeType::NodeComposite:
        return Compare(static_cast<const NodeComposite*>(_other));
        break;
    case NodeType::Leaf:
        return Compare(static_cast<const Leaf*>(_other));
        break;
    }

    return CompareResult::Inconclusive;
}

CChallenge_13::Leaf::Leaf(int _value) 
    : m_Value(_value) 
{ 
    m_NodeType = NodeType::Leaf;

#ifdef _DEBUG
    ToString(m_DebugString);
#endif // _DEBUG
}

void CChallenge_13::Leaf::Print() const
{
    cout << m_Value;
}

void CChallenge_13::Leaf::ToString(string& _outString) const
{
    _outString += to_string(m_Value);
}

CChallenge_13::Packet::Packet(const string& _packetStr)
{
    Build(_packetStr);
}

void CChallenge_13::Packet::Build(const string& _packetStr)
{
    m_Root = new NodeComposite();

    queue<char> workString;
    for (const char& character : _packetStr)
    {
        workString.push(character);
    }

    workString.pop();
    m_Root->Build(workString);

#ifdef _DEBUG
    m_Root->ToString(m_DebugString);
#endif // _DEBUG
}


CChallenge_13::CompareResult CChallenge_13::Packet::Compare(const Packet& _other) const
{
    return m_Root->Compare(_other.m_Root);
}

void CChallenge_13::Packet::Print() const
{
    if (m_Root != nullptr)
    {
        m_Root->Print();
    }
}

bool CChallenge_13::Packet::operator==(const Packet& _other) const
{
    return Compare(_other) == CompareResult::Inconclusive;
}

bool CChallenge_13::Packet::operator<(const Packet& _other) const
{
    return Compare(_other) == CompareResult::RightOrder;
}
