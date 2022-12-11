#pragma once

#include "Common/Challenge.h"

using namespace std;


class CChallenge_07 : public CChallenge
{
private:
    class Folder;

    enum class FileSystemElementType
    {
        None,
        File,
        Folder
    };

    class FileSystemElement
    {
    public:
        FileSystemElement(const string& _name, Folder* _parentFolder, FileSystemElementType _type) : m_Name(_name), m_ParentFolder(_parentFolder), m_Type(_type) {}
        virtual ~FileSystemElement() {}

        virtual long long GetSize() = 0;
        const string& GetName() const { return m_Name; }
        Folder* GetParentFolder() const  { return m_ParentFolder; }
        FileSystemElementType GetType() const { return m_Type; }

    private:
        string m_Name;
        Folder* m_ParentFolder = nullptr;
        FileSystemElementType m_Type = FileSystemElementType::None;
    };


    class File : public FileSystemElement
    {
    public:
        File(const string& _name, Folder* _parentFolder, long long _size) : FileSystemElement(_name, _parentFolder, FileSystemElementType::File), m_Size(_size) {}
        virtual ~File() {}

        virtual long long GetSize() override { return m_Size; }

    private:
        long long m_Size;
    };


    class Folder : public FileSystemElement
    {
    public:
        class ElementFinder
        {
        public:
            ElementFinder(const FileSystemElement* _element) : m_Element(_element) {}

            bool operator()(const FileSystemElement* _element) const
            {
                return (_element->GetName() == m_Element->GetName());
            }

        private:
            const FileSystemElement* m_Element;
        };

        class ElementFinderByName
        {
        public:
            ElementFinderByName(const string& _elementName) : m_ElementName(_elementName) {}

            bool operator()(const FileSystemElement* _element) const
            {
                return (_element->GetName().compare(m_ElementName) == 0);
            }

        private:
            string m_ElementName;
        };

        Folder(const string& _name, Folder* _parentFolder) : FileSystemElement(_name, _parentFolder, FileSystemElementType::Folder) {}
        virtual ~Folder();

        virtual long long GetSize() override;
        const vector<FileSystemElement*>& GetElements() const { return m_Elements; }

        void AddElement(FileSystemElement* _element);
        bool Contains(const FileSystemElement* _element) const { return find_if(m_Elements.begin(), m_Elements.end(), ElementFinder(_element)) != m_Elements.end(); }
        FileSystemElement* FindElement(const string& _elementName);

    private:
        vector<FileSystemElement*> m_Elements;
        bool m_IsDirty = true;
        long long m_CachedSize = 0ll;
    };
    

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void RebuildFileSystem();
    void ComputeTotalSize(const Folder* _root, long long _maxSize, long long& _outTotalSize);
    void FindSmallestFolderToDelete(const Folder* _root, long long _minSize, Folder*& _outFolder);

    static string const sm_inputFilePath;

    vector<string> m_Output;
    Folder* m_Root = nullptr;
};
