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
template<class T>
class RBTNode
{
public:
    RBTNode( RBTColor col, T val, RBTNode* lch, RBTNode* rch, RBTNode* fa )
    : color( col ), value( val ), blackNum( 0 ), leftChild( lch ), rightChild( rch ), father( fa ) {}

    RBTColor color;         // 颜色(红或黑)
    T value;                // 数据
    int blackNum;           // 该节点的路径包含的黑色节点个数
    RBTNode *leftChild;     // 左孩子
    RBTNode *rightChild;    // 右孩子
    RBTNode *father;        // 父节点

public:
    RBTNode* getFather() { return father; };
    RBTNode* getLeft() { return leftChild; };
    RBTNode* getRight() { return rightChild; };
    T getValue() { return value; };
    RBTColor getColor() { return color; };
    void setFather( RBTNode* fa ) { father = fa; };
    void setLeft( RBTNode* left ) { leftChild = left; };
    void setRight( RBTNode* right ) { rightChild = right; };
    void setValue( T val ) { value = val; };
};

/* 节点-层数对，用于遍历 */
template<class T>
class NLPair
{
public:
    NLPair( RBTNode<T>* node, int layer )
    : node( node ), layer( layer ) {}

    RBTNode<T>* node;
    int layer;
};

template<class T>
class RBTree
{
    
private:
    RBTNode<T> *root;

public:
    RBTree();
    ~RBTree();
    bool isRBT();
    void insert( T value );
    void erase( T &key );
    void clear();
    RBTNode<T>* find( T &key );
    void draw();

private:
    bool isBST( RBTNode<T> *node );
    bool hasTwoRed( RBTNode<T> *node );
    bool hasSameBlack( RBTNode<T> *node );
    void calBlackNum( RBTNode<T> *node, int blackNum );
    void leftRotate( RBTNode<T> *root );
    void rightRotate( RBTNode<T> *root );
    void insert( RBTNode<T> *node );
    void insertFix( RBTNode<T> *node );
    void erase( RBTNode<T> *node );
    void eraseFix( RBTNode<T> *node );
    void eraseNode( RBTNode<T> *node );
    void clear( RBTNode<T> *node );
    RBTNode<T>* find( RBTNode<T> *node, T &key );
};
#endif
