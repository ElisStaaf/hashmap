#ifndef HASHMAP_HPP_
#define HASHMAP_HPP_

#include <stdint.h>
#include <cstddef>
#include <cstring>

typedef uint32_t (*hashMapHash)(void *k);
const int defaultBuckets = 100; 

template <typename K, typename V>
struct HashMapNode
{
    void *key;
    void *value;
    
    HashMapNode<K,V>* next; 
    
    HashMapNode()
    {
        value = NULL;
        next = NULL;
    }

    HashMapNode(K k, V v)
    {
        value = v;
        key = k;
        next = NULL;
    }
};


template <typename K, typename V>
class HashMap
{
    public:
        HashMap();
        HashMap(hashMapHash h);         
        V insert(K k, V v);             
        void *get(K k);               
        void remove(K k);           
        void clear();                
        void destroyHashMap();      
    private:
        HashMapNode<K,V>* buckets; 
        int numBuckets;           
        hashMapHash hash;        
};

uint32_t defaultHash(void *k)
{
    char* key = (char*)k;
    int len = strlen(key);
    uint32_t hash = 0;
    unsigned int higherorder = hash & (int)0xf8000000;
    hash = hash << 5;
    hash = hash ^ (higherorder >> 27);

    for(int i = 0; i < len; ++i)
    {
        hash = hash ^ (unsigned int)key[i];
    }
    return hash;
}

template <typename K, typename V>
HashMap<K,V>::HashMap()
{
    hash = defaultHash;

    HashMapNode<K,V>* b = new HashMapNode<K,V>[defaultBuckets];    
    buckets = b;

    numBuckets = defaultBuckets;
}

template <typename K, typename V>
HashMap<K,V>::HashMap(hashMapHash h)
{
    hash = h;

    HashMapNode<K,V>* b = new HashMapNode<K,V>[defaultBuckets]; 
    buckets = b;

    numBuckets = defaultBuckets;
}

template <typename K, typename V>
V HashMap<K,V>::insert(K k, V v)
{
    uint32_t hashValue = hash(k);

    if(hashValue > numBuckets)
    {
        HashMapNode<K,V>* tempBuckets = new HashMapNode<K,V>[ hashValue + 1 ];

        for(int i = 0; i < numBuckets; ++i)
            tempBuckets[i] = buckets[i];

        delete [] buckets;
        buckets = tempBuckets;  
        numBuckets = hashValue + 1;
    }
    
    if(!buckets[hashValue].value || buckets[hashValue].key == k)
    {
        buckets[hashValue].value = v;
        buckets[hashValue].key = k;
    }
    else
    {
        HashMapNode<K,V>* currBucket = &buckets[hashValue];
        while(currBucket->next and currBucket->key != k)
        {
            currBucket = currBucket->next;
            if(currBucket->key == k)
            {
                currBucket->value = v;
                return v;
            }
        }
        currBucket->next = new HashMapNode<K,V>(k,v);
    }
    return v;
}

template <typename K, typename V>
void HashMap<K,V>::remove(K k)
{
    uint32_t hashValue = hash(k); 

    if(buckets[hashValue].key == k)
    {
        buckets[hashValue] = *buckets[hashValue].next;
    }
    else
    {   
        HashMapNode<K, V>* prevBucket = &buckets[hashValue];
        HashMapNode<K, V>* currBucket = buckets[hashValue].next;
        
        while(currBucket)
        {
            if(currBucket->key == k)
            {
                prevBucket->next = currBucket->next;
                delete currBucket;
                break;
            }
            prevBucket = prevBucket->next;
            currBucket = currBucket->next;
        }
    }
        
}

template <typename K, typename V>
void *HashMap<K,V>::get(K k)
{
    uint32_t hashValue = hash(k);
    
    if(numBuckets < hashValue || !buckets[hashValue].key)
        return NULL;

    if(buckets[hashValue].key == k)
        return buckets[hashValue].value;
    else
    {
        HashMapNode<K,V>* currBucket = &buckets[hashValue];
        while(currBucket->next and currBucket->key != k)
            currBucket = currBucket->next;
        if(currBucket->key == k)
            return currBucket->value;
    }
    return NULL;
}

template <typename K, typename V>
void HashMap<K,V>::clear()
{
    for(int i = 0; i < numBuckets; ++i)
    {
        if(buckets[i].next)
        {
            HashMapNode<K,V>* currBucket = buckets[i].next;
            HashMapNode<K,V>* next;
            while(currBucket)
            {
                next = currBucket->next;
                delete currBucket;
                currBucket = next;
            }
        }
        buckets[i] = NULL;
    }
}

template <typename K, typename V>
void HashMap<K,V>::destroyHashMap()
{
    for(int i = 0; i < numBuckets; ++i)
    {
        if(buckets[i].next)
        {
            HashMapNode<K,V>* currBucket = buckets[i].next;
            HashMapNode<K,V>* next;
            while(currBucket)
            {               
                next = currBucket->next;
                delete currBucket;
                currBucket = next;
            }
        }
    } 
    delete []buckets;
}

#endif
