#include<bits/stdc++.h>
using namespace std;

class Bucket{
    private:
        int *arr;
        int depth;
        int capacity;
        int filled;

        bool checkDuplicate(int n) {
            for(int i=0;i<filled;i++)
                if(arr[i]==n)
                    return true;
            return false;
        }

    public:
        Bucket(int capacity) {
            this->capacity = capacity;
            arr = new int[capacity];
            filled = 0;
            depth = 1;
        }

        bool isFilled() {
            return (filled == capacity);
        }

        bool insert(int n){
            if (!isFilled() && !checkDuplicate(n)) {
                arr[filled] = n;
                filled++;
                return true;
            } else {
                return false;
            }
        }

        bool deleteElement(int n) {
            for (int i = 0; i < filled; i++) {
                if (arr[i] == n) {
                    arr[i] = arr[filled-1];
                    filled--;
                    return true;
                }
            }
            return false;
        }

        void setDepth(int n){
            this->depth = n;
        }

        int getDepth(){
            return depth;
        }

        int getCapacity(){
            return capacity;
        }

        int getFilled(){
            return filled;
        }

        int getElement(int index){
            return arr[index];
        }
};

class ExtendedHashTable {
    private:
        Bucket **buckets;
        int size;
        int filled;
        int capacity;
        int depth;

        bool checkDuplicate(int n) {
            for(int i=0;i<filled;i++)
                if(buckets[i]->getFilled()>0)
                    for(int j=0;j<buckets[i]->getFilled();j++)
                        if(buckets[i]->getElement(j)==n)
                            return true;
            return false;
        }

        void rehash(){
            Bucket **newBuckets = new Bucket*[size*2];
            for(int i=0;i<size*2;i++)
                newBuckets[i] = new Bucket(capacity);
            for(int i=0;i<size;i++)
                for(int j=0;j<buckets[i]->getFilled();j++)
                    newBuckets[i*2]->insert(buckets[i]->getElement(j));
            for(int i=0;i<size;i++)
                delete buckets[i];
            delete [] buckets;
            buckets = newBuckets;
            size = size*2;
        }

        int countBuckets() {
            // count non empty buckets
            int count = 0; //
            for(int i=0; i<size; i++)
                if(buckets[i]->getFilled()>0)
                    count++;
            
            return count;
        }

    public:
        ExtendedHashTable(int capacity) {
            this->capacity = capacity;
            size = capacity;
            filled = 0;
            depth = 1;
            buckets = new Bucket*[capacity];
            for(int i=0;i<capacity;i++)
                buckets[i] = new Bucket(capacity);
        }

        bool insert(int n){
            if (!checkDuplicate(n)) {
                if (filled == size)
                    rehash();
                int index = n % size;
                if(buckets[index]->insert(n))
                    filled++;
                return true;
            } else {
                return false;
            }
        }

        void setDepth(int n){
            this->depth = n;
        }

        int getDepth(){
            return depth;
        }

        int getCapacity(){
            return capacity;
        }

        void print(){
            for(int i=0;i<size;i++){
                cout<<"Bucket "<<i<<" : ";
                for(int j=0;j<buckets[i]->getFilled();j++)
                    cout<<buckets[i]->getElement(j)<<" ";
                cout<<endl;
            }
        }

        void deleteElement(int n){
            for(int i=0;i<size;i++){
                for(int j=0;j<buckets[i]->getFilled();j++)
                    if(buckets[i]->getElement(j)==n){
                        buckets[i]->deleteElement(n);
                        return;
                    }
            }
        } 

        void print_bucketsinfo() {
            cout << depth << endl;
            cout << filled << endl;

            for(int i=0;i<size;i++){
                cout<<"Bucket "<<i<<" : ";
                cout<<buckets[i]->getFilled()<<" ";
                cout<<buckets[i]->getDepth()<<" ";
                cout<<endl;
            }
        }
};

int main() {
    int n,k;

    cin >> n >> k;

    ExtendedHashTable *hashTable = new ExtendedHashTable(k);

    int x;
    while(cin >> x) {
        switch(x) {
            case 2:
                cin >> x;
                hashTable->insert(x);
                break;
            case 3:
                cin >> x;
                break;
            case 4:
                cin >> x;
                hashTable->deleteElement(x);
                break;
            case 5: 
                hashTable->print_bucketsinfo();
                break;
            case 6:
                return 0;
        }
    }

    hashTable->print();    

    return 0;
}
