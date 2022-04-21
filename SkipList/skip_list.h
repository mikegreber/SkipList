/*
 * Skip List implementation for University of Alberta CMPUT 403 final project Winter 2022.
 * 
 * Based on description from https://en.wikipedia.org/wiki/Skip_list.
 * A skip list is an ordered sequence data structure with O(logn) running time for Insert(), Remove(), and Contains().
 * Advantages are fast searching like an ordered array, combined with fast insertion/deletion like a linked list.
 *
 * Should work with any type T that defines <,>, ==, and !=.
 * 
 * Author: Mike Greber
 */

#pragma once

#include <vector>
#include <iostream>
#include <cassert>

#include "sorted.h"


/* node for use in with skip_list */
template <typename T>
struct skip_list_node
{
    skip_list_node() : val(0), next(nullptr), prev(nullptr), down(nullptr){}
    
    skip_list_node(T val, skip_list_node* next = nullptr, skip_list_node* prev = nullptr, skip_list_node* down = nullptr)
        : val(val), next(next), prev(prev), down(down) {}
    
    T val;
    skip_list_node* next;
    skip_list_node* prev;
    skip_list_node* down;
};


/*
 * Skip List implementation, based on https://en.wikipedia.org/wiki/Skip_list. Randomly inserts in new nodes into higher layers.
 */
template <typename T>
class skip_list : public sorted<T>
{
public:

    // Constructor
    skip_list(float p = 0.5);

    // Copy constructor
    skip_list(const skip_list& other);

    // Move constructor
    skip_list(skip_list&& other) noexcept;

    // Assignment
    skip_list& operator=(const skip_list& other);

    // Move assignment
    skip_list& operator=(skip_list&& other) noexcept;

    // Destructor
    ~skip_list() override;

    // return name of list
    std::string GetName() const override { return "skip list"; }
    
    // returns true if list contains val
    bool Contains(T val) override;

    // insert val into its sorted position in the list
    void Insert(T val) override;

    // remove val from list, returns false if val not in list
    bool Remove(T val) override;

    // removes all elements form the list
    void Clear() override;
    
    // returns the number of elements in the list
    size_t Size() const override { return size_; }

    // return list as a vector
    std::vector<T> AsVector() const override;

    // print the skip list to standard output. If internal_representation is true, all layers will be displayed
    void Print(bool internal_representation = false);
private:
    std::vector<skip_list_node<T>*> layers_;
    size_t size_;
    float p_;

    // finds the first node matching val in any layer, starting search from highest layer
    skip_list_node<T>* Find(T val, int& layer);
};


/* Skip List. p is the probability (must be in range [0,1]) that an inserted element will be inserted into a higher layer. */
template <typename T>
skip_list<T>::skip_list(float p) : size_(0), p_(p)
{
    assert(p >= 0 && p <= 1);
}

/* Copy constructor */
template <typename T>
skip_list<T>::skip_list(const skip_list& other)
{
    auto current = other.layers_[0];
    p_ = other.p_;
    while (current)
    {
        skip_list<T>::Insert(current->val);
        current = current->next;
    }
}

/* Move copy constructor */
template <typename T>
skip_list<T>::skip_list(skip_list&& other) noexcept: layers_(std::move(other.layers_)),size_(other.size_), p_(other.p_) {}

/* Assignment */
template <typename T>
skip_list<T>& skip_list<T>::operator=(const skip_list& other)
{
    skip_list<T>::Clear();
    auto node = other.layers_[0];
    while (node)
    {
        Insert(node->val);
        node = node->next;
    }
    return *this;
}

/* Move Assignment */
template <typename T>
skip_list<T>& skip_list<T>::operator=(skip_list&& other) noexcept
{
    if (this == &other)
        return *this;
    layers_ = std::move(other.layers_);
    size_ = other.size_;
    p_ = other.p_;
    return *this;
}

/* Destructor */
template <typename T>
skip_list<T>::~skip_list()
{
    skip_list<T>::Clear();
}

/*
 * returns true if val is in the list, false otherwise
 */
template <typename T>
bool skip_list<T>::Contains(T val)
{
    int layer;
    return Find(val, layer);   
}

/*
 * Inserts val in its sorted position in the skip list
 */
