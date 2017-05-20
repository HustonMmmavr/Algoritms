// ConsoleApplication16.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"


#include <stdio.h>
#include <stdlib.h>
using namespace std;
#include <queue>
#include <exception>
#include <iostream>
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
    queueOfNodes.push(node);
    if(node)
    {
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

int Fill(BinaryTree<int> &tree)
{
	int n;
	scanf("%d", &n);
	int data;
	int i = 0;

	while (i < n)
	{
		i++;
		scanf("%d\n", &data);
		tree.Insert(data);
	}
}

int main()
{
	try
	{
		BinaryTree<int> tree;
		Fill(tree);
		tree.LevelOrder();
	}
	catch(Exception &e)
	{
		cout << e.what() << "\n";
	}
	return 0;
}
