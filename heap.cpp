# include <heap.h>
# include <hash.h>
# include <vector>
# include <cstdlib>

// Heap Class
// By: Jeffrey Wong
/* This file is used to implement a version of a binary heap where
nodes can store an object using a void pointer. Supports insertion,
deleteMin, a setKey function that functions as both decreaseKey and 
increaseKey, and a delete function for a specific id. */

heap::heap(int size):entryTable(size * 2){
    capacity = size;
    // Allocate space for the nodes (0 slot is not used)
    data.resize(capacity+1);
    filled = 0; // The heap starts off empty
}

int heap::insert(const std::string &id, int key, void *pv){
    if(filled >= capacity){return 1;} // Cannot insert if the heap is at capacity
    if(entryTable.contains(id)){return 2;} // Should not insert duplicate ids into heap
    filled++;
    int curPos = filled;
    while(curPos > 1 && key < data[curPos/2].key){
        percolateUp(curPos);
        curPos /= 2;
    }
    data[curPos].id = id;
    data[curPos].key = key;
    data[curPos].pData = pv;
    entryTable.insert(id, &data[curPos]);
    return 0;
}

int heap::setKey(const std::string &id, int key){
    bool inHeap;
    node *pn = static_cast<node *> (entryTable.getPointer(id, &inHeap));  
    if(!inHeap){return 1;} // Cannot set key of node if it its id is not in entries
    // Process key
    int diff = key - pn->key;
    pn -> key = key;
    int posCur = getPos(pn);
    node temp = *pn; // Copies the data in pn so that we can write it back to fill holes formed for percolation

    if(diff == 0){return 0;} // We don't need to do anything if the new key is the same as the old key
    else if(diff < 0){ // Key decreases, requires a percolate up
        while(posCur > 1 && key < data[posCur/2].key){
            percolateUp(posCur);
            posCur /= 2;
        }
    }
    else{ // Key increases, requires a percolate down
        while(posCur <= filled/2 && (key > data[posCur * 2].key || (filled > posCur * 2 && key > data[posCur * 2 + 1].key))){
            int child = percolateDown(posCur); // Child will be 0 if we percolated to left child and 1 if we percolated to right child
            posCur = 2 * posCur + child; // Will update posCur to the correct child node
        }        
    }
    // We should update the node pointer regardless of if we increased or decreased the key
    data[posCur] = temp;
    entryTable.setPointer(data[posCur].id, &data[posCur]);
    return 0;
}

    
int heap::deleteMin(std::string *pId, int *pKey, void *ppData){
    if(!filled){return 1;} // Cannot delete if heap is empty

    // Process values to return
    int key = data[1].key;
    if(pId != nullptr){*pId = data[1].id;}
    if(pKey != nullptr){*pKey = key;}
    if(ppData != nullptr){*(static_cast<void **> (ppData)) = data[1].pData;} // Remember ppData is a pointer to a void pointer
    entryTable.remove(data[1].id);
    node temp = data[filled];
    filled--;

    // Percolate down
    int posCur = 1;
    key = temp.key; // we move the last filled node to the root, and take its key to percolate down
    // We first check if there are parent nodes, then if left child is larger than current node
    // or if right child exists and is larger than current node
    while(posCur <= filled/2 && (key > data[posCur * 2].key || (filled > posCur * 2 && key > data[posCur * 2 + 1].key))){
        int child = percolateDown(posCur); // Child will be 0 if we percolated to left child and 1 if we percolated to right child
        posCur = 2 * posCur + child; // Will update posCur to the correct child node
    }

    data[posCur] = temp;
    entryTable.setPointer(temp.id, &data[posCur]);
    return 0;
}


int heap::remove(const std::string &id, int *pKey, void *ppData){
    bool inHeap;
    node *pn = static_cast<node *> (entryTable.getPointer(id, &inHeap));  
    if(!inHeap){return 1;} // Cannot set key of node if it its id is not in entries

    *pKey = pn->key; // Retreive original value of key to return before setting it to perform deletemin
    setKey(id, data[1].key - 1);
    deleteMin(nullptr, nullptr, ppData);
    return 0;
}

void heap::percolateUp(int posCur){
    if(posCur > 1){ // Should not percolate up if at root node
        data[posCur] = data[posCur/2];
        entryTable.setPointer(data[posCur/2].id, &data[posCur]); // Update location of node
    } 
}

// Percolate down could percolate to left child or right child. We return 0 if we percolate to left child, 
// 1 if we percolate to right child, and -1 in case we cannot percolate due to not having children
int heap::percolateDown(int posCur){
    if(posCur > filled/2){return -1;} // We shouldn't return -1 during normal use of this class
    
    if(filled == posCur * 2 || data[posCur * 2].key <= data[posCur * 2 + 1].key){
        data[posCur] = data[posCur * 2]; // If we are at the parent of the last filled note which is a left child we go left by default
        entryTable.setPointer(data[posCur*2].id, &data[posCur]); // Update location of node
        return 0;
    }
    else{
        data[posCur] = data[posCur * 2 + 1];
        entryTable.setPointer(data[posCur*2+1].id, &data[posCur]); // Update location of node
        return 1;
    }
}

int heap::getPos(node *pn){
    int pos = pn - &data[0];
    return pos;
}