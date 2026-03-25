listNode :: listNode()
{
    fetchTime();

    //set it up with some default values
    m_sequenceNumber = 0;
    m_name = "unnamed";
    m_next = nullptr;
}

/*the constructor used by linkedList to create new valid listNode objects
@param sequencenNumber the numerical position of where this node is in the list
@param name the name to assign to this node
@param serviceRequired what to set the m_serviceRequired meer to, default is "N/A"
*/
listNode :: listNode(int sequenceNumber, string name, string serviceRequired)
{
    fetchTime();
    m_serviceRequired = serviceRequired;
    m_name = name;
    m_sequenceNumber = sequenceNumber;
    m_next = nullptr;
}

//uses the system clock to apply the current date and time to this listNode
void listNode :: fetchTime()
{
    //get the current time and record it into the correct memeber variables
    /*std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
    m_year = now->tm_year + 1900;
    m_month = now->tm_mon + 1;
    m_day = now->tm_mday;
    m_hour = now->tm_hour;
    m_minute = now->tm_min;*/

    //I dont have a rtc driver in c++ os yet
}

void printNode(const listNode& node)
{
    printString("name: ", 0x0F);
    printString(node.m_name, 0x0F);
    printString(". Service required: ", 0x0F);
    printString(node.m_serviceRequired , 0x0F);
    printString(".", 0x0F);
    consoleNewLine();
    //printInt()
    //string msg1 = "name: " + node.m_name + ". Service required: " + node.m_serviceRequired + " ";
    //string msg2 = to_string(node.m_month) << "/" << to_string(node.m_day) << "/" << to_string(node.m_year) << " ";
    //stream << to_string(node.m_hour) << ":" << to_string(node.m_minute); 
    //return stream;
}

/*ostream &operator<<(ostream& stream, const listNode& node)
{
    stream << "name: " << node.m_name << ". Service required: " << node.m_serviceRequired << " ";
    stream << to_string(node.m_month) << "/" << to_string(node.m_day) << "/" << to_string(node.m_year) << " ";
    stream << to_string(node.m_hour) << ":" << to_string(node.m_minute); 
    return stream;
}*/

//linked list default constructor
linkedList :: linkedList()
{
    m_start = nullptr;
    m_last = nullptr;
}

//delete the entire contents of the list and all the nodes. Be sure to deallocate them from memory to avoid memory leaks
linkedList :: ~linkedList()
{
    while(m_start != nullptr)
    {
        //delete the last node
        deleteNode(m_last->m_name);

        //recalculate the new last node
        m_last = findLastNode();
    }
}

/*inserts a node into the linkedList
@param customerName the name of the customer to insert into the list
@param serviceRequired the service that the customer is requesting
*/
void linkedList :: insertNode(string customerName, string serviceRequired)
{
    int insertPosition = numItems();
    if (insertPosition == 0)
    {
        //cout << "numItems() is 0, doing start case" << endl;
        //oh yeah, I don't have a new keyword either
        m_start = (listNode*)malloc(sizeof(listNode));
        *m_start = listNode(insertPosition, customerName, serviceRequired);
        
        //if the size is actualyl zero, m_last is the same as m_start
        m_last = m_start;
    }
    else
    {
        listNode *n = findLastNode();
        //n->m_next = new listNode(insertPosition, customerName, serviceRequired);
        n->m_next = (listNode*)malloc(sizeof(listNode));
        *n->m_next = listNode(insertPosition, customerName, serviceRequired);
        m_last = n->m_next;//don't forget to set a last node
    }
}

/*deletes whichever node has the same name as this one, if it exists. returns true if the requested node was deleted, returns false if the node with the requested name was not found
@param customerName the name of the customer node that you want to delete
*/
bool linkedList :: deleteNode(string customerName)
{
    //first, attempt to find the requested node
    listNode *node = searchNode(customerName);

    //if it was found, delete the node
    if (node != nullptr)
    {
        //cout  << "actually found the node" << endl;
        //listNode *prev = getPrevNode(node);

        if (node == m_start)
        {
            m_start = node->m_next;

            if (m_start != nullptr)
            {
                m_start->m_sequenceNumber = 0;
            }
            
            //delete node;    //no memory leaks
            free(node);

            //fix sequence numbers
            fixSequenceNumbers();
        }
        else
        {
            listNode *prev = getNodeByIndex(node->m_sequenceNumber-1);
            //cout << "got previous node" << endl;
            
            if (node->m_next != nullptr)
            {
                prev->m_next = node->m_next;

                //delete node;    //no memory leaks
                free(node);

                //fix sequence numbers
                fixSequenceNumbers();
            }
            else
            {
                //if it got here and m_next == nullptr, then this is probably the last node
                prev->m_next = nullptr;

                //delete node;
                free(node);
            }
        }

        //whatever happens, make sure m_last always contains valid data
        m_last = findLastNode();
        return true;

        /*if (prev != nullptr && findLastNode() != node)
        {
            prev->m_next = node->m_next;    //set the previous node next node to the node's next node
            cout << "next previous node" << endl;
        }

        //special case for when you delete the beginnin start node
        if (m_start == node)
        {
            m_start = node->m_next;
        }

        //if last node got changed, update it
        if (m_last == node)
        {
            m_last = findLastNode();
        }
        delete node;    //delete the node from memory
        return true;*/
    }
    else
    {
        //if the node wasn'tfound, return false
        return false;
    }
}

