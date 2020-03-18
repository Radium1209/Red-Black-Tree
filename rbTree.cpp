#include <stdio.h>
#include <algorithm>
#include <queue>
#include <iostream>
#include <string>

/* 构造函数 */
template<class Key, class Value>
RBTree<Key, Value>::RBTree()
: root( NULL ) {}

/* 析构函数 */
template<class Key, class Value>
RBTree<Key, Value>::~RBTree()
{
    clear();
}

/* 清空操作 */
template<class Key, class Value>
void RBTree<Key, Value>::clear()
{
    clear( root );
    root = NULL;
}

/* 删除所有节点 */
template<class Key, class Value>
void RBTree<Key, Value>::clear( RBTNode<Key, Value> *node )
{
    if ( node == NULL )
    {
        return ;
    }
    if ( node->leftChild != NULL )
    {
        clear( node->leftChild );
    }
    if ( node->rightChild != NULL )
    {
        clear( node->rightChild );
    }
    delete node;
}

/* 左旋操作示例
 * 对红黑树的节点(x)进行左旋转
 *
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \      --(左旋)-->           / \
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 *
 * 其中需要变化的值：
 * px->Child = y
 * x->father = y
 * x->rightChild = ly
 * y->father = px
 * y->leftChild = x
 * ly->father = x
 */
template<class Key, class Value>
void RBTree<Key, Value>::leftRotate( RBTNode<Key, Value> *node )
{
    RBTNode<Key, Value> *x = node;
    RBTNode<Key, Value> *y = x->rightChild;

    // 修改x->rightChild = ly
    x->rightChild = y->leftChild;
    
    // 修改ly->father = x, 注意ly可能为空
    if ( y->leftChild != NULL)
    {
        y->leftChild->father = x;
    }
    
    // 修改y->father = px
    y->father = x->father;

    // 修改px-Child = y，此时需要考虑px为NULL以及左右孩子的区别
    if ( x->father == NULL )
    {
        root = y;   // 此时要更新root
    }
    else 
    {
        if ( x->father->leftChild == x )
        {
            x->father->leftChild = y;
        }
        else
        {
            x->father->rightChild = y;
        }
    }

    // 修改y->leftChild = x
    y->leftChild = x;
    
    // 修改x->father = y
    x->father = y;
}

/* 右旋，左旋的逆操作 */
template<class Key, class Value>
void RBTree<Key, Value>::rightRotate( RBTNode<Key, Value>* node )
{
    RBTNode<Key, Value> *y = node;
    RBTNode<Key, Value> *x = y->leftChild;

    y->leftChild = x->rightChild;
    
    if ( x->rightChild != NULL )
    {
        x->rightChild->father = y;
    }

    x->father = y->father;

    if ( y->father == NULL )
    {
        root = x;
    }
    else
    {
        if ( y->father->leftChild == y )
        {
            y->father->leftChild = x;
        }
        else
        {
            y->father->rightChild = x;
        }
    }

    x->rightChild = y;

    y->father = x;
}

/* 插入 */
template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::insert( Key key ) {
    return insert( key, key );
}

/* 插入 */
template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::insert( Key key, Value value ) {
    RBTNode<Key, Value> *node = NULL;

    if ( ( node = new RBTNode<Key, Value>( BLACK, key, value, NULL, NULL, NULL ) ) == NULL )
    {
        return NULL;
    }

    insert( node );

    return node;
}

/* 插入 */
template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::insertUnique( Key key ) {
    return insertUnique( key, key );
}

/* 插入 */
template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::insertUnique( Key key, Value value ) {
    RBTNode<Key, Value>* node = find( key );
    if ( node == NULL )
    {
        return insert( key, value );
    }
    return node;
}

/* 插入具体实现
 * 插入的的节点默认为红色，且先按照普通的二叉排序树插入
 * 新生成的树可能不满足红黑树特性，需要进行修正
 * 可能会违背的红黑树特性是特性4：如果一个节点是红色的，那么他的子节点必须是黑色的
 */
template<class Key, class Value>
void RBTree<Key, Value>::insert( RBTNode<Key, Value>* node )
{
    RBTNode<Key, Value> *y = NULL;
    RBTNode<Key, Value> *x = root;

    // 当做二叉排序树正常插入
    while ( x != NULL )
    {
        y = x;
        if ( node->key < x->key )
        {
            x = x->leftChild;
        }
        else
        {
            x = x->rightChild;
        }
    }

    node->father = y;
    if ( y != NULL )
    {
        if ( node->key < y->key )
        {
            y->leftChild = node;
        }
        else
        {
            y->rightChild = node;
        }
    }
    else
    {
        root = node;
    }

    node->color = RED;

    insertFix( node );
}

