#include <iostream>
using namespace std;

template <class T> class node {
      node* prev;
      node* next;
      T word;
      
      public:
             ~node(){if(this->prev != 0)
                       this->prev->next = this->next;
                     if(this->next != 0)
                       this->next->prev = this->prev;};
            T getWord() {return word;};
            
            node* getNext()  {return next;};
            
            node* getPrev()  {return prev;};
            
            node(T w): prev(0), next(0), word(w) {};
            
            node(node* point, T w)
                       {
                                     this->word = w;
                                     point->next = this;
                                     this->prev = point;
                                     this->next = 0;
                       };
                       
            void clear() {
                 
                 if(this->prev != 0)
                 this->prev->next = this->next;
                 
                 if(this->next != 0)
                 this->next->prev = this->prev;
                 
                 delete this;
                 };
                 
            node(node* left,node* right,T w)   {
                if (!( (left->next != right) || (right->prev != left) ))
                {   
                    left->next = this;
                    right->prev = this;
                    this-> prev = left;
                    this-> next = right;
                    word = w; 
                }               
                    }
      };
      
      int main() {
          node<char*>* a = new node<char*>("Bird");
          node<char*>* b = new node<char*>(a,"is");
          node<char*>* c = new node<char*>(b,"the");
          node<char*>* d = new node<char*>(c,"word");
          
          node<char*>* e = new node<char*>(d,"testing!");
          
          cout << a->getWord() << " " << b->getWord() << " " << c->getWord() << " " << d->getWord() << "!" << endl;
          cout << a->getWord() << " " << a->getNext()->getWord() << " " << a->getNext()->getNext()->getWord() << " " << a->getNext()->getNext()->getNext()->getWord() << "!" << endl;
          cout << a->getNext()->getPrev()->getWord() << endl;
          
          node<char*>* cd = new node<char*>(c,d,"ultimate");
          
          node<char*>* iter = a;
          while(iter)
                   {
                   cout << iter->getWord() << " ";
                   iter = iter->getNext();
                   };
                   
          cout << endl << "Editing. Removing 'the'" << endl;
          delete c;
          
          iter = a;
          while(iter)
                   {
                   cout << iter->getWord() << " ";
                   iter = iter->getNext();
                   };
          
          
          cout<<endl<<endl<<endl;
          system("pause");
          return 0;
          }
