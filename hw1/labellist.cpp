#include "labellist.h"
#include <cassert>

// Complete - Do not alter
LabelList::MsgNode::MsgNode(const MsgType& msg)
    : msg_(msg)
{
    // Every message is part of the "all" (index 0) label
    labelled_.push_back(true);
    next_.push_back(nullptr);
    prev_.push_back(nullptr);
}

// Complete - Do not alter
LabelList::MsgToken::MsgToken()
    : node_(nullptr), list_(nullptr)
{
}

// COMPLETE
LabelList::MsgToken::MsgToken(MsgNode* node, LabelList* list)
// Add any initializers here
    : node_(node), list_(list)
{

}

// COMPLETE
bool LabelList::MsgToken::operator==(const MsgToken& rhs) const
{
    // if both tokens point to the same node, on the same list, they are identical
    return ((this->node_ == rhs.node_) && (this->list_ == rhs.list_));
}

// Complete - Do not alter
bool LabelList::MsgToken::operator!=(const MsgToken& rhs) const
{
    return !operator==(rhs);
}

// COMPLETE
LabelList::MsgToken::operator bool() const
{
    // if the token is valid, it means it can be used as a boolean to represent true
    // if it's not valid, it equates to false
    return (this->isValid());
}

// Complete - Do not alter
bool LabelList::MsgToken::isValid() const
{
    return this->node_ != nullptr;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::next(const LabelType& label)
{
    size_t index = this->list_->findLabelIndex(label);
    if( false == this->isLabelMember(index) ) 
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->next_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::prev(const LabelType& label)
{
    size_t index = this->list_->findLabelIndex(label);
    if( false == this->isLabelMember(index) ) 
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->prev_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(const LabelType& label) const
{
    size_t index = this->list_->findLabelIndex(label);
    return isLabelMember(index);
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(size_t index) const
{
    return this->isValid() && 
            index != INVALID_LABEL &&
            index < this->node_->labelled_.size() &&
            this->node_->labelled_[index] ;
}

// COMPLETE
LabelList::MsgType const & LabelList::MsgToken::msg() const
{
    if(this->isValid())
    {
        // return the message from the node that this token points to
        return this->node_->msg_;
    }
    else
    {
        throw std::logic_error("Token references an invalid message");
    }
}
// COMPLETE
LabelList::MsgType& LabelList::MsgToken::msg() 
{
    if(this->isValid())
    {
        // return the message from the node that this token points to
        return this->node_->msg_;
    }
    else
    {
        throw std::logic_error("Token references an invalid message");
    }
}

// To be completed - must adhere to any requirements in the .h file
std::ostream& operator<<(std::ostream& ostr, const LabelList::MsgToken& token)
{
    if(token.isValid())
    {
        ostr << token.node_->msg_;
    }
    return ostr;
}

// Static constant 
const LabelList::MsgToken LabelList::end_(nullptr, nullptr);

// COMPLETE
LabelList::LabelList() 
// Add any initializers here
{
    this->heads_.push_back(nullptr); // first index of heads_ points to nothing yet
    this->labels_.push_back("all"); // creating the "all" label
    this->tails_.push_back(nullptr); // first index of tails_ points to nothing yet
}

// To be completed - must adhere to any requirements in the .h file
LabelList::~LabelList()
{
    // N O T   D O N E
    // FIRST NEED TO DELETE ALL THE MSGNODES
    
    clear(); // clear the list so it holds nothing
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::clear()
{
    // loop through the MsgNodes and remove each one until the list is empty and at its beginning state
    // beginning state: no nodes, only one label ("all"), only one index of ptrs, all pointing to nullptr
    MsgNode* curr = heads_[0];
    while(curr != nullptr)
    {
        remove(MsgToken(curr, this)); // remove the current node we're pointing to

        curr = curr->next_[0]; // increment curr
    }
}

// To be completed - must adhere to any requirements in the .h file
bool LabelList::empty() const
{
    // if the heads and tails ptrs for "all" are null, there are no messages
    return (heads_[0] == nullptr && tails_[0] == nullptr);
}

// Complete
size_t LabelList::size() const
{
    MsgNode* n = this->heads_[0];
    size_t cnt = 0;
    while(n != nullptr){
        cnt++;
        n = n->next_[0];
    }
    return cnt;
}

// COMPLETE
LabelList::MsgToken LabelList::add(const MsgType& msg)
{
    MsgNode* curr = new MsgNode(msg); // dynamically allocate a msgnode

    // if head ptr is empty for all (0th level), then there are no messages yet
    if(this->heads_[0] == nullptr)
    {
        // doesn't point to anything before it
        curr->prev_[0] = nullptr;
        // doesn't point to anything after it
        curr->next_[0] = nullptr; 
        // make heads_ and tails_ point to new_message
        this->heads_[0] = curr;
        this->tails_[0] = curr;
    }
    else if (this->heads_[0] != nullptr) // attach it to the last one in "all"
    {
        // doesn't point to anything after it bc its last in line
        curr->next_[0] = nullptr; 
        // make the "next" pointer of last node point to curr
        this->tails_[0]->next_[0] = curr;
        // make curr point to the second to last one via prev
        curr->prev_[0] = tails_[0];
        // make tails_ point to curr now because it's the new last node
        this->tails_[0] = curr;
    }

    // create a message token to be returned
    MsgToken token(curr, this);
    return token;
}


// To be completed - must adhere to any requirements in the .h file
void LabelList::remove(const MsgToken& token)
{
    if(!token.isValid()) return; // simply return if token isn't valid

    // determine which of the labels the node held by token belongs to
    // O(|L|)
    for(int i = 0; i < (int)labels_.size(); i++)
    {
        // if token belongs to a label, unlabel it
        if(token.node_->labelled_[i] == true)
        {
            LabelType label = labels_[i]; // keeps track of what the current label is
            // if label == "all", there should be no change
            if(label != "all")
            {
                // this is just the unlabel() code without the part where you look for the index of the level
                MsgNode* left_most = token.node_->prev_[i]; // pointer to mark the left most node before the node that will be removed from label
                MsgNode* right_most = token.node_->next_[i]; // pointer to mark the right most node before the node that will be removed from label
                token.node_->labelled_[i] = 0; // the specified node no longer participates in the label

                if(left_most != nullptr && right_most != nullptr) // neither of the ptrs are nullptrs means they exist
                {
                    // make prev of removed node point to nothing
                    token.node_->prev_[i] = nullptr;
                    // make next of removed node point to nothing
                    token.node_->next_[i] = nullptr;

                    // make the left most node point to the right_most node
                    left_most->next_[i] = right_most;
                    // make the right most node point to the left_most node
                    right_most->prev_[i] = left_most;
                }
                else if(left_most == nullptr && right_most != nullptr) // left most doesn't exist, but right most does = removed node will be at the front
                {
                    // make next of removed node point to nothing
                    token.node_->next_[i] = nullptr;
                    // make the right most node point to nothing before it
                    right_most->prev_[i] = nullptr;
                    // make the heads_ ptr point to the right_most node
                    heads_[i] = right_most;
                }
                else if(left_most != nullptr && right_most == nullptr) // left most does exist, but right most doesn't = removed node will be at the end
                {
                    // make prev of removed node point to nothing
                    token.node_->prev_[i] = nullptr;
                    // make the left most node point to nothing
                    left_most->next_[i] = nullptr;
                    // make the tails_ ptr point to the left_most node
                    tails_[i] = left_most;
                }
                else // both left_most and right_most are nullptrs = this is the last node in the label, so the head and tail ptrs should be null again
                {
                    heads_[i] = nullptr;
                    tails_[i] = nullptr;
                }
            }
        }
    }
}

// COMPLETE
void LabelList::label(const MsgToken& token, const LabelType& label)
{
    if(!token.isValid())
    {
        throw std::invalid_argument("Bad token in label()");
    }

    // loop through the labels and check if the label you need exists
    // (there can never be more labels than nodes, so maximally there are n labels, making this O(n))
    bool label_exists = false; // start with it being false
    int loc_label = 0; // keeps track of location of label

    for(int i = 0; i < (int)labels_.size(); i++)
    {
        if(label == labels_[i])
        {
            label_exists = true;
            loc_label = i;
            break; // break the for loop once you've found it
        }
    }

    // case 1: label doesn't exist already
    if(!label_exists)
    {    
        // make the node in question be part of the label
        token.node_->labelled_.push_back(1);
        // make the node at that level not point to anything prev_ or next_
        token.node_->prev_.push_back(nullptr);
        token.node_->next_.push_back(nullptr);

        // add a new label
        labels_.push_back(label);
        // make heads_ and tails_ ptrs point to the node in question
        heads_.push_back(token.node_);
        tails_.push_back(token.node_);

        MsgNode* temp = heads_[0]; // create a temp ptr that points to the first node in "all"
        // for all other nodes, make them not part of that label & make them point to nothing
        // another O(n) loop
        while(temp->next_[0] != nullptr)
        {   
            if(temp != token.node_) // if temp hasn't landed on newly labelled node, push value of 0 and nullptrs
            {  
                temp->labelled_.push_back(0);
                temp->prev_.push_back(nullptr);
                temp->next_.push_back(nullptr);
            }

            temp = temp->next_[0]; // increment the temp
        }
    }
    else if(label_exists && heads_[loc_label] == nullptr) // case 2: label exists already but it's empty
    {
        // make the node in question be part of the label
        token.node_->labelled_[loc_label] = true;
        // make the node at that level not point to anything prev_ or next_
        token.node_->prev_[loc_label] = nullptr;
        token.node_->next_[loc_label] = nullptr;

        heads_[loc_label] = token.node_; // make head ptr point to token.node_
        tails_[loc_label] = token.node_; // make tail ptr point to token.node_
    }
    else if(label_exists && heads_[loc_label] != nullptr) // case 3: label exists already and it's not empty
    {
        // make the node in question be part of the label
        token.node_->labelled_[loc_label] = true;
        MsgNode* left_most = nullptr; // pointer to mark the left most node before newly labelled one that is under that same label
        MsgNode* right_most = nullptr; // pointer to mark the right most node before newly labelled one that is under that same label
        
        MsgNode* curr = heads_[0]; // pointer that will loop from the head to the newly labeled node
        // loop from the head to the newly labeled node and mark left most
        // loops from the head, and it can maximally loop through all the n nodes if the newly labelled node is at the end
        while(curr != token.node_)
        {
            // if this current node is part of that label
            if(curr->labelled_[loc_label] == true)
            {
                left_most = curr;
            }
            curr = curr->next_[0]; // increment the pointer (go right)
        }

        curr = tails_[0]; // pointer that will loop from the tail to the newly labeled node
        // loop from the tails to the newly labeled node and mark right most
        // loops from the tail, and it can maximally loop through all the n nodes if the newly labelled node is at the beginning
        while(curr != token.node_)
        {
            // if this current node is part of that label
            if(curr->labelled_[loc_label] == true)
            {
                right_most = curr;
            }
            curr = curr->prev_[0]; // decrement the pointer (go left)
        }

        if(left_most != nullptr && right_most != nullptr) // neither of the ptrs are nullptrs means they exist
        {
            // make prev of new node point to left most
            token.node_->prev_[loc_label] = left_most;
            // make the left most node point to the new node
            left_most->next_[loc_label] = token.node_;
            // make next of new node point to right most
            token.node_->next_[loc_label] = right_most;
            // make the right most node point to the new node
            right_most->prev_[loc_label] = token.node_;
        }
        else if(left_most == nullptr && right_most != nullptr) // left most doesn't exist, but right most does = new node will be at the front
        {
            // make next of new node point to right most
            token.node_->next_[loc_label] = right_most;
            // make the prev of the new node point to nullptr
            token.node_->prev_[loc_label] = nullptr;
            // make the right most node point to the new node
            right_most->prev_[loc_label] = token.node_;
            // make the heads_ ptr point to the new node
            heads_[loc_label] = token.node_;
        }
        else if(left_most != nullptr && right_most == nullptr) // left most does exist, but right most doesn't = new node will be at the end
        {
            // make prev of new node point to left most
            token.node_->prev_[loc_label] = left_most;
            // make the next of the new node point to nullptr
            token.node_->next_[loc_label] = nullptr;
            // make the left most node point to the new node
            left_most->next_[loc_label] = token.node_;
            // make the tails_ ptr point to the new node
            tails_[loc_label] = token.node_;
        }
    }
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::unlabel(const MsgToken& token, const LabelType& label)
{
    if(!token.isValid())
    {
        throw std::invalid_argument("Bad token in label()");
    }
    size_t level = findLabelIndex(label);
    if(level == INVALID_LABEL)
    {
        throw std::out_of_range("The label provided is nonexistent");
    }

    // if label == "all" and the token is not a label member, there should be no change
    if(label != "all" && token.isLabelMember(level))
    {
        MsgNode* left_most = token.node_->prev_[level]; // pointer to mark the left most node before the node that will be removed from label
        MsgNode* right_most = token.node_->next_[level]; // pointer to mark the right most node before the node that will be removed from label
        token.node_->labelled_[level] = 0; // the specified node no longer participates in the label

        if(left_most != nullptr && right_most != nullptr) // neither of the ptrs are nullptrs means they exist
        {
            // make prev of removed node point to nothing
            token.node_->prev_[level] = nullptr;
            // make next of removed node point to nothing
            token.node_->next_[level] = nullptr;

            // make the left most node point to the right_most node
            left_most->next_[level] = right_most;
            // make the right most node point to the left_most node
            right_most->prev_[level] = left_most;
        }
        else if(left_most == nullptr && right_most != nullptr) // left most doesn't exist, but right most does = removed node will be at the front
        {
            // make next of removed node point to nothing
            token.node_->next_[level] = nullptr;
            // make the right most node point to nothing before it
            right_most->prev_[level] = nullptr;
            // make the heads_ ptr point to the right_most node
            heads_[level] = right_most;
        }
        else if(left_most != nullptr && right_most == nullptr) // left most does exist, but right most doesn't = removed node will be at the end
        {
            // make prev of removed node point to nothing
            token.node_->prev_[level] = nullptr;
            // make the left most node point to nothing
            left_most->next_[level] = nullptr;
            // make the tails_ ptr point to the left_most node
            tails_[level] = left_most;
        }
        else // both left_most and right_most are nullptrs = this is the last node in the label, so the head and tail ptrs should be null again
        {
            heads_[level] = nullptr;
            tails_[level] = nullptr;
        }
    }
}

// Complete - Do not alter
LabelList::MsgToken LabelList::find(const MsgType& msg, const LabelType& label)
{
    // See if the label exists, will throw if not
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while(n != nullptr)
    {
        if(n->msg_ == msg)
        {
            return MsgToken(n, this);
        }
        n = n->next_[level];
    }
    return end();
}

// NEED
// TO
// CHECK
LabelList::MsgToken LabelList::find(size_t index, const LabelType& label)
{
    // get the level of label
    size_t level = this->getLabelIndex(label);
    // throw this error if a nonexistent label is given
    if(level == INVALID_LABEL)
    {
        throw std::out_of_range("Label doesn't exist");
    }

    // if the list is empty, return a specific kind of token
    if(this->empty())
    {
        // token must have (0x0, end()) as parameters
        return MsgToken(this->heads_[level], this->end().list_);
    }

    // create a ptr that points to the head node in this label
    MsgNode* n = this->heads_[level];
    int i = 0; // number used to keep track of index
    // loop from head until you reach the index OR after n is a nullptr
    while(i <= (int)index || n->next_[level] != nullptr)
    { 
        if(i == (int)index)
        {
            // create a message token to be returned
            return MsgToken(n, this);
        }
        if(n->next_[level] == nullptr)
        {
            return MsgToken(nullptr, this->end().list_);
        }
        i++; // increment index tracker
        n = n->next_[level]; // increment pointer so when we find the right node, it's actually pointing to it
    }
    return end(); // this happens if the index is invalid
}

// Complete - Do not alter
LabelList::MsgToken const & LabelList::end() const
{
    return end_;
}

// Complete - Do not alter
void LabelList::print(std::ostream& ostr, const LabelType& label, char separator) const
{
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while(nullptr != n)
    {
        ostr << n->msg_ << separator;
        n = n->next_[level];
    }
    ostr << std::endl;
}

// COMPLETE
size_t LabelList::findLabelIndex(const LabelType& label) const
{
    // loop through nodes of labels_ vector to find the right index
    for(int i = 0; i < (int)labels_.size(); i++)
    {
        if(labels_[i] == label)
        {
            return i;
        }
    }
    return INVALID_LABEL; 
}

// Complete - Do not alter
size_t LabelList::getLabelIndex(const LabelType& label) const
{
    size_t retval = this->findLabelIndex(label);
    if(INVALID_LABEL == retval)
    {
        throw std::out_of_range("Label doesn't exist");
    }
    return retval;
}