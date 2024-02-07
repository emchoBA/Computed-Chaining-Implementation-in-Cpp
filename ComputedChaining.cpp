#include "ComputedChaining.h"
using namespace std;

ComputedChaining::ComputedChaining(int table_size){
        for (int i = 0; i < table_size; ++i) {
            data_vec.push_back(entry());
        }
}
 int ComputedChaining::hash(int key) {
        return key % data_vec.size();
}
int ComputedChaining::hash2(int key){
        int step=(key/data_vec.size())%data_vec.size();
        if(step==0)step=1;
        return step;
}
void ComputedChaining::insert(int key){
        int index=hash(key);
        
        if(data_vec[index].data != -1){//collision
                if(isThisYourHome(data_vec[index].data)){//Q1: Add to chain

                    int incr = hash2(data_vec[index].data);
                    int dest = hash(data_vec[index].data);
                    int last_chain = index;
                    
                    while(data_vec[dest].link != -1){
                        int link_dest = dest;
                        for(int i = 0; i < data_vec[dest].link; i++){
                            link_dest = hash(link_dest + incr);
                        }
                        dest = link_dest;
                        last_chain = dest;
                        incr = hash2(data_vec[dest].data);
                        
                    }
                    
                    int link_count;
                    for(link_count = 0; data_vec[dest].data != -1; link_count++){
                        dest = hash(dest + incr);
                    }
                    
                    data_vec[dest].data = key;
                    data_vec[dest].link = -1;
                    data_vec[last_chain].link = link_count;
                    
                }else{
                        int moved_key = data_vec[index].data;
                        int chain = first_of_my_chain(moved_key);
                        vector<int> moved_vector = find_chain(chain);
                        int moved_key_index = 0;
                        for(int i = 0; moved_key != data_vec[moved_vector.at(i)].data; i++){
                            moved_key_index++;
                        }
                        
                        int moved_previous = (moved_key_index == 0) ? (moved_vector.size() - 1) : (moved_vector.at(moved_key_index - 1));
                        data_vec[moved_previous].link = -1;
                        
                        vector<int> stored_keys;
                        
                        for(int i = moved_key_index; i < moved_vector.size(); i++){
                            stored_keys.push_back(data_vec[moved_vector.at(i)].data);
                            data_vec[moved_vector.at(i)].data = -1;
                            data_vec[moved_vector.at(i)].link = -1;
                        }
                        
                        data_vec[index].data = key;
                        
                        for(int i = 0; i < stored_keys.size(); i++){
                            insert(stored_keys.at(i));
                        }
                        
                }
        }else{//direct insertion
              data_vec[index].data = key;
              data_vec[index].link= -1; 
        }
}
int ComputedChaining::find_index(int key){//find the index of 
        int idx=hash(key);
        int inc=hash2(key);
                if(data_vec[idx].data==key)
                        return idx;
                else{
                        while(data_vec[idx].data != -1){
                                if(data_vec[idx].data != key){
                                        inc=hash2(data_vec[idx].data);
                                        idx=(idx+data_vec[idx].link*inc)%data_vec.size();
                                }else return idx;
                
                }
                }
        return -1;
}
bool ComputedChaining::isThisYourHome(int key){//If returns true if the hash of the key is same with the index of data_vec 

        int table_index = find_index(key);
        
        if (hash(key) == table_index) {
            return true;
        }
        else {
            return false;
        }
        
}
int ComputedChaining::first_of_my_chain(int key) {
        return data_vec[hash(key)].data;
}

int ComputedChaining::find_num_probes(int key){//

    int index = hash(key);
    int step = 1;
    if(data_vec[index].data == key){
        return step;
    }
    else{
        int dest = hash(data_vec[index].data);
        int incr = hash2(data_vec[index].data);
        
        
        while(data_vec[dest].data != key){
            int link = data_vec[dest].link;
            int linked_dest = dest;
            for(int i = 0; i < link; i++){
                linked_dest = hash(linked_dest + incr);
            }
            dest = linked_dest;
            incr = hash2(data_vec[linked_dest].data);
            step++;
        }
        
        return step;
    }


}
vector<int> ComputedChaining::find_chain(int key){ //gives you chain whose member is the key
        vector<int> chain;
        int index=hash(key);
        int inc;//1
        while(data_vec[index].data != -1){
                chain.push_back(index);
                if(data_vec[index].link != -1){
                        inc=hash2(data_vec[index].data);//2
                        index=(index+data_vec[index].link*inc)%data_vec.size();

        }else
                        break;
        }
        return chain;
}