/* 插入修正函数，用于修正树使满足红黑树特性
 * 需要进行操作则说明违反了特性4，则当前节点的父节点为红色
 * 共有3种操作1.变颜色，2.左旋，3.右旋
 * 进行何种操作取决于父节点的兄弟节点（叔节点）颜色以及当前节点node、父亲节点fa以及祖父节点gfa的关系
 * 如果叔节点为红，则只变颜色
 * 叔节点为黑，则需要旋转
 * gfa,fa和node的关系共有4种：LL,LR,RL,RR
 * 旋转的方向根据node与fa的关系（即第二个参数），如果是左子树(L)右旋，否则(R)左旋
 * 旋转的对象根据node,fa和gfa的关系，如果为RL或者LR旋转fa，如果为RR或者LL旋转gfa
 */
template<class Key, class Value>
void RBTree<Key, Value>::insertFix( RBTNode<Key, Value>* node )
{
    if ( node == NULL )
    {
        return ;
    }

    // fa--->父亲, gfa--->祖父, un--->叔叔
    RBTNode<Key, Value> *fa, *gfa, *un;

    // 如果父节点存在，且为红色
    while ( ( ( fa = node->father ) != NULL ) && ( fa->color == RED) )
    {
        gfa = fa->father;

        // 父亲节点是祖父节点的左孩子，所以只能出现LL和LR情况
        if ( fa == gfa->leftChild )
        {
            un = gfa->rightChild;
            // 叔节点是红色的
            if ( un != NULL && un->color == RED )
            {
                fa->color = BLACK;
                un->color = BLACK;
                gfa->color = RED;
                node = gfa;
                continue;
            }
            else
            {
                // LR型，左旋父节点，旋转完后会变成LL型
                if ( fa->rightChild == node )
                {
                    leftRotate( fa );
                    std::swap( fa, node );
                }

                // LL型，右旋祖父节点
                fa->color = BLACK;
                gfa->color = RED;
                rightRotate( gfa );
            }
        }
        // 父亲节点是祖父节点的右孩子，所以只能出现RL和RR情况
        else
        {
            un = gfa->leftChild;
            // 叔节点是红色的
            if ( un != NULL && un->color == RED )
            {
                fa->color = BLACK;
                un->color = BLACK;
                gfa->color = RED;
                node = gfa;
                continue;
            }
            else
            {
                // RL型，右旋父节点，旋转完后会变成RR型
                if ( fa->leftChild == node )
                {
                    rightRotate( fa );
                    std::swap( fa, node );
                }

                // RR型，左旋祖父节点
                fa->color = BLACK;
                gfa->color = RED;
                leftRotate( gfa );
            }
        }
    }

    root->color = BLACK;
}

/* 删除节点 */
template<class Key, class Value>
void RBTree<Key, Value>::erase( Key key )
{
    RBTNode<Key, Value>* node = find( key );
    if ( node != NULL )
    {
        erase( node );
    }
    else
    {
        std::cout << "Can't find node" << std::endl;
    }
}

/* 删除具体实现
 * 和普通的二叉搜索树类似，通过转换都变为情况1，其中只有情况1.2需要修正
 * 1. 无子节点时，删除节点可能为红色或者黑色
 *   1.1 红色，直接删除
 *   1.2 黑色，需要修正
 * 2. 只有一个子节点，当前节点必为黑色，交换当前节点和子节点，删除子节点，此时转换成情况1
 * 3. 两个子节点，将当前节点与后继节点交换，变为删除后继节点。
 *   3.1 后继节点是叶节点，变为情况1
 *   3.2 有一个子节点，变成情况2
 */
template<class Key, class Value>
void RBTree<Key, Value>::erase( RBTNode<Key, Value>* node )
{
    if ( node == NULL )
    {
        return ;
    }

    RBTNode<Key, Value> *child, *father;
    RBTColor color;

    // 情况1
    if ( node->leftChild == NULL && node->rightChild == NULL )
    {
        // 情况1.2
        if ( node->color == BLACK )
        {
            if ( node == root )
            {
                eraseNode( node );
                return ;
            }
            eraseFix( node );
        }
    }
    // 情况2
    else if ( node->leftChild == NULL || node->rightChild == NULL )
    {
        child = node->leftChild ? node->leftChild : node->rightChild;

        std::swap( node->key, child->key );
        std::swap( node->value, child->value );
        
        erase( child );
        return ;
    }
    // 情况3，有两个孩子
    else
    {
        // 待删节点的后继节点，用它来取代待删节点
        RBTNode<Key, Value> *replace = node;

        // 求后继节点，后继节点是右孩子的最左下角的孩子
        replace = replace->rightChild;
        while ( replace->leftChild != NULL )
        {
            replace = replace->leftChild;
        }

        std::swap( node->key, replace->key );
        std::swap( node->value, replace->value );
        
        erase( replace );
        return ;
    }
    eraseNode( node );
}

