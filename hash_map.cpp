
#include <iosteam>
#include <string>
#include "hash_map.h"

using namespace std;


//Constructor
template <typename Key, typename Value>
hash_map<Key,Value>::hash_map(){
	for(int i = 0; i < capacity; i++)
		HashTable[i] = new entry;

	header = new entry;
	trailer = new entry;
	header->next = trailer;
	trailer->prev = header;	
	hash_size = 0;
}


//Destructor
template <typename Key, typename Value>
hash_map<Key,Value>::~hash_map(){
	while(!empty()) rmEntry(); //TODO: remove entry fucntion
        for(int i = 0; i < capacity; i++)
                delete HashTable[i];

	delete header;
	delete trailer;
        hash_size = 0;
}


//Hash fuction: return hash index
template <typename Key, typename Value>
size_t hash_map<Key,Value>::hash (const Key& key){
	//TODO: hashing function
}


//Hash map size function
template <typename Key, typename Value>
size_t hash_map<Key,Value>::size(){	return hash_size;	}


/*Returns an iterator for the container hash_map that
	points to the first data entry in hash_map*/
template <typename Key, typename Value>
typename hash_map<Key,Valu>::iterator 
hash_map<Key,Value>::begin(){	return iterator(header->next)	}


/*Returns an iterator for the container hash_map that
	points to the last data entry in hash_map*/
template <typename Key, typename Value>
typename hash_map<Key,Valu>::iterator 
iterator hash_map::end(){	return iterator(trailer);	}


//Hash insert
template <typename Key, typename Value>
typename hash_map<Key,Valu>::iterator 
iterator insert (const Key& key, const Value& value){
	size_t index = hash(key);		//Get hash index
	entry* bucket = new entry;		//Create new bucket
	bucket->_key = key;
	bucket->_value = value;
	
	if(Header->next == Trailer){		//Empty hash map
		HashTable[index]->next = bucket;//Points to first bucket in the index chain
		HashTable[index]->prev = bucket;//Points to last bucket in the index chain
		Header->next = bucket;		//Header points to the first bucket in the hash map
		bucket->prev = Header;		//First bucket's 'prev' points to the header
		bucket->next = Trailer;		//First bucket's 'next' points to the trailer
		Trailer->prev = bucket;		//Trailer points to the last bucket

		return iterator(bucket);
	}
	
	//In case of header not points to trailer, hash map is not empty
	if(HashTable[index]->next == NULL){		//Empty index chain
		HashTable[index]->next = bucket;	
                HashTable[index]->prev = bucket;	
		if(index < hash[Header->next->_key]){	//In case of inserting bucket before the header
			bucket->next = Header->next;
			bucket->prev = Header;
			Header->next->prev = bucket;
			Header->next = bucket;
		}
		else if(index > hash[Header->next->_key] && index < hash[Trailer->prev->_key]){
			//In case of inserting bucket between header and trailer
			size_t prev_index = index;

			while(HashTable[--prev_index]->next != NULL){}
		
			bucket->next = HashTable[prev_index]->prev->next;
			bucket->prev = HashTable[prev_index]->prev;
			HashTable[prev_index]->prev->next = bucket;
			bucket->next->prev = bucket;
		}
		else{ //Insert bucket after the trailer
			bucket->next = Trailer;
                        bucket->prev = Trailer->prev;
                        Trailer->prev = bucket;
                        bucket->prev->next = bucket;	

		}

		return iterator(bucket);
	}
	
	//Insert bucket to the index chain, which is not empty
	bucket->next = HashTable[index]->next;		//Insert bucket in the first position in the index chain
	bucket->prev = HashTable[index]->next->prev;
	HashTable[index]->next = bucket;
	
	return iterator(bucket);

}


//Hash find
template <typename Key, typename Value>
typename hash_map<Key,Valu>::iterator 
iterator find (const Key& key){
	size_t index = hash(key);
	iterator iter(HashTable[index]);
	
	for( ; iter->_bucket->next != NULL; ++iter){
		if(iter->_bucket->_key == key)
			return iter;
	}

	return iter(NULL);
	
	
}



void erase (Iterator pos);

             
Value operator[] (const Key& key);






