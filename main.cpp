#include <bits/stdc++.h>
using namespace std;
#define DELETION_MODE 0

class Bucket{
    private:
        int depth,size;
        vector<int> values;
    public:
        Bucket(int depth, int size);
        int insert(int key);
        int remove(int key);
        void search(int key);
        int isFull();
        int isEmpty();
        int getDepth();
        int changeDepth(int quantity);
        vector<int> copy();
        void clear();
        void display();
        virtual ~Bucket();
};

class Directory{
    private:
        int global_depth, bucket_size;
        vector<Bucket*> buckets;
        int hash(int n);
        int pairIndex(int bucket_no, int depth);
        void grow();
        void shrink();
        void split(int bucket_no);
        void merge(int bucket_no);
        string bucket_id(int n);
    public:
        Directory(int depth, int bucket_size);
        void insert(int key,bool reinserted);
        void remove(int key,int mode);
        void search(int key);
        void display(bool duplicates, int mode);
        virtual ~Directory();
};

// Main function

int main(){
    int max_bucket_size, global_depth;
    int key;
    int choice;
    bool show_duplicate_buckets = 0;
    cin>>global_depth;
    cin>>max_bucket_size;
    Directory d(global_depth,max_bucket_size);
    cin>>choice;
    while(choice != 6){
        switch(choice){
            case 2:{
                cin>>key;
                d.insert(key,0);
                break;
            }
            case 3:{
                cin>>key;
                d.search(key);
                break;
            }
            case 4:{
                cin>>key;
                d.remove(key,DELETION_MODE);
                break;
            }
            case 5:{
                d.display(show_duplicate_buckets,DELETION_MODE);
                break;
            }
        }
        cin>>choice;
    }
    return 0;
}

// Directory class functions

Directory::Directory(int depth, int bucket_size)
{
    this->global_depth = depth;
    this->bucket_size = bucket_size;
    for(int i = 0 ; i < 1<<depth ; i++) buckets.push_back(new Bucket(depth,bucket_size));
}

int Directory::hash(int n){
    return n&((1<<global_depth)-1);
}

int Directory::pairIndex(int bucket_no, int depth){
    return bucket_no^(1<<(depth-1));
}

void Directory::grow(void){
    for(int i = 0 ; i < 1<<global_depth ; i++) buckets.push_back(buckets[i]);
    global_depth++;
}

void Directory::shrink(void){
    for(int i = 0 ; i<buckets.size() ; i++){
        if(buckets[i]->getDepth()==global_depth) return;
    }
    global_depth--;
    for(int i = 0 ; i < 1<<global_depth ; i++) buckets.pop_back();
}

void Directory::split(int bucket_no){
    int local_depth,pair_index,index_diff,dir_size;
    vector<int> temp;
    local_depth = buckets[bucket_no]->changeDepth(1);
    if(local_depth>global_depth) grow();
    pair_index = pairIndex(bucket_no,local_depth);
    buckets[pair_index] = new Bucket(local_depth,bucket_size);
    temp = buckets[bucket_no]->copy();
    buckets[bucket_no]->clear();
    index_diff = 1<<local_depth;
    dir_size = 1<<global_depth;
    for(int i=pair_index-index_diff ; i>=0 ; i-=index_diff ) buckets[i] = buckets[pair_index];
    for(int i=pair_index+index_diff ; i<dir_size ; i+=index_diff ) buckets[i] = buckets[pair_index];
    for(auto it=temp.begin();it!=temp.end();it++) insert((*it),1);
}

void Directory::merge(int bucket_no){
    int local_depth,pair_index,index_diff,dir_size;
    local_depth = buckets[bucket_no]->getDepth();
    pair_index = pairIndex(bucket_no,local_depth);
    index_diff = 1<<local_depth;
    dir_size = 1<<global_depth;
    if( buckets[pair_index]->getDepth() == local_depth ){
        buckets[pair_index]->changeDepth(-1);
        delete(buckets[bucket_no]);
        buckets[bucket_no] = buckets[pair_index];
        for(int i=bucket_no-index_diff ; i>=0 ; i-=index_diff ) buckets[i] = buckets[pair_index];
        for(int i=bucket_no+index_diff ; i<dir_size ; i+=index_diff ) buckets[i] = buckets[pair_index];
    }
}

