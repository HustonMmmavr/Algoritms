//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <exception>
#include <stack>
using namespace std;

class Exception : public std::exception
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
    return *this;
}


template <typename T>
struct Node
{
    T data;
    unsigned char height;
    int nodesCount;
    Node *left;
    Node *right;
    Node(T data_, Node *left_ = NULL, Node *right_ = NULL) :
        data(data_), left(left_), right(right_), height(1), nodesCount(1) {}
};

template <typename T>
class AVLTree
{
public:
    AVLTree() : root(NULL) {}
    ~AVLTree();
    void Insert(const T &data, int *position);
    void Remove(const T &data);
    void Search(const T &data) const;
    void LevelOrder() const;
    int GetMaxWidth() const;
private:
    unsigned char Height(Node<T> *node) const;
    int BalanceFactor(Node<T> *node) const;
    Node<T> *Insert(Node<T> *node, const T &data, int *position);
    Node<T> *Remove(Node<T> *node, const T &data);
    Node<T> *BalanceNode(Node<T> *node);
    Node<T> *FindMin(Node<T> *node) const;
    Node<T> *RemoveMin(Node<T> *node);

    void FixNodesCount(Node<T> *node);
    void FixHeight(Node<T> *node);
    Node<T>* RotateRight(Node<T> *node);
    Node<T>* RotateLeft(Node<T> *node);
    Node<T>* root;
};

template <typename T>
unsigned char AVLTree<T>::Height(Node<T> *node) const
{
    return node ? node->height: 0;
}

template <typename T>
int AVLTree<T>::BalanceFactor(Node<T> *node) const
{
    return Height(node->right) - Height(node->left);
}

