//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <exception>
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

template <typename T>
class BinaryTree
{
public:
    BinaryTree() : root(NULL) {}
    BinaryTree(const T &data);
    ~BinaryTree();
    void Insert(const T &data);
    void LevelOrder() const;
    int GetMaxWidth() const;
private:
    struct Node
    {
        T data;
        Node *left;
        Node *right;
        Node(T _data = 0, Node *_left = NULL, Node *_right = NULL) :
            data(_data), right(_right), left(_left) {}
    };

    Node* root;
};

template <typename T>
BinaryTree<T>::BinaryTree(const T &data)
{
    root = new Node(data);
    if (!root)
        ThrowException("cant alloc root");
}

template <typename T>
void BinaryTree<T>::Insert(const T &data)
{
    Node *newNode = new Node(data);
    if (!newNode)
        ThrowException("cant alloc node");
    if (!root)
    {
        root = newNode;
        return;
    }

    Node *temp = root;
    while (temp)
    {
        if (temp->data >= data)
        {
            if (temp->left)
                temp = temp->left;
            else
            {
                temp->left = newNode;
                return;
            }
        }
        else
        {
            if (temp->right)
                temp = temp->right;
            else
            {
                temp->right = newNode;
                return;
            }
        }
    }
    return;
}

template <typename T>
BinaryTree<T>::~BinaryTree()
{
    queue<Node*> queueOfNodes;
    Node *node = root;
    if (node)
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
void BinaryTree<T>::LevelOrder() const
{
    queue<Node*> queueOfNodes;
    Node *node = root;
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
int BinaryTree<T>::GetMaxWidth() const
{
    queue<Node*> queueOfNodes;
    queueOfNodes.push(root);
    int max = 0;
    while (queueOfNodes.size())
    {
        int count = queueOfNodes.size();
        if (count > max)
            max = count;

        while (count--)
        {
            Node* node = queueOfNodes.front();
            queueOfNodes.pop();
            if (node->left)
                queueOfNodes.push(node->left);
            if (node->right)
                queueOfNodes.push(node->right);
        }

    }
    return max;
}

template <typename T1, typename T2>
struct Node
{
    T1 x;
    T2 y;
    Node *left;
    Node *right;

    Node(T1 x_ = 0, T2 y_ = 0, Node *left_ = NULL, Node *right_ = NULL) :
        x(x_), y(y_), left(left_), right(right_) {}
};



template <typename T1, typename T2>
class Treap
{
public:
    Treap() : root(NULL) {}
    ~Treap();
    void Insert(T1 data, T2 priority);
    int GetMaxWidth() const;
    void LevelOrder() const;
private:
    void Split(Node<T1, T2>* current, T1 key, Node<T1, T2> *&left, Node<T1, T2> *&right) const;
    Node<T1, T2>* Merge(Node<T1, T2> *left, Node<T1, T2> *right);

    Node<T1, T2> *root;
};

template <typename T1, typename T2>
void Treap<T1, T2>::Split(Node<T1, T2> *current, T1 key, Node<T1, T2> *&left, Node<T1, T2> *&right) const
{
    if (!current)
    {
        right = left = NULL;
    }
    else if (current->x <= key)
    {
        Split(current->right, key, current->right, right);
        left = current;
    }
    else
    {
        Split(current->left, key, left, current->left);
        right = current;
    }
}

template <typename T1, typename T2>
Node<T1, T2>* Treap<T1, T2>::Merge(Node<T1, T2> *left, Node<T1, T2> *right)
{
    if (!left || !right)
    {
        return left ? left : right;
    }
    if (left->y > right->y)
    {
        left->right = Merge(left->right, right);
        return left;
    }
    right->left = Merge(left, right->left);
    return right;
}

template <typename T1, typename T2>
Treap<T1, T2>::~Treap()
{
    queue<Node<T1, T2>*> queueOfNodes;
    Node<T1, T2> *node = root;
    if (node)
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

template <typename T1, typename T2>
int Treap<T1, T2>::GetMaxWidth() const
{
    queue<Node<T1, T2>*> queueOfNodes;

    queueOfNodes.push(root);
    int max = 0;
    while (queueOfNodes.size())
    {
        int count = queueOfNodes.size();
        if (count > max)
            max = count;

        while (count--)
        {
            Node<T1, T2>* node = queueOfNodes.front();
            queueOfNodes.pop();
            if (node->left)
                queueOfNodes.push(node->left);
            if (node->right)
                queueOfNodes.push(node->right);
        }
    }
    return max;
}

template <typename T1, typename T2>
void Treap<T1, T2>::LevelOrder() const
{
    queue<Node<T1, T2>*> queueOfNodes;
    Node<T1, T2> *node = root;
    queueOfNodes.push(node);
    while (queueOfNodes.size())
    {
        node = queueOfNodes.front();
        queueOfNodes.pop();
        printf("%d %d  ", node->x, node->y);
        if (node->left)
            queueOfNodes.push(node->left);
        if (node->right)
            queueOfNodes.push(node->right);
    }
}

template <typename T1, typename T2>
void Treap<T1, T2>::Insert(T1 data, T2 priority)
{
    Node<T1, T2> *newNode = new Node<T1, T2>(data, priority);
    if (!newNode)
        ThrowException("cant alloc node");
    if (!root)
    {
        root = newNode;
        return;
    }

    Node<T1, T2> *current = root;
    Node<T1, T2> *previous = NULL;
    int flag = 1;
    while (flag)
    {
        if (newNode->y > current->y)
        {
            flag = 0;
        }
        else
        {
            if (current->x >= newNode->x)
            {
                if (current->left)
                {
                    previous = current;
                    current = current->left;
                }
                else
                {
                    current->left = newNode;
                    return;
                }
            }
            else
            {
                if (current->right)
                {
                    previous = current;
                    current = current->right;
                }
                else
                {
                    current->right = newNode;
                    return;
                }
            }
        }
    }

    Split(current, data, newNode->left, newNode->right);
    if (previous)
    {
        if (previous->left == current)
        {
            previous->left = newNode;
        }
        else
        {
            previous->right = newNode;
        }
    }
    else
        root = newNode;
    return;
}

int Fill(BinaryTree<int> &tree, Treap<int, int> &treap)
{
    int n;
    scanf("%d", &n);
    int data;
    int priority;
    int i = 0;

    while (i < n)
    {
        i++;
        scanf("%d %d\n", &data, &priority);
        tree.Insert(data);
        treap.Insert(data, priority);
    }
    return 0;
}

int main()
{
    try
    {
        BinaryTree<int> tree;
        Treap<int, int> treap;
        Fill(tree, treap);
        printf("%d", treap.GetMaxWidth() - tree.GetMaxWidth());
    }
    catch (Exception &e)
    {
        printf("%s", e.what());
    }
    return 0;

}