/* 删除修正
 *
 */
template<class Key, class Value>
void RBTree<Key, Value>::eraseFix( RBTNode<Key, Value>* node )
{
    if ( node == NULL || node == root )
    {
        return ;
    }

    RBTNode<Key, Value> *brother, *father;
    father = node->father;
    
    while ( ( node == NULL || node->color == BLACK ) && node != root )
    {
        // 当前节点是左孩子
        if ( node = father->leftChild )
        {
            brother = father->rightChild;

            // 1. 兄弟是红色的
            if ( brother != NULL && brother->color == RED )
            {
                brother->color = BLACK;
                father->color = RED;
                leftRotate( father );
                brother = father->rightChild;
            }
            if ( ( brother->leftChild == NULL || brother->leftChild->color == BLACK ) &&
                ( brother->rightChild == NULL || brother->rightChild->color == BLACK) )
            {
                brother->color = RED;
                node = father;
                father = node->father;
            }
            else
            {
                if ( brother->rightChild == NULL || brother->rightChild->color == BLACK )
                {
                    if ( brother->leftChild != NULL )
                    {
                        brother->leftChild->color = BLACK;
                    }
                    brother->color = RED;
                    rightRotate( brother );
                    brother = father->rightChild;
                }

                brother->color = father->color;
                father->color = BLACK;
                if ( brother->rightChild )
                {
                    brother->rightChild->color = BLACK;
                }
                leftRotate( father );
                node = root;
                break;
            }
        }
        else
        {
            brother = father->leftChild;

            // 1. 兄弟是红色的
            if ( brother != NULL && brother->color == RED )
            {
                brother->color = BLACK;
                father->color = RED;
                rightRotate( father );
                brother = father->leftChild;
            }
            if ( ( brother->leftChild == NULL || brother->leftChild->color == BLACK ) &&
                ( brother->rightChild == NULL || brother->rightChild->color == BLACK) )
            {
                brother->color = RED;
                node = father;
                father = node->father;
            }
            else
            {
                if ( brother->leftChild == NULL || brother->leftChild->color == BLACK )
                {
                    if ( brother->rightChild != NULL )
                    {
                        brother->rightChild->color = BLACK;
                    }
                    brother->color = RED;
                    leftRotate( brother );
                    brother = father->leftChild;
                }

                brother->color = father->color;
                father->color = BLACK;
                if ( brother->leftChild )
                {
                    brother->leftChild->color = BLACK;
                }
                rightRotate( father );
                node = root;
                break;
            }
        }
    }
    if ( node )
        node->color = BLACK;
}

template<class Key, class Value>
void RBTree<Key, Value>::eraseNode( RBTNode<Key, Value> *node )
{
    if ( node == root )
    {
        root = NULL;
    }
    else
    {
        if ( node->father->leftChild == node )
        {
            node->father->leftChild = NULL;
        }
        else
        {
            node->father->rightChild = NULL;
        }
    }

    delete node;
}

/* 查询函数 */
template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::find( Key key )
{
    return find( root, key );
}

template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::find( RBTNode<Key, Value> *node, Key key )
{
    if ( node == NULL || node->key == key)
    {
        return node;
    }
    if ( key < node->key )
    {
        return find( node->leftChild, key );
    }
    else
    {
        return find( node->rightChild, key );
    }   
}

template<class Key, class Value>
void RBTree<Key, Value>::draw()
{
    if ( root == NULL )
    {
        std::cout << "NULL" << std::endl;
        return ;
    }

    NLPair<Key, Value> Now( NULL, 0 ), Next( NULL, 0 );
    std::queue<NLPair<Key, Value>> Que;
    int layer = 0;

    Que.push( NLPair<Key, Value>( root, layer ) );
    while( !Que.empty() )
    {
        Now = Que.front();
        Que.pop();
        if ( Now.node == NULL )
        {
            continue;
        }
        if ( Now.layer != layer )
        {
            layer = Now.layer;
            std::cout << std::endl;
        }
        std::cout << Now.node->key << "(";
        if ( Now.node->father != NULL )
        {
            std::cout << Now.node->father->key << " ";
        }
        std::cout << (Now.node->color == RED ? "RED" : "BLACK") << ")" << "\t";
        Next = NLPair<Key, Value>( Now.node->leftChild, Now.layer + 1 );
        Que.push( Next );
        Next = NLPair<Key, Value>( Now.node->rightChild, Now.layer + 1 );
        Que.push( Next );
    }
    std::cout << std::endl;
}

