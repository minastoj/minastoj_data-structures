#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>

// Use inheritance from std::vector (choose public/private) as appropriate
template <typename T>
class Stack 
{
public:
    Stack();
    ~Stack();
    bool empty() const;
    size_t size() const;
    void push(const T& item);
    void pop();  // throws std::underflow_error if empty
    const T& top() const; // throws std::underflow_error if empty
    // Add other members only if necessary
private:
    std::vector<T> data;
};

template <typename T>
Stack<T>::Stack()
{

}

template <typename T>
Stack<T>::~Stack()
{
    for(int i = 0; i < (int)data.size(); i++)
    {
        data.pop_back();
    }
}

template <typename T>
bool Stack<T>::empty() const
{
    return (size() == 0);
}

template <typename T>
size_t Stack<T>::size() const
{
    return data.size();
}

template <typename T>
void Stack<T>::push(const T& item)
{
    data.push_back(item);
}

template <typename T>
void Stack<T>::pop()
{
    if(data.size() == 0)
    {
        throw std::underflow_error("Stack is empty, nothing to pop");
    }
    return data.pop_back();
}

template <typename T>
const T& Stack<T>::top() const
{
    if(data.size() == 0)
    {
        throw std::underflow_error("Stack is empty, no values present");
    }
    return data[data.size() - 1];
}

#endif