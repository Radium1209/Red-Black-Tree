#ifndef RED_BLACK_TREE
#define RED_BALCK_TREE

#include <stdlib.h>

/* 红黑树颜色 */
enum RBTColor
{
    RED,
    BLACK,
};

/* 红黑树节点 */
template<class Key, class Value>
class RBTNode
{
public:
    RBTNode( RBTColor col, Key k, Value val, RBTNode* lch, RBTNode* rch, RBTNode* fa )
    : color( col ), key( k ), value( val ), blackNum( 0 ), leftChild( lch ), rightChild( rch ), father( fa ) {}

    RBTColor color;         // 颜色(红或黑)
    Key key;                // 关键字
    Value value;            // 数据
    int blackNum;           // 该节点的路径包含的黑色节点个数
    RBTNode *leftChild;     // 左孩子
    RBTNode *rightChild;    // 右孩子
    RBTNode *father;        // 父节点
};

/* 节点-层数对，用于遍历 */
template<class Key, class Value>
class NLPair
{
public:
    NLPair( RBTNode<Key, Value>* node, int layer )
    : node( node ), layer( layer ) {}

    RBTNode<Key, Value>* node;
    unsigned int layer;
};

template<class Key, class Value>
class RBTree
{
    
private:
    RBTNode<Key, Value> *root;

public:
    RBTree();
    ~RBTree();
    bool isRBT();
    RBTNode<Key, Value>* insert( Key key );
    RBTNode<Key, Value>* insert( Key key, Value value );
    RBTNode<Key, Value>* insertUnique( Key key );
    RBTNode<Key, Value>* insertUnique( Key key, Value value );
    void erase( Key key );
    void clear();
    RBTNode<Key, Value>* find( Key key );
    void draw();
    RBTNode<Key, Value>* begin();
    RBTNode<Key, Value>* end();
    unsigned int size();
    bool empty();

private:
    bool isBST( RBTNode<Key, Value> *node );
    bool hasTwoRed( RBTNode<Key, Value> *node );
    bool hasSameBlack( RBTNode<Key, Value> *node );
    void calBlackNum( RBTNode<Key, Value> *node, int blackNum );
    void leftRotate( RBTNode<Key, Value> *root );
    void rightRotate( RBTNode<Key, Value> *root );
    void insert( RBTNode<Key, Value> *node );
    void insertFix( RBTNode<Key, Value> *node );
    void erase( RBTNode<Key, Value> *node );
    void eraseFix( RBTNode<Key, Value> *node );
    void eraseNode( RBTNode<Key, Value> *node );
    void clear( RBTNode<Key, Value> *node );
    RBTNode<Key, Value>* find( RBTNode<Key, Value> *node, Key key );
    unsigned int size( RBTNode<Key, Value> *node );
};
#include "rbTree.cpp"
#endif