string Directory::bucket_id(int n){
    int d;
    string s;
    d = buckets[n]->getDepth();
    s = "";
    while(n>0 && d>0){
        s = (n%2==0?"0":"1")+s;
        n/=2;
        d--;
    }
    while(d>0){
        s = "0"+s;
        d--;
    }
    return s;
}

void Directory::insert(int key,bool reinserted){
    int bucket_no = hash(key);
    int status = buckets[bucket_no]->insert(key);
    if(status==0){
        split(bucket_no);
        insert(key,reinserted);
    }
    // else if(status==1){
    //     if(!reinserted) cout<<"Key Inserted!"<<endl;
    //     else cout<<"Key Moved To Other Bucket!"<<endl;
    // }
    // else cout<<"Key Already Exists!"<<endl;
}

void Directory::remove(int key,int mode){
    int bucket_no = hash(key);
    buckets[bucket_no]->remove(key);
    if(mode>0 && buckets[bucket_no]->isEmpty() && buckets[bucket_no]->getDepth()>1) merge(bucket_no);
    if(mode>1) shrink();
}

void Directory::search(int key){
    int bucket_no = hash(key);
    buckets[bucket_no]->search(key);
}

void Directory::display(bool duplicates, int mode){
    int i,j,d;
    int size=0;
    string s;
    set<string> shown;
    cout<<global_depth<<"\n";
    for(i=0;i<buckets.size();i++){
        s = bucket_id(i);
        if(duplicates || shown.find(s)==shown.end()){
            shown.insert(s);
            if(buckets[i]->isEmpty() == 0 || mode == 0) size++;
        }
    }
    cout<<size<<"\n";
    shown.clear();
    for(i=0;i<buckets.size();i++){
        s = bucket_id(i);
        if(duplicates || shown.find(s)==shown.end()){
            shown.insert(s);
            if(buckets[i]->isEmpty() == 0 || mode == 0) buckets[i]->display();
        }
    }
}

Directory::~Directory(){
    for(int i=0; i<buckets.size(); i++){
        buckets[i]->clear();
        delete(buckets[i]);
    }
}

// Bucket class functions

Bucket::Bucket(int depth, int size){
    this->depth = depth;
    this->size = size;
}

int Bucket::insert(int key){
    auto it = find(values.begin(),values.end(),key);
    if(it!=values.end()) return -1;
    if(isFull()) return 0;
    values.push_back(key);
    return 1;
}

int Bucket::remove(int key){
    auto it = find(values.begin(),values.end(),key);
    if(it!=values.end()){
        values.erase(it);
        return 1;
    }
    else{
        // cout<<"Error : Key Not Found!"<<endl;
        return 0;
    }
}

void Bucket::search(int key){
    auto it = find(values.begin(),values.end(),key);
    // if(it!=values.end()) cout<<"Found!"<<endl;
    // else cout<<"Not Found!"<<endl;
}

int Bucket::isFull(void){
    if(values.size()==size) return 1;
    else return 0;
}

int Bucket::isEmpty(void){
    if(values.size()==0) return 1;
    else return 0;
}

int Bucket::getDepth(void){
    return depth;
}

int Bucket::changeDepth(int quantity){
    depth += quantity;
    return depth;
}

vector<int> Bucket::copy(void){
    vector<int> temp(values.begin(),values.end());
    return temp;
}

void Bucket::clear(void){
    values.clear();
}

void Bucket::display(){
    cout<<values.size()<<" "<<depth<<"\n";
}

Bucket::~Bucket(){
    return;
}