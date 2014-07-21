
#include <iosteam>
#include <string>
#include "hash_map.h"

using namespace std;


//Constructor
template <typename Key, typename Value>
hash_map<Key,Value>::hash_map(){
	try{
		for(int i = 0; i < capacity; i++)
			HashTable[i] = new entry;

		header = new entry;
		trailer = new entry;
		header->next = trailer;
		trailer->prev = header;	
		hash_size = 0;

	}catch(bad_alloc& ba){
		cerr << "exception caught:" << ba.what() << endl;
	}
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
typename hash_map<Key,Value>::iterator 
hash_map<Key,Value>::begin(){	return iterator(header->next)	}


/*Returns an iterator for the container hash_map that
	points to the last data entry in hash_map*/
template <typename Key, typename Value>
typename hash_map<Key,Value>::iterator 
hash_map<Key,Value>::end(){	return iterator(trailer);	}


//Hash insert
template <typename Key, typename Value>
typename hash_map<Key,Value>::iterator 
hash_map<Key,Value>::insert (const Key& key, const Value& value){
	size_t index = hash(key);		//Get hash index
	try{
		entry* bucket = new entry;		//Create new bucket
		bucket->_key = key;
		bucket->_value = value;
	}catch(bad_alloc& ba){
		cerr << "exception caught:" << ba.what() << endl;
		return iterator(NULL);
	}

	hash_size++;
	
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
typename hash_map<Key,Value>::iterator 
hash_map<Key,Value>::find (const Key& key){
	size_t index = hash(key);
	iterator iter(HashTable[index]);
	
	for( ; hash[iter->_bucket->_key] < index+1; ++iter){
		if(iter->_bucket->_key == key)
			return iter;
	}
	
	cout << "Item with specified key is not found in the hash map!" << endl;
	return iter(NULL);
	
}


template <typename Key, typename Value>
hash_map<Key,Value>::erase (Iterator pos){
	if(pos->bucket != NULL){
		size_t index = hash[pos->_bucket->_key];
	
		if(HashTable[index]->next == pos->_bucket && 
			HashTable[index]->prev == pos->_bucket){
			HashTable[index]->next = NULL;
			HashTable[index]->prev = NULL;
			pos->_bucket->prev->next = pos->_bucket->next;
                        pos->_bucket->next->prev = pos->_bucket->prev;
			
			delete pos->_bucket;
		}
		else if(HashTable[index]->next == pos->_bucket){
			HashTable[index]->next = pos->_bucket->next;
			pos->_bucket->prev->next = pos->_bucket->next;
			pos->_bucket->next->prev = pos->_bucket->prev;
			
			delete pos->_bucket;
		}
		else if(HashTable[index]->prev == pos->_bucket){
			HashTable[index]->prev = pos->_bucket->prev;
			pos->_bucket->prev->next = pos->_bucket->next;
                	pos->_bucket->next->prev = pos->_bucket->prev;
			
			delete pos->_bucket;
		}
		else{
			pos->_bucket->prev->next = pos->_bucket->next;
                        pos->_bucket->next->prev = pos->_bucket->prev;

                        delete pos->_bucket;
		}	
	}
	
	hash_size--;
}

            
Value operator[] (const Key& key);






