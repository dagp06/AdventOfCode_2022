#include "Challenge_07.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_07::sm_inputFilePath = "Inputs/Input_Challenge_07.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_07::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Output = lines;

    return EErrorCode::Success;
}

EErrorCode CChallenge_07::Run_FirstPart()
{
    // Rebuild file system
    RebuildFileSystem();

    // Compute total size for folders lighter than 100000
    long long totalSize = 0ll;
    long long maxSize = 100000ll;
    ComputeTotalSize(m_Root, maxSize, totalSize);

    cout << "The total size of folders smaller than " << maxSize << " is " << totalSize << "." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_07::CleanUp_FirstPart()
{
    delete m_Root;
    m_Root = nullptr;

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_07::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }
    
    m_Output = lines;

    return EErrorCode::Success;
}

EErrorCode CChallenge_07::Run_SecondPart()
{
    // Rebuild file system
    RebuildFileSystem();

    // Compute current root size
    const long long diskSpace = 70000000ll;
    const long long requiredDiskSpace = 30000000ll;
    long long availableSpace = diskSpace - m_Root->GetSize();
    long long spaceToFree = requiredDiskSpace - availableSpace;
    Folder* folderToDelete = m_Root;
    FindSmallestFolderToDelete(m_Root, spaceToFree, folderToDelete);

    cout << "We need to free " << spaceToFree << ". If we deleted folder " << folderToDelete->GetName() << ", we'd save " << folderToDelete->GetSize() << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_07::CleanUp_SecondPart()
{
    delete m_Root;
    m_Root = nullptr;

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_07::RebuildFileSystem()
{
    m_Root = new Folder("/", nullptr);

    Folder* currentFolder = nullptr;
    int currentCommandIdx = 0;
    while (currentCommandIdx < m_Output.size())
    {
        vector<string> commandStrings;
        FileHelper::SplitLine(m_Output[currentCommandIdx], " ", commandStrings);

        if (commandStrings[1].compare("cd") == 0)
        {
            if (commandStrings[2].compare("/") == 0)
            {
                currentFolder = m_Root;
            }
            else if (commandStrings[2].compare("..") == 0)
            {
                currentFolder = currentFolder ? currentFolder->GetParentFolder() : nullptr;
            }
            else
            {
                currentFolder = currentFolder ? static_cast<Folder*>(currentFolder->FindElement(commandStrings[2])) : nullptr;
            }

            ++currentCommandIdx;
        }
        else if (commandStrings[1].compare("ls") == 0)
        {
            int currentElementIdx = currentCommandIdx + 1;
            while (currentElementIdx < m_Output.size() && m_Output[currentElementIdx][0] != '$')
            {
                vector<string> elementStrings;
                FileHelper::SplitLine(m_Output[currentElementIdx], " ", elementStrings);

                if (currentFolder->FindElement(elementStrings[1]) == nullptr)
                {
                    if (isdigit(elementStrings[0][0])) // File
                    {
                        currentFolder->AddElement(new File(elementStrings[1], currentFolder, stoi(elementStrings[0])));
                    }
                    else // Folder
                    {
                        currentFolder->AddElement(new Folder(elementStrings[1], currentFolder));
                    }
                }

                ++currentElementIdx;
            }

            currentCommandIdx = currentElementIdx;
        }
    }
}

void CChallenge_07::ComputeTotalSize(const Folder* _root, long long _maxSize, long long& _outTotalSize)
{
    const vector<FileSystemElement*>& elements = _root->GetElements();
    for (FileSystemElement* element : elements)
    {
        if (element->GetType() != FileSystemElementType::Folder)
            continue;

        long long folderSize = element->GetSize();
        if (folderSize <= _maxSize)
            _outTotalSize += folderSize;

        ComputeTotalSize(static_cast<const Folder*>(element), _maxSize, _outTotalSize);
    }
}

void CChallenge_07::FindSmallestFolderToDelete(const Folder* _root, long long _minSize, Folder*& _outFolder)
{
    const vector<FileSystemElement*>& elements = _root->GetElements();
    for (FileSystemElement* element : elements)
    {
        if (element->GetType() != FileSystemElementType::Folder)
            continue;

        long long folderSize = element->GetSize();
        if (folderSize >= _minSize && (_outFolder == nullptr || folderSize < _outFolder->GetSize()))
            _outFolder = static_cast<Folder*>(element);

        FindSmallestFolderToDelete(static_cast<const Folder*>(element), _minSize, _outFolder);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
CChallenge_07::Folder::~Folder()
{
    for (FileSystemElement* element : m_Elements)
    {
        delete element;
    }

    m_Elements.clear();
}

CChallenge_07::FileSystemElement* CChallenge_07::Folder::FindElement(const string& _elementName)
{
    vector<FileSystemElement*>::iterator it = find_if(m_Elements.begin(), m_Elements.end(), ElementFinderByName(_elementName));

    return it != m_Elements.end() ? *it : nullptr;
}

long long CChallenge_07::Folder::GetSize()
{
    if (!m_IsDirty)
        return m_CachedSize;
    else
    {
        long long size = 0;
        for (FileSystemElement* element : m_Elements)
        {
            size += element->GetSize();
        }

        m_CachedSize = size;
        m_IsDirty = false;
        return size;
    }
}

void CChallenge_07::Folder::AddElement(FileSystemElement* _element)
{
    m_Elements.push_back(_element);
    m_IsDirty = true;
}