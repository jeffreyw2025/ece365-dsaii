# include <hash.h>
# include <vector>

# include <iostream>

/* This list of primes is a truncated version of the list from 
https://codeyarns.com/tech/2015-10-16-prime-number-of-buckets-in-hash-table-implementations.html#gsc.tab=0
using only the values that can fit within a signed int rather than an unsigned long */
static const unsigned int primesList[28] = {
    5u, 11u, 23u, 47u, 97u, 199u, 409u, 823u, 1741u, 3469u, 6949u, 14033u, 
    28411u, 57557u, 116731u, 236897u, 480881u, 976369u, 1982627u, 4026031u, 
    8175383u, 16601593u, 33712729u, 68460391u, 139022417u, 282312799u, 
    573292817u, 1164186217u
};

hashTable::hashTable(int size){
    capacity = getPrime(size);
    filled = 0; // The hashTable starts with no entries
    data.resize(capacity);
}

int hashTable::insert(const std::string &key, void *pv){
    // If hashtable more than half full, rehash
    if(filled >= (int)(capacity/2)){
        if(!rehash()){return 2;} // If rehash fails return error
    }
    int pos = hash(key);
    // pos will iterate until a matching key is found in an occupied, non-deleted cell (already exists) or an unoccupied cell is found (can insert) 
    while(data[pos].isOccupied){
        if(!(data[pos].isDeleted) && (data[pos].key == key)){return 1;} // If key match found then key is already in table, exit.
        pos = (pos + 1) % capacity;
    }
    // If an element can be inserted, set the associated key and void pointer, and flag the cell as occupied
    data[pos].isOccupied = true;
    data[pos].key = key;
    data[pos].pv = pv;
    filled++;
    return 0;
}

bool hashTable::contains(const std::string &key){
    int pos = findPos(key);
    if(pos < 0){return false;} // If key not in hashtable pos will be -1
    return true;
};

void *hashTable::getPointer(const std::string &key, bool *b){
    int pos = findPos(key);
    if(pos < 0){
        if(b != nullptr){*b = false;}
        return nullptr;
    }
    if(b != nullptr){*b = true;}
    return data[pos].pv;
};

int hashTable::setPointer(const std::string &key, void *pv){
    int pos = findPos(key);
    if(pos < 0){return -1;} // If key not in hashtable pos will be -1
    data[pos].pv = pv; 
    return 0;
};

bool hashTable::remove(const std::string &key){
    int pos = findPos(key);
    if(pos < 0){return false;} // If key not in hashtable pos will be -1
    data[pos].isDeleted = true; 
    return true;
};

int hashTable::hash(const std::string &key){
    unsigned int hashVal = 0;
    for(int i = 0; i < key.length(); i++){
        hashVal = 37 * hashVal + key[i];
    }
    return (int)(hashVal % capacity); 
};

int hashTable::findPos(const std::string &key){
    int pos = hash(key);
    // pos will iterate until a matching key is found in an occupied, non-deleted cell (exists) or an unoccupied cell is found (does not exist) 
    while(data[pos].isOccupied){
        if(!(data[pos].isDeleted) && (data[pos].key == key)){return pos;}
        pos = (pos + 1) % capacity;
    }
    return -1; // An empty entry means the key is not in the hashtable
};

bool hashTable::rehash(){
    // Since the capacity is always prime, getPrime(capacity+1) should return the next biggest prime number above capacity
    int newSize = getPrime(capacity + 1);
    if(newSize == 0){return false;} // No larger prime exists than current capcity, cannot rehash
    // Hacky workaround to expand data while keeping the original vector for reinsertion
    std::vector<hashItem> newData(newSize);
    std::vector<hashItem> oldData(capacity);
    oldData = data;
    data = newData;
    capacity = newSize; // The hashtable will have the new size but be empty before reinsertion
    filled = 0;
    // Re-insertion of old data
    for(int i = 0; i < oldData.size(); i++){
        if(oldData[i].isOccupied && !(oldData[i].isDeleted)){ // Lazily deleted items are removed
            insert(oldData[i].key, oldData[i].pv);
        }
    }
    return true;
}

unsigned int hashTable::getPrime(int size){
    for(int i = 0; i < sizeof(primesList); i++){
        if(primesList[i] >= size){return primesList[i];}
    }
    return 0; // 0 is used in case of an error
}