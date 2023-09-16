#include "llrec.h"

//*********************************************
// Provide your implementation of llpivot below
//*********************************************

void llpivot(Node *&head, Node *&smaller, Node *&larger, int pivot)
{
    smaller = NULL;
    larger = NULL;
    if(head == NULL)
    {
        return;
    }

    llpivot(head->next, smaller, larger, pivot);
    
    if(head->val > pivot)
    {
        Node* temp = larger;
        larger = head;
        larger->next = temp;
    }
    else if(head->val <= pivot)
    {
        Node* temp = smaller;
        smaller = head;
        smaller->next = temp;
    }
    head = NULL;
}