#ifndef NUI_GADGET_NODE_H_
#define NUI_GADGET_NODE_H_

#include <nui/nui.h>
#include <vector>

namespace nui
{

template<typename T>
class Node
{
public:
    Node(T obj) : obj_(obj), parent_(nullptr)
    {

    }
    Node() : obj_(0), parent_(nullptr)
    {

    }

    Node * parent() const
    {
        return parent_;
    }

    void Reset(T obj)
    {
        obj_ = obj;
    }

    T Get() const
    {
        return obj_;
    }

    void AttachChild(Node * child, uint32_t index)
    {
        if (!child)
            return;
        auto iter = std::find(children_.begin(), children_.end(), child);
        if (iter != children_.end())
            return;

        if (children_.size() < index && index != -1)
            return;

        child->DetachFromParent();
        child->parent_ = this;
        if (index == -1)
        {
            this->children_.push_back(child);
        }
        else
        {
            iter = children_.begin();
            iter += index;
            children_.insert(iter, child);
        }
    }

    void DetachChild(Node * child)
    {
        auto iter = std::find(children_.begin(), children_.end(), child);
        if (iter != children_.end())
        {
            children_.erase(iter);
            child->parent_ = nullptr;
        }
    }

    void DetachAllChildren()
    {
        for (auto iter = children_.begin(); iter != children_.end(); ++iter)
        {
            (*iter)->parent_ = nullptr;
        }
        children_.clear();
    }

    void DetachFromParent()
    {
        if (parent_)
            parent_->DetachChild(this);
    }

    size_t GetChildrenCount() const
    {
        return children_.size();
    }

    Node * GetChild(uint32_t index) const
    {
        Node * child = nullptr;
        if (children_.size() > index)
            child = children_[index];
        return child;
    }
private:
    T obj_;
    Node * parent_;
    std::vector<Node *> children_;
};


}



#endif