template <typename T>
void AVLTree<T>::FixHeight(Node<T> *node)
{
    unsigned char leftHeight = Height(node->left);
    unsigned char rightHeight = Height(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

template <typename T>
void AVLTree<T>::FixNodesCount(Node<T> *node)
{
    int nodesCount = 1;
    if (node->right) nodesCount += node->right->nodesCount;
    if (node->left) nodesCount += node->left->nodesCount;
    node->nodesCount = nodesCount;
}

template <typename T>
Node<T>* AVLTree<T>::RotateRight(Node<T> *node)
{
    Node<T> *newParent = node->left;
    node->left = newParent->right;
    newParent->right = node;

    FixNodesCount(node);
    FixNodesCount(newParent);
    FixHeight(node);
    FixHeight(newParent);
    return newParent;
}

template <typename T>
Node<T>* AVLTree<T>::RotateLeft(Node<T> *node)
{
    Node<T> *newParent = node->right;
    node->right = newParent->left;
    newParent->left = node;

    FixNodesCount(node);
    FixNodesCount(newParent);
    FixHeight(node);
    FixHeight(newParent);
    return newParent;
}

template <typename T>
Node<T> *AVLTree<T>::BalanceNode(Node<T> *node)
{
    FixHeight(node);
    if (BalanceFactor(node) == 2)
    {
        if(BalanceFactor(node->right) < 0)
            node->right = RotateRight(node->right);
        return RotateLeft(node);
    }
    if (BalanceFactor(node) == -2)
    {
        if(BalanceFactor(node->left) > 0)
            node->left = RotateLeft(node->left);
        return RotateRight(node);
    }
    return node;
}

template <typename T>
Node<T>* AVLTree<T>::FindMin(Node<T>* node) const
{
    return node->left ? FindMin(node->left) : node;
}

template <typename T>
Node<T>* AVLTree<T>::RemoveMin(Node<T>* node)
{
    if (node->left == NULL) return node->right;
    node->left = RemoveMin(node->left);
    node->nodesCount--;
    return BalanceNode(node);
}

template <typename T>
Node<T>* AVLTree<T>::Insert(Node<T> *node, const T &data, int *position)
{
    if (!node)
    {
        Node<T> *out = new Node<T>(data);
        if (!out)
            ThrowException("cant allocate node");
        return out;
    }

    node->nodesCount++;

    if (data > node->data)
         node->right = Insert(node->right, data, position);
    else
    {
        *position += (node->right ? node->right->nodesCount : 0) + 1;
        node->left = Insert(node->left, data, position);
    }

    return BalanceNode(node);
}

template <typename T>
void AVLTree<T>::Insert(const T& data, int *position)
{
    root = Insert(root, data, position);
}

template <typename T>
Node<T> *AVLTree<T>::Remove(Node<T> *node, const T &data)
{
    if (!node) return NULL;
    if (data > node->nodesCount) return node;

    int summaryNodes = 0;
    std::stack<Node<T>*> nodesStack;
    int flag = 1;
    while(flag)
    {
        int nodesCountRight = node->right ? node->right->nodesCount : 0;
        int diffNodes = data - summaryNodes;//position - dummaryNodes
        if (diffNodes > nodesCountRight)
        {
            nodesStack.push(node);
            node = node->left;
            summaryNodes += nodesCountRight + 1;
        }
        else if (diffNodes < nodesCountRight)
        {
            if (node->right)
            {
                nodesStack.push(node);
                node = node->right;
            }
            else
                flag = 1;
        }
        else
        {
            Node<T> *l = node->left;
            Node<T> *r = node->right;
            T nodeData = node->data;
            delete node;
            if (!r)
            {
                if (!l)
                {
                    if (nodesStack.empty())
                        return NULL;
                    else
                    {
                        node = nodesStack.top();
                        nodesStack.pop();

                        if (node->data > nodeData)
                            node->left = NULL;
                        else
                            node->right = NULL;
                        node->nodesCount--;
                    }
                }
                else
                    node = l;
            }
            else
            {
                Node<T>* min = FindMin(r);
                min->right = RemoveMin(r);
                min->left = l;
                FixNodesCount(min);
                node = BalanceNode(min);
            }
            flag = 0;
        }
    }

    Node<T> *temp;

    while (!nodesStack.empty())
    {
        temp = nodesStack.top();
        temp->nodesCount--;

        if (temp->data > node->data)
            temp->left = node;
        else
            temp->right = node;

        node = BalanceNode(temp);
        nodesStack.pop();
    }

    return node;
}

template <typename T>
void AVLTree<T>::Remove(const T& data)
{
    root = Remove(root, data);
}



template <typename T>
void AVLTree<T>::Search(const T& data) const
{
    Node<T> *node = root;
    int flag = 1;
    while (node && flag)
    {
        if (node->data == data)
            flag = 0;
        else if(node->data > data)
            node = node->left;
        else
            node = node->right;
    }
    return node;
}

template <typename T>
AVLTree<T>::~AVLTree()
{
    queue<Node<T>*> queueOfNodes;
    Node<T> *node = root;
    if(node)
    {
        queueOfNodes.push(node);
        while (queueOfNodes.size())
        {
            node = queueOfNodes.front();
            queueOfNodes.pop();
            if (node->left)
                queueOfNodes.push(node->left);
            if (node->right)
                queueOfNodes.push(node->right);
            delete node;
        }
    }
}

template <typename T>
void AVLTree<T>::LevelOrder() const
{
    queue<Node<T>*> queueOfNodes;
    Node<T> *node = root;
    queueOfNodes.push(node);
    while (queueOfNodes.size())
    {
        node = queueOfNodes.front();
        queueOfNodes.pop();
        printf("%d ", node->data);
        if (node->left)
            queueOfNodes.push(node->left);
        if (node->right)
            queueOfNodes.push(node->right);
    }
}

template <typename T>
int AVLTree<T>::GetMaxWidth() const
{
    queue<Node<T>*> queueOfNodes;
    queueOfNodes.push(root);
    int max = 0;
    while (queueOfNodes.size())
    {
        int count = queueOfNodes.size();
        if (count > max)
            max = count;

        while (count--)
        {
            Node<T>* node = queueOfNodes.front();
            queueOfNodes.pop();
            if (node->left)
                queueOfNodes.push(node->left);
            if (node->right)
                queueOfNodes.push(node->right);
        }

    }
    return max;
}


Array<int> GetPositions()
{
    AVLTree<int> tree;
    int size;
    int operation, data;
    Array<int> positions;
    int position;
    scanf("%d", &size);
    for (int i = 0; i < size; i++)
    {
        scanf("%d %d", &operation, &data);
        position = 0;
        if(operation==1)
        {
            tree.Insert(data, &position);
            positions.PushBack(position);
        }
        if (operation == 2)
        {
            tree.Remove(data);
        }
    }
    return positions;
}

int main()
{
    try
    {
        Array<int> positions = GetPositions();
        for (int i = 0; i < positions.Count(); i++)
            printf("%d\n", positions[i]);
    }
    catch (Exception &e)
    {
        printf("%s", e.what());
    }
    return 0;

}
