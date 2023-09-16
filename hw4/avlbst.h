#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    signed char getBalance () const;
    void setBalance (signed char balance);
    void updateBalance(signed char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    signed char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
signed char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(signed char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(signed char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insert_fix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void remove_fix(AVLNode<Key, Value>* n, signed char diff);

    bool isLeftChild(AVLNode<Key, Value>* node);
    bool isRightChild(AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // DONE
    AVLNode<Key, Value>* n = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);

    if(this->root_ == NULL) // tree is empty
    {
        this->root_ = n;
        n->setBalance(0);
        return;
    }

    // given: tree isn't empty
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(temp != NULL) // while temp is a valid node
    {
        if(temp->getKey() == new_item.first)
        {
            temp->setValue(new_item.second); // overwrite the current value
            delete n;
            return;
        }
        else if(new_item.first < temp->getKey()) // need to go left
        {
            if(temp->getLeft() == NULL) // no left child
            {
                temp->setLeft(n);
                n->setParent(temp);

                if(temp->getBalance() == -1)
                {
                    temp->setBalance(0);
                }
                else if(temp->getBalance() == 1)
                {
                    temp->setBalance(0);
                }
                else if(temp->getBalance() == 0)
                {
                    temp->setBalance(-1);
                    insert_fix(temp, n);
                }
                return;
            }
            else
            {
                temp = temp->getLeft(); // go to left child
            }
        }
        else if(new_item.first > temp->getKey())
        {
            if(temp->getRight() == NULL) // no right child
            {
                temp->setRight(n);
                n->setParent(temp);

                if(temp->getBalance() == -1)
                {
                    temp->setBalance(0);
                }
                else if(temp->getBalance() == 1)
                {
                    temp->setBalance(0);
                }
                else if(temp->getBalance() == 0)
                {
                    temp->setBalance(1);
                    insert_fix(temp, n);
                }
                return;
            }
            else 
            {
                temp = temp->getRight(); // go to right child
            }
        }
        else break; // temp has no children
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    // DONE
    if(p == NULL || p->getParent() == NULL) return; // do nothing

    AVLNode<Key, Value>* g = p->getParent();

    if(isLeftChild(p))
    {
        g->updateBalance(-1);

        // Case 1
        if(g->getBalance() == 0) return;
        // Case 2
        else if(g->getBalance() == -1) insert_fix(g, p);
        // Case 3
        else if(g->getBalance() == -2)
        {
            if((isLeftChild(p) && isLeftChild(n)) || (isRightChild(p) && isRightChild(n))) // zig-zig check
            {
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else if((isLeftChild(p) && isRightChild(n)) || (isRightChild(p) && isLeftChild(n))) // zig-zag check
            {
                rotateLeft(p);
                rotateRight(g);
                
                // Case 3a
                if(n->getBalance() == -1)
                {
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                // Case 3b
                else if(n->getBalance() == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                // Case 3c
                else if(n->getBalance() == 1)
                {
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
    else // p is the right child
    {
        g->updateBalance(1);

        // Case 1
        if(g->getBalance() == 0) return;
        // Case 2
        else if(g->getBalance() == 1) insert_fix(g, p);
        // Case 3
        else if(g->getBalance() == 2)
        {
            if((isLeftChild(p) && isLeftChild(n)) || (isRightChild(p) && isRightChild(n))) // zig-zig check
            {
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else if((isLeftChild(p) && isRightChild(n)) || (isRightChild(p) && isLeftChild(n))) // zig-zag check
            {
                rotateRight(p);
                rotateLeft(g);
                
                // Case 3a
                if(n->getBalance() == 1)
                {
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                // Case 3b
                else if(n->getBalance() == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                // Case 3c
                else if(n->getBalance() == -1)
                {
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    signed char diff = 0;

    AVLNode<Key, Value>* removed_node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(removed_node == NULL) return;
    
    AVLNode<Key, Value>* parent = removed_node->getParent();
    bool removing_root = false;
    if(parent == NULL) removing_root = true; // if the node doesn't have a parent, it's the root
    /*else
    {
        if(isLeftChild(removed_node)) 
        {
            diff = 1;
        }
        else if(isRightChild(removed_node)) 
        {
            diff = -1;
        }
    }*/

    // CASE 1: 2 children
    if(removed_node->getLeft() != NULL && removed_node->getRight() != NULL)
    {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(removed_node));
        nodeSwap(removed_node, pred);
        parent = removed_node->getParent();

        if(removing_root)
        {
            this->root_ = pred;
            removing_root = false; // after swap, we are no longer removing root
        }
    }
    // now the 2 child case is either 0 children or 1 child case

    // CASE 2: 0 children
    if(removed_node->getLeft() == NULL && removed_node->getRight() == NULL)
    {
        if(removing_root == false) // there is a parent to deal with
        {
            if(parent->getLeft() == removed_node) // removed_node is the left child of parent
            {
                parent->setLeft(NULL);
                diff = 1;
            }
            else
            {
                parent->setRight(NULL);
                diff = -1;
            }
        }
        else // we are erasing the root
        {
            this->root_ = NULL;
        }
        delete removed_node;
        remove_fix(parent, diff);
    }
    // CASE 3: 1 child
    else if((removed_node->getLeft() == NULL && removed_node->getRight() != NULL) || (removed_node->getLeft() != NULL && removed_node->getRight() == NULL))
    {
        Node<Key, Value>* child;
        // find the one child
        if(removed_node->getLeft() != NULL) child = removed_node->getLeft();
        else child = removed_node->getRight();

        if(removing_root == false) // there is a parent to deal with
        {
            if(parent->getLeft() == removed_node) // removed_node is on the left
            {
                parent->setLeft(child);
                child->setParent(parent);
                diff = 1;
            }
            else // removed_node is on the right
            {
                parent->setRight(child);
                child->setParent(parent);
                diff = -1;
            }
        }
        else // we are erasing the root
        {
            this->root_ = child;
            child->setParent(NULL);
        }
        delete removed_node;
        remove_fix(parent, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key, Value>* n, signed char diff)
{
    // TODO
    signed char ndiff = 0;
    if(n == NULL) return;

    AVLNode<Key, Value>* p = n->getParent();
    if(p != NULL)
    {
        if(isLeftChild(n)) ndiff = 1;
        else ndiff = -1;
    }

    if(diff == -1)
    {
        // Case 1
        if(n->getBalance() + diff == -2)
        {
            AVLNode<Key, Value>* c = n->getLeft(); // c is the taller child
            // Case 1a
            if(c->getBalance() == -1)
            {
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                remove_fix(p, ndiff);
            }
            // Case 1b
            else if(c->getBalance() == 0)
            {
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            }
            // Case 1c
            else if(c->getBalance() == 1)
            {
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if(g->getBalance() == 1)
                {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if(g->getBalance() == -1)
                {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                remove_fix(p, ndiff);
            }
        }
        // Case 2
        else if(n->getBalance() + diff == -1)
        {
            n->setBalance(-1);
        }
        // Case 3
        else if(n->getBalance() + diff == 0)
        {
            n->setBalance(0);
            remove_fix(p, ndiff);
        }
    }
    else if(diff == 1)
    {
        // Case 1
        if(n->getBalance() + diff == 2)
        {
            AVLNode<Key, Value>* c = n->getRight(); // c is the taller child
            // Case 1a
            if(c->getBalance() == 1)
            {
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                remove_fix(p, ndiff);
            }
            // Case 1b
            else if(c->getBalance() == 0)
            {
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
            }
            // Case 1c
            else if(c->getBalance() == -1)
            {
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);
                if(g->getBalance() == -1)
                {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 1)
                {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                remove_fix(p, ndiff);
            }
        }
        // Case 2
        else if(n->getBalance() + diff == 1)
        {
            n->setBalance(1);
        }
        // Case 3
        else if(n->getBalance() + diff == 0)
        {
            n->setBalance(0);
            remove_fix(p, ndiff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    signed char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* parent = node->getParent();
    if(parent->getLeft() == node) return true;
    else return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChild(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* parent = node->getParent();
    if(parent->getRight() == node) return true;
    else return false;
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    // done?
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* right_child = node->getRight();
    AVLNode<Key, Value>* right_left_child = right_child->getLeft();

    if(parent == NULL) // i.e. we're working with the root node
    {
        this->root_ = right_child;
        right_child->setParent(NULL);
    }
    else // there is a parent node to work with
    {
        if(isRightChild(node)) // node is the right child of its parent?
        {
            parent->setRight(right_child);
        }
        else
        {
            parent->setLeft(right_child);
        }

        right_child->setParent(parent);
    }

    // connect node with its right child's left child
    node->setRight(right_left_child);
    if(right_left_child != NULL) right_left_child->setParent(node);

    // make node's right child its parent
    right_child->setLeft(node);
    node->setParent(right_child);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    // done?
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* left_child = node->getLeft();
    AVLNode<Key, Value>* left_right_child = left_child->getRight();

    if(parent == NULL) // i.e. we're working with the root node
    {
        this->root_ = left_child;
        left_child->setParent(NULL);
    }
    else // there is a parent node to work with
    {
        if(isRightChild(node)) // node is the right child of its parent?
        {
            parent->setRight(left_child);
        }
        else
        {
            parent->setLeft(left_child);
        }

        left_child->setParent(parent);
    }

    // connect node with its left child's right child
    node->setLeft(left_right_child);
    if(left_right_child != NULL) left_right_child->setParent(node);

    // make node's left child its parent
    left_child->setRight(node);
    node->setParent(left_child);
}

#endif
