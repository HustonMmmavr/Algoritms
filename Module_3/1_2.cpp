// ConsoleApplication13.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//

//#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <exception>
#include <string>
#include <iostream>
using std::exception;
using std::string;
using std::cin;
using std::cout;
#define REHASH_COEFF 3.0 / 4.0
#define HASH_ARG1 33
#define HASH_ARG2 47
class Exception : public exception
{
    char message[1024];
public:
    Exception() {}
    Exception(const char *message, const char* file, const char* func, int line);
    ~Exception() throw() {}
    const char* what() const throw();
};

// Exception Realisation
Exception::Exception(const char *message, const char* file, const char* func, int line)
{
    sprintf(this->message, "In file %s func %s line %d message %s", file, func, line, message);
}

const char* Exception::what() const throw()
{
    return message;
}
#define ThrowException(msg) throw Exception(msg, __FILE__, __FUNCTION__, __LINE__);
/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Array (my vector)
#define MEMORY_STEP 2
typedef long sizeT;
template <typename T>
class Array
{
    T* ptr;
    sizeT allocatedSize;
    sizeT elementsInBuffer;
public:
    Array();
    Array(sizeT allocatedSize);
    Array(sizeT allocatedSize, T defaultValue);
    Array(const Array& arr);
    ~Array();
    void PushBack(const T& data);
    void Resize();
    sizeT Count() const { return elementsInBuffer; }
    sizeT Allocated() const { return allocatedSize; }
    T* GetPointer();
    void Swap(T *a, T* b) { T temp = *a; *a = *b, *b = temp; }
    const T& operator[] (sizeT i) const { return ptr[i]; }
    T& operator[] (sizeT i) { return ptr[i]; }
    Array& operator = (const Array& arr);
    const T& AtIndex(sizeT i) const { return ptr[i]; }
    T& AtIndex(sizeT i) { return ptr[i]; }

};


template <typename T>
Array<T>::Array()
{
    allocatedSize = 0;
    ptr = NULL;
    elementsInBuffer = 0;
}

template <typename T>
Array<T>::Array(const Array<T>& arr)
{
    ptr = NULL;
    *this = arr;
}

template <typename T>
Array<T>::Array(sizeT sizeToAlloc)
{
    allocatedSize = sizeToAlloc;
    if (allocatedSize == 0)
    {
        ptr = NULL;
    }
    else
    {
        ptr = new T[allocatedSize];

        if (!ptr)
            ThrowException("cant allocate");
    }
    elementsInBuffer = 0;
}

template <typename T>
Array<T>::Array(sizeT sizeToAlloc, T defaultValue)
{
    allocatedSize = sizeToAlloc;
    if (allocatedSize == 0)
    {
        ptr = NULL;
    }
    else
    {
        ptr = new T[allocatedSize];
        for (int i = 0; i < allocatedSize; i++)
            ptr[i] = defaultValue;

        if (!ptr)
            ThrowException("cant allocate");
    }
    elementsInBuffer = 0;
}

template <typename T>
Array<T>::~Array()
{
    delete[] ptr;
}

template <typename T>
void Array<T>::PushBack(const T& data)
{
    if (elementsInBuffer == allocatedSize)
        Resize();
    ptr[elementsInBuffer++] = data;
}

template <typename T>
void Array<T>::Resize()
{
    if (allocatedSize == 0)
    {
        allocatedSize = 1;
        ptr = new T[allocatedSize];
        if (!ptr) ThrowException("err");
        return;
    }
    sizeT newSize = allocatedSize * MEMORY_STEP;
    T *oldPtr = ptr;
    T *newPtr = new T[newSize];
    if (!newPtr) ThrowException("Cant reallcate");
    for (int i = 0; i < allocatedSize; i++)
        newPtr[i] = oldPtr[i];
    delete[] oldPtr;
    ptr = newPtr;
    allocatedSize *= MEMORY_STEP;
}

template <typename T>
T *Array<T>::GetPointer()
{
    return ptr;
}

template <typename T>
Array<T>& Array<T>::operator = (const Array<T>& arr)
{
    delete[] ptr;
    allocatedSize = arr.allocatedSize;
    elementsInBuffer = arr.elementsInBuffer;
    if (allocatedSize == 0)
    {
        ptr = NULL;
        return *this;
    }
    ptr = new T[allocatedSize];
    if (!ptr) ThrowException("Cant alloc");
    for (int i = 0; i < allocatedSize; i++)
        ptr[i] = arr.ptr[i];
    //memcpy(ptr, arr.ptr, elementsInBuffer * sizeof(T));
    return *this;
}


typedef Array<char> String;

template <typename T>
// y = (str[i] * coeff + str[i + 1]) + b
inline int HashFunction(const T &data, int mod, int k)
{
    int res = 0;
    for (int i = 0; i < data.length(); i++)
    {
        res += res * k + data[i];
        res %= mod;
    }
    return res;
}

template <typename T>
inline int HashFunction1(const T &data, int mod, int k)
{
    return HashFunction(data, mod, k);
}

