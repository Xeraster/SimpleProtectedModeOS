#ifndef LINKEDLIST_H
#define LINKEDLIST_H

class listNode
{
    public:
        //the default constructor
        listNode();

        /*the constructor used by linkedList to create new valid listNode objects
        @param sequencenNumber the numerical position of where this node is in the list
        @param name the name to assign to this node
        @param serviceRequired what to set the m_serviceRequired meer to, default is "N/A"
        */
        listNode(int sequenceNumber, string name, string serviceRequired = "N/A");


        unsigned int m_sequenceNumber;
        string m_name;
        int m_month;
        int m_day;
        int m_year;
        int m_hour;
        int m_minute;
        string m_serviceRequired;
        listNode *m_next;

    private:
        //uses the system clock to apply the current date and time to this listNode
        void fetchTime();

};

class linkedList
{
    public:
        //linked list default constructor
        linkedList();

        ~linkedList();

        //insert node with defaults
        void insertNode(string customerName = "unnamed", string serviceRequired = "N/A");
        bool deleteNode(string customerName);

        /*returns a pointer to the last node whether m_last contains valid data or not. Only retrurns useful data if numItems > 0, otherwise it returns nullptr. 
        Plenty of opportunities for crash avoidance if this code is used in other things, although that probably won't matter in this assignment (i'm putting this in my homebrew operating system later)*/
        listNode *findLastNode();

        //returns the number of listNode objects in this linkedList
        unsigned int numItems();

        listNode *searchNode(string customerName);

        //returns whichever node is before the input node
        listNode *getPrevNode(listNode *node);

        listNode *getNodeByIndex(unsigned int indexPos);

        //goes through the list and fixes every node that has an incorrect sequence number
        void fixSequenceNumbers();

        void serveCustomer();

        void listAll();
        
    private:
        listNode *m_start;
        listNode *m_last;
};

void printNode(const listNode& node);

//ostream &operator<<(ostream& stream, const listNode& node);


#include "linkedList.hpp"
#endif