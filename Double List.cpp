#include <iostream>
#include <string>
using namespace std;

struct NodeErr {string msg; NodeErr(string w){msg=w;};};
class node {
      node* init;
      node* prev;
      node* next;
      string word;
      
      public:
            string getWord() {return word;};
            
            node* getNext()  {if (this->next!=0) return next; else throw NodeErr("Not a next node");};
            
            node* getPrev()  {if (this->prev!=0) return prev; else throw NodeErr("Not a previous node.");};
            
            /*node* getFirst(){    node* point = init; 
                                 while((point->next!=0)) point = point->next;
                                 return point;
                                 };
                                 
            node* getLast() {    node * point = init;
                                 while(point->prev!=0) point = point->prev;
                                 return point;
                                 };
            */                     
            node(string w = "Node"): prev(0), next(0), word(w) {init = this;};
            
            node(node* point, string w = "Bird", bool toright = true)
                       {
                              if (toright == true)
                                     {
                                     this->word = w;
                                     point->next = this;
                                     this->prev = point;
                                     this->next = 0;
                                     }
                              else
                                     {
                                     this->word = w;
                                     point->prev = this;
                                     this->next = point;
                                     this->prev = 0;
                                       
                                     };
                       };
                       
            void clear() {
                 
                 if(this->prev != 0)
                 this->prev->next = this->next;
                 
                 if(this->next != 0)
                 this->next->prev = this->prev;
                 
                 delete this;
                 };
            node(node* left,node* right,string w = "Sfinos")
            {
            if ( (left->next != right) || (right->prev != left) ) throw NodeErr("Bad_Constructor");   
            left->next = this;
            right->prev = this;
            this-> prev = left;
            this-> next = right;
            word = w;                
            }
      };
      
      int main() {
          node* a = new node("Bird");
          node* b = new node(a,"is");
          node* c = new node(b,"the");
          node* d = new node(c,"word");
          cout << a->getWord() << " " << b->getWord() << " " << c->getWord() << " " << d->getWord() << "!" << endl;
          cout << a->getWord() << " " << a->getNext()->getWord() << " " << a->getNext()->getNext()->getWord() << " " << a->getNext()->getNext()->getNext()->getWord() << "!" << endl;
          cout << a->getNext()->getPrev()->getWord() << endl;
          
          node* e = new node(d,"testing!");
          
          try{
          d->getNext();
          } catch(NodeErr& e) {cout<<e.msg<<endl<<endl;};
          
          for(node* iter = a;1;)
                   {
                   cout << iter->getWord() << " ";
                   try{iter = iter->getNext();}
                   catch (...) {cout<<endl<<endl; break; }
                   };
          
          cout<<"Modifying"<<endl<<endl;
          node* cd = new node(c,d,"ultimate");
          
                   
          for(node* iter = a;1;)
                   {
                   cout << iter->getWord() << " ";
                   try{iter = iter->getNext();}
                   catch (...) {cout<<endl<<endl; break; }
                   };
          
          system("pause");
          return 0;
          }