//returns a pointer to the last node whether m_last contains valid data or not. almost completely foolproof
listNode* linkedList :: findLastNode()
{
    listNode *next = m_start;
    if (next != nullptr)
    {
        while (next->m_next != nullptr)
        {
            next = next->m_next;
        }
    }

    return next;
}

unsigned int linkedList :: numItems()
{
    if (m_start == nullptr)
    {
        return 0;
    }
    else
    {
        listNode *next = m_start;
        int ctr = 0;
        while (next != nullptr)
        {
            next = next->m_next;
            ctr++;
        }
        //cout << "numItems() = " << ctr << endl;
        return ctr;
    }
}

listNode* linkedList :: searchNode(string customerName)
{
    listNode *nodeToReturn = nullptr;
    listNode *next = m_start;
    bool foundMatch = false;
    if (next != nullptr)
    {
        while (next != nullptr && !foundMatch)
        {
            if (next->m_name == customerName)
            {
                nodeToReturn = next;
                foundMatch = true;
            }
            else
            {
                next = next->m_next;
            }
        }
    }

    //cout << "address of last node = " << next << endl;;

    return nodeToReturn;
}

//returns whichever node is before the input node
listNode* linkedList :: getPrevNode(listNode *node)
{
    //you can't get the previous node of the starting node
    if (m_start == node)
    {
        return nullptr;
    }

    listNode *nodeToReturn = nullptr;
    listNode *next = m_start;
    bool foundMatch = false;
    if (next != nullptr)
    {
        while (next->m_next != nullptr && !foundMatch)
        {
            if (next->m_next == node)
            {
                nodeToReturn = next;
                foundMatch = true;
                //cout << "found prev node name = " << nodeToReturn->m_name << endl;
            }
            else
            {
                next = next->m_next;
            }
        }
    } 

    return nodeToReturn;
}

listNode* linkedList :: getNodeByIndex(unsigned int indexPos)
{
    listNode *nodeToReturn = nullptr;
    listNode *next = m_start;
    bool foundMatch = false;
    if (next != nullptr)
    {
        while (next->m_next != nullptr && !foundMatch)
        {
            if (next->m_sequenceNumber == indexPos)
            {
                nodeToReturn = next;
                foundMatch = true;
            }
            else
            {
                next = next->m_next;
            }
        }
    }

    return nodeToReturn;
}

//goes through the list and fixes every node that has an incorrect sequence number
void linkedList :: fixSequenceNumbers()
{
    //the sequence numbers aren't actually used for anything, but its in the program instructions so i keep them updated
    int correctNumber = 0;
    listNode *node = m_start;
    while (node != nullptr)
    {
        node->m_sequenceNumber = correctNumber;
        correctNumber++;
        node = node->m_next;
    }
}

void linkedList :: serveCustomer()
{
    //delete the node at the end of the list the easiest way
    string name = m_last->m_name;
    bool result = deleteNode(name);
    if (result)
    {
        //cout << "The customer named " << name << " has been served." << endl;
    }
    else
    {
        //cout << "ERROR: tried to serve a customer that isn't present." << endl;
    }
}

void linkedList :: listAll()
{
    consoleNewLine();
    listNode *next = m_start;
    while (next != nullptr)
    {
        //use ostream overload to easily print the list nodes
        //cout << *next << endl;
        printNode(*next);
        next = next->m_next;
    }

    consoleNewLine();

    printString("There are ", 0x0F);
    printInt(numItems(), 0x0F);
    printString(" customers waiting to be served", 0x0F);
    consoleNewLine();
}