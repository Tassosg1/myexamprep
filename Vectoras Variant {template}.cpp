#include <iostream>
using namespace std;

template <class T> class vectoras {
      private:
              T* elem;
              int size;
              int space;
      public:
      vectoras(): elem(0), size(0), space(0) {};
      
      vectoras(int sz) : size(sz), space(sz), elem(new T[sz]) {};
      
   /*   vectoras operator=(vectoras& old)
               {elem = new T[old.size];
               for (int i = 0; i < old.size;i++) elem[i] = old.elem[i];
               space = size = old.size;
               };
               
      vectoras(const vectoras& old)
               {elem = new T[old.size];
               for (int i = 0; i < old.size;i++) elem[i] = old.elem[i];
               space = size = old.size;
               };  */
          
     
      ~vectoras() {delete [] elem;};
      
      T operator[](int loc) {return elem[loc];};
      
      int getSize() {return size;};
      
      int getCapacity() {return space;};
      
      int resize(int newsize) {
          if (space > newsize) return 1;
          int* p = new T[newsize];
          for (int i =0; i < newsize;i++)
              if (i < size) p[i] = elem[i];
                 else p[i] = 0;
          delete [] elem;
          elem = p;
          space = newsize;
          return 0;
          }
      
      int reserve(int tofill) {
          int* p = new T[tofill];
          for (int i =0; i < tofill;i++) p[i] = 0;
          delete [] elem;
          elem = p;
          space = tofill;
          return 0;
          }
      
      void pushback(T toPush) {
          if (space==0) reserve(8);
          else if (space==size) resize(size*2);
          elem[size] = toPush;
          size++;
          };
      };
      
      int main() {
              
          vectoras<int> a;
          a.pushback(35);
          a.pushback(40);
          a.pushback(45);
          a.pushback(50);
          a.pushback(55);
          a.pushback(60);
          a.pushback(70);
          a.pushback(75);
          a.pushback(45);
          a.pushback(3);
          
          for (int i = 0; i < 15; i++)
              cout << a[i] << endl;
          cout << "A has " << a.getSize() << " size and " << a.getCapacity() << " capacity." << endl;
              
          cout << "-----------NOW TESTING B----------" << endl;
          
          vectoras<int> b(a);
          vectoras<int> c = a;
          
          cout << "A has " << a.getSize() << " size and " << a.getCapacity() << " capacity." << endl;
          b.pushback(-2);b.pushback(4);
          cout << "B has " << b.getSize() << " size and " << b.getCapacity() << " capacity." << endl;
          cout << "C has " << c.getSize() << " size and " << c.getCapacity() << " capacity." << endl;
          
    //      for (int i = 0; i < 15; i++)
    //          cout << b[i] << endl;
              
          
          
          int fin;
          cin >> fin;
          
          return 0;
             
      }