template <typename T>
void skip_list<T>::Insert(T val)
{
    // increment size (will not fail)
    ++size_;

    // first element
    if (layers_.empty())
    {
        layers_.push_back(new skip_list_node<T>(val));
        return;
    }
    
    // start at highest layer
    auto layer = layers_.size() - 1;
    skip_list_node<T>* current = layers_[layer];
    
    // move down start of layers if val smaller than current
    while (layer > 0 && current->val > val)
        current = layers_[--layer];

    // cache how to go back up where we went down
    std::vector<skip_list_node<T>*> up;
    
    while (true)
    {
        // search current layer while value is less than current
        while (current->next && current->next->val <= val) current = current->next;

        // go down a layer if there is one
        if (current->down)
        {
            --layer;
            up.push_back(current);
            current = current->down;
        }
        else break;
    }
    
    skip_list_node<T>* new_node;

    // insert at front
    if (current->val > val) layers_[0] = new_node = new skip_list_node<T>(val, current);

    // insert inside list
    else new_node = new skip_list_node<T>(val, current->next, current);

    // reset neighboring links
    if (new_node->prev) new_node->prev->next = new_node;
    if (new_node->next) new_node->next->prev = new_node;

    // randomly add node to higher layers
    auto max_layer = static_cast<unsigned>(floor(std::log(size_)));
    while (++layer <= max_layer)
    {
        // flip a coin on adding this to the next level (probability based on p)
        if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) <= p_)
        {
            // add to after cached node in layer up
            if (!up.empty())
            {
                auto node = up.back(); up.pop_back();
                
                new_node = new skip_list_node<T>(val, node->next, node, new_node);

                // fix neighboring node links
                new_node->prev->next = new_node;
                if (new_node->next) new_node->next->prev = new_node;
            }

            // add to new higher layer if needed
            else if (layers_.size() <= layer)
            {
                layers_.push_back(new_node = new skip_list_node<T>(val, nullptr, nullptr, new_node));
            }

            // add to start of layer
            else
            {
                new_node = layers_[layer] = new skip_list_node<T>(val, layers_[layer], nullptr, new_node);
                new_node->next->prev = new_node;
            }
                
        }
        else break;
    }
}

/*
 * removes the first element matching val from the skip list.
 * returns true if successful, false if val isn't in the list.
 */
template <typename T>
bool skip_list<T>::Remove(T val)
{
    int layer;
    auto current = Find(val, layer);
    if (current)
    {
        // remove node and any down nodes connected
        do 
        {
            auto node = current;
            current = current->down;

            // adjust links
            if (node->next) node->next->prev = node->prev;
            if (node->prev) node->prev->next = node->next;
            else
            {
                layers_[layer] = node->next;
                if (!layers_.back()) layers_.pop_back();
            }
        
            --layer;
            delete node;
        } while (current);

        --size_;
        return true;
    }
    
    return false;
}

/*
 * Removes all elements from the list.
 */
template <typename T>
void skip_list<T>::Clear()
{
    if (size_ == 0) return;
        
    for (auto node : layers_)
    {
        while (node)
        {
            auto next = node->next;
            delete node;
            node = next;
        }
    }
    
    layers_.clear();
    size_ = 0;
}

/*
 * Prints the skip_list.
 * Prints all layers if internal_representation is true, otherwise only the lowest layer is displayed.
 */
template <typename T>
void skip_list<T>::Print(bool internal_representation)
{
    const int n = internal_representation ? static_cast<int>(layers_.size()) : 1;

    if (internal_representation && size_ == 0) std::cout << " Empty" << std::endl;
    
    for (int i = 0; i < n; ++i)
    {
        if (internal_representation) std::cout << " Layer " << i << ":";
        
        auto current = layers_[i];
        while (current)
        {
            std::cout << " " << current->val;
            current = current->next;
        }
        std::cout << std::endl;
    }
    if (internal_representation) std::cout << " Size: " << size_ << std::endl;
    std::cout << std::endl;
}

/*
 * Finds and returns the first node matching val in any layer, searching from highest layer.
 * layer will contain the layer of a returned node
 * returns null if val is not in the list
 */
template <typename T>
skip_list_node<T>* skip_list<T>::Find(T val, int& layer)
{
    // is list empty
    if (size_ == 0) return nullptr;
    
    // start at highest layer
    layer = static_cast<int>(layers_.size()) - 1;
    skip_list_node<T>* current = layers_[layer];
    
    // move down start of layers if val is smaller than first in layer
    while (layer > 0 && current->val > val)
        current = layers_[--layer];
    
    // smaller than min, not in list
    if (current->val > val) return nullptr;

    // search through each layer until we find the value
    while (current)
    {
        // search current layer while value is less than current
        while (current->next && current->next->val <= val) current = current->next;
        
        if (current->val == val || !current->down) break;

        // continue down a layer
        current = current->down;
        --layer;
    }
    
    // will have found it by now if it exists
    if (current->val == val) return current;
    
    return nullptr;
}

/*
 * returns the values of the list as a vector
 */
template <typename T>
std::vector<T> skip_list<T>::AsVector() const
{
    std::vector<T> v;
    if (size_ == 0) return v;
    v.reserve(size_);
    auto node = layers_[0];
    while (node)
    {
        v.push_back(node->val);
        node = node->next;
    }
    return v;
}