template <typename T>
inline int HashFunction2(const T &data, int mod, int k)
{
    int res = HashFunction(data, mod, k);
    return (2 * res % mod) + 1;
}


template <typename T>
class HashTable
{
public:
    HashTable(int size);
    ~HashTable();
    bool AddData(T &s);
    bool RemoveData(T &s);
    bool ExistData(T &s) const;
private:
    struct Node
    {
        T _data;
        Node * next;
        bool isDeleted;

        Node(T data) { _data = data; isDeleted = false; next = NULL; }
    };
    void Rehash();
    Array<Node* > arrayOfNodes;
    int _sizeAllocated;
    int _size;
};


template <typename T>
HashTable<T>::HashTable(int size)
{
    _size = 0;
    _sizeAllocated = size;
    arrayOfNodes = Array<Node*>(size, NULL);
}

template <typename T>
HashTable<T>::~HashTable()
{
    for (int i = 0; i < _sizeAllocated; i++)
        delete arrayOfNodes[i];
}


template <typename T>
bool HashTable<T>::AddData(T &data)
{
    if (_size / (double)_sizeAllocated >= REHASH_COEFF)
        Rehash();
    int hash1 = HashFunction1(data, _sizeAllocated, HASH_ARG1);
    int hash2 = HashFunction2(data, _sizeAllocated, HASH_ARG2);

    for (int i = 0; i < _sizeAllocated; i++)
    {
        if (!arrayOfNodes[hash1])
        {
            Node *node = new Node(data);
            if (!node)
                ThrowException("cant allocate node");
            arrayOfNodes[hash1] = node;
            _size++;
            return true;

        }
        else if (arrayOfNodes[hash1]->isDeleted)
        {
            arrayOfNodes[hash1]->_data = data;
            arrayOfNodes[hash1]->isDeleted = false;
            _size++;
            return true;
        } else	if (arrayOfNodes[hash1]->_data == data && !arrayOfNodes[hash1]->isDeleted)
            return false;

        hash1 += hash2;
        hash1 %= _sizeAllocated;
    }
    return false;
}

template <typename T>
bool HashTable<T>::RemoveData(T &data)
{
    int hash1 = HashFunction1(data, _sizeAllocated, HASH_ARG1);
    int hash2 = HashFunction2(data, _sizeAllocated, HASH_ARG2);

    for (int i = 0; i < _sizeAllocated; i++)
    {
        if (arrayOfNodes[hash1])
        {
            if (arrayOfNodes[hash1]->_data == data && !arrayOfNodes[hash1]->isDeleted)
            {
                arrayOfNodes[hash1]->isDeleted = true;
                _size--;
                return true;
            }
        }
        else
            return false;

        hash1 += hash2;
        hash1 %= _sizeAllocated;
    }
    return false;
}

template <typename T>
bool HashTable<T>::ExistData(T &data) const
{
    int hash1 = HashFunction1(data, _sizeAllocated, HASH_ARG1);
    int hash2 = HashFunction2(data, _sizeAllocated, HASH_ARG2);
    for (int i = 0; i < _sizeAllocated; i++)
    {
        if (arrayOfNodes[hash1])
        {
            if (arrayOfNodes[hash1]->_data == data && !arrayOfNodes[hash1]->isDeleted)
                return true;
        }
        else
            return false;
        hash1 += hash2;
        hash1 %= _sizeAllocated;
    }
    return false;
}


template <typename T>
void HashTable<T>::Rehash()
{
    int newSizeAllocated = 2 * _sizeAllocated;
    Array<Node*> newArray = Array<Node*>(newSizeAllocated, NULL);
    for (int i = 0; i < _sizeAllocated; i++)
    {
        if (arrayOfNodes[i] != NULL)
        {
            if (arrayOfNodes[i]->isDeleted)
            {
                delete arrayOfNodes[i];
            }
            else
            {
                int hash1 = HashFunction1(arrayOfNodes[i]->_data, newSizeAllocated, HASH_ARG1);
                int hash2 = HashFunction2(arrayOfNodes[i]->_data, newSizeAllocated, HASH_ARG2);
                int j = 0;
                while (newArray[hash1] && j < newSizeAllocated)
                {
                    hash1 += hash2;
                    hash1 %= newSizeAllocated;
                    j++;
                }

                newArray[hash1] = arrayOfNodes[i];
            }
        }
    }
    arrayOfNodes = newArray;
    _sizeAllocated = newSizeAllocated;
}


int Commander(HashTable<string> &hashTable)
{
    char operation;
    string str;
    while (cin >> operation >> str)
    {
        bool res;

        if (operation == '+')
        {
            res = hashTable.AddData(str);
        }
        else if (operation == '-')
        {
            res = hashTable.RemoveData(str);
        }
        else if (operation == '?')
        {
            res = hashTable.ExistData(str);
        }
        cout << ((res) ? "OK" : "FAIL") << "\n";
    }
    return 0;
}

int main()
{
    try
    {
        HashTable<string> hashTable(8);
        Commander(hashTable);
    }
    catch(exception e)
    {
        cout << e.what() << "\n";
    }
    return 0;
}