template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::begin()
{
    if ( root == NULL )
    {
        return NULL;
    }
    RBTNode<Key, Value>* ret = root;
    while ( ret->leftChild )
    {
        ret = ret->leftChild;
    }
    return ret;
}

template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::end()
{
    if ( root == NULL )
    {
        return NULL;
    }
    RBTNode<Key, Value>* ret = root;
    while ( ret->rightChild )
    {
        ret = ret->rightChild;
    }
    return ret;
}

template<class Key, class Value>
unsigned int RBTree<Key, Value>::size()
{
    return size( root );
}

template<class Key, class Value>
unsigned int RBTree<Key, Value>::size( RBTNode<Key, Value> *node )
{
    if ( node == NULL )
    {
        return 0;
    }
    return ( size( node->leftChild ) + size( node->rightChild ) + 1 );
}

template<class Key, class Value>
bool RBTree<Key, Value>::empty()
{
    if ( root == NULL )
    {
        return true;
    }
    return false;
}

/* 判断是不是二叉搜索树，判断红黑树的时候用 */
template<class Key, class Value>
bool RBTree<Key, Value>::isBST( RBTNode<Key, Value> *node )
{
    if ( node == NULL )
    {
        return true;
    }

    bool flagL = true, flagR = true;
    if ( node->leftChild != NULL )
    {
        if ( node->leftChild->key >= node->key )
        {
            flagL = false;
        }
    }

    if ( node->rightChild != NULL )
    {
        if ( node->key >= node->rightChild->key )
        {
            flagR = false;
        }
    }

    if ( flagL == false || flagR == false )
    {
        return false;
    }

    if ( isBST( node->leftChild ) && isBST( node->rightChild ) )
    {
        return true;
    }

    return false;
}

/* 判断是不是红黑树，测试用 */
template<class Key, class Value>
bool RBTree<Key, Value>::isRBT()
{
    if ( root == NULL )
    {
        return true;
    }

    if ( !isBST( root ) )
    {
        std::cout << "Not a BST!" << std::endl;
        return false;
    }

    if ( root->color == RED )
    {
        std::cout << "Root is red!" << std::endl;
        return false;
    }

    if ( hasTwoRed( root ) )
    {
        return false;
    }

    if ( !hasSameBlack( root ) )
    {
        return false;
    }

    return true;
}

/* 判断是否有两个连续的红色节点 */
template<class Key, class Value>
bool RBTree<Key, Value>::hasTwoRed( RBTNode<Key, Value>* node )
{
    if ( node == NULL )
    {
        return false;
    }

    if ( node != root )
    {
        if ( node->father->color == RED && node->color == RED )
        {
            std::cout << "Has tow red node!" << std::endl;
            std::cout << "node key = " << node->key << std::endl;
            return true;
        }
    }

    if ( hasTwoRed( node->leftChild ) || hasTwoRed( node->rightChild ) )
    {
        return true;
    }

    return false;
}

/* 判断每个节点的所有路径上的黑色节点个数是否相等 */
template<class Key, class Value>
bool RBTree<Key, Value>::hasSameBlack( RBTNode<Key, Value>* node )
{
    if ( node == NULL )
    {
        return true;
    }

    node->blackNum = -1;

    if ( node->leftChild == NULL && node->rightChild == NULL) 
    {
        calBlackNum( node, 0 );
        return true;
    }

    if ( node->leftChild )
    {
        if ( !hasSameBlack( node->leftChild ) )
        {
            return false;
        }
    }

    if ( node->rightChild )
    {
        if ( !hasSameBlack( node->rightChild ) )
        {
            return false;
        }
    }

    if ( node->blackNum == -2 )
    {
        std::cout << "Don't has same black node!" << std::endl;
        std::cout << "node key = " << node->key << std::endl;
        return false;
    }

    return true;
}

/* 辅助函数，用于计数路径上黑色节点个数 */
template<class Key, class Value>
void RBTree<Key, Value>::calBlackNum( RBTNode<Key, Value>* node, int blackNum )
{
    if ( node == NULL )
    {
        return ;
    }

    if ( node->color == BLACK )
    {
        if ( node->blackNum == -1 )
        {
            node->blackNum = ++blackNum;
        }
        else if ( node->blackNum != -2 )
        {
            if ( node->blackNum != ++blackNum )
            {
                node->blackNum = -2;
            }
        }
    }

    if ( node->father != NULL )
    {
        calBlackNum( node->father, blackNum );
    }
}