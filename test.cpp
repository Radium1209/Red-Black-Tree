#include "rbTree.h"
#include "mySet.h"
#include "myMap.h"
#include <iostream>
#include <string>
using namespace std;

int main( int argc, char* argv[] )
{
    mySet<int> s;
    s.insert(1);
    cout << (s.find(1))->key << endl;
    s.insert(10);
    cout << (s.end())->key << endl;
    s.insert(23);
    cout << (s.begin())->key << endl;
    s.erase(23);
    cout << (s.end())->key << endl;

    myMap<int, int> m;
    m.insert(1, 100);
    m.insert(2, 32);
    m.insert(3, 11);
    m.insert(11, 44);
    cout << m.end()->value << endl;
    m.erase(2);
    m.erase(100);

    RBTree<int, int> tree;
    string option;
    for (int i=1; i<=10000; i++)
    {
        tree.insert( i );
    }
    for (int i=1; i<=10000; i++)
    {
        tree.erase( i );
    }
    while( true )
    {
        cout << "> ";
        cin >> option;
        if ( option == "insert" )
        {
            int k, val;
            cin >> k >> val;
            tree.insert( k, val );
            cout << "finish insert" << endl;
            tree.isRBT();
        }
        else if ( option == "erase" )
        {
            int val;
            cin >> val;
            tree.erase( val );
            cout << "finish erase" << endl;
            tree.isRBT();
        }
        else if ( option == "draw" )
        {
            tree.draw();
        }
        else if ( option == "exit" )
        {
            break;
        }
        
    }
    return 0;
}