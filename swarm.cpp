
#include "swarm.h"
const int ONE = 1;
const int NEGATIVE_ONE = -1;
const int ZERO = 0;
Swarm::Swarm(){
    m_root = nullptr;
}

Swarm::~Swarm(){
    deleteTreeHelper(m_root);
}

void Swarm::insert(const Robot& robot){
    // protect against case where object being inserted is already in tree
    if(findBot(robot.getID())){
        cout << "Robot with ID " << robot.getID() << " already exsit can not insert it again" << endl;
        return;
    }
    if(robot.getID() <= ZERO){
        throw std::out_of_range("Robot with value of zero or lower than zero can not be created");
        return;
    }
        m_root = insertRecursive(m_root, robot.getID(),robot);
}

void Swarm::clear(){
    if(m_root == nullptr){
        cout << "Tree is already cleared" << endl;
        return;
    }
    deleteTreeHelper(m_root);
}

void Swarm::remove(int id){
    // throw errors if tree is false or robot isnt in tree
    if(m_root == nullptr){
        throw std::out_of_range("Can not remove a node from empty Tree");
        return;
    }if(findBot(id) == false){
        throw std::out_of_range("Robot with this ID is not in the tree can not remove!");
        return;
    // case if robot to remove is the current root
    }if(id == m_root -> getID()){
        Robot* temp = findMin(m_root -> getRight(), ONE);
        // copying over temp's data to update root
        m_root -> setID(temp -> getID());
        m_root -> setState(temp -> getState());
        m_root -> setType(temp -> getType());
        temp = nullptr;
        // deleting temp since copy
        delete temp;
        // updating and rebalancing tree
        updateHeight(m_root);
        m_root = rebalance(m_root);
        return;
    }
    else{
        removeRecursive(m_root, id);
    }
}

void Swarm::updateHeight(Robot* aBot){
     if ( aBot == nullptr ) {
         return;
     }
    // getting left and right subtree heights
   int leftHeight = ( aBot ->getLeft() == nullptr ? NEGATIVE_ONE : aBot->getLeft()->getHeight() );
   int rightHeight = ( aBot->getRight() == nullptr ? NEGATIVE_ONE : aBot->getRight()->getHeight() );
   // setting abots height to whatever one is bigger + 1
   aBot -> setHeight(ONE + (leftHeight > rightHeight ? leftHeight : rightHeight ));
}

int Swarm::checkImbalance(Robot* aBot){
    if (aBot == nullptr)
        return NEGATIVE_ONE;
    else{
        int leftHeight = NEGATIVE_ONE;
        int rightHeight = NEGATIVE_ONE;
        if (aBot -> getLeft() != nullptr)
            leftHeight = aBot -> getLeft() -> getHeight();
        if (aBot -> getRight() != nullptr)
            rightHeight = aBot -> getRight() ->getHeight();
        return (leftHeight - rightHeight);
    }
}

Robot* Swarm::rebalance(Robot* aBot){
    // normal left rotation 
     if ((checkImbalance(aBot) < NEGATIVE_ONE) && (checkImbalance(aBot -> getRight()) <= ZERO)){
        return leftRotation(aBot);
     }
     // right rotation
    else if ((checkImbalance(aBot) > ONE) && (checkImbalance(aBot -> getLeft()) >= ZERO)){
        return rightRotation(aBot);
    }
    // right left rotation
    else if ((checkImbalance(aBot) < NEGATIVE_ONE) && (checkImbalance(aBot -> getRight()) >= ZERO)){
        aBot -> setRight(rightRotation(aBot -> getRight()));
        return leftRotation(aBot);
    }
    // left right rotation
    else if ((checkImbalance(aBot) > ONE) && (checkImbalance(aBot -> getLeft()) <= ZERO)){
        aBot -> setLeft(leftRotation(aBot -> getLeft()));
        return rightRotation(aBot);
    }
    else{
        // no rotation neccessary becuase no imbalances so just return aBot
         return aBot;
    }
}

void Swarm::listRobots() const {
    if(m_root == nullptr){
        throw std::out_of_range("Can not list an empty Tree");
        return;
    }
    listRobotsRecursive(m_root);

}

bool Swarm::setState(int id, STATE state){
    if(findBot(id)){
        Robot* temp = m_root;
        // traverse still id is found set state
        while (temp != nullptr){
                if(id == temp -> getID()){
                    temp -> setState(state);
                    return true;
                }else if(id < temp -> getID()){
                    temp = temp -> getLeft();
                }else{
                    temp = temp -> getRight();
                }
        }
    }else{
        throw std::out_of_range("Robot does not exist can not set state");
        return false;
    }
    return false;
}

void Swarm::removeDead(){
    if(m_root == nullptr){
        throw std::out_of_range("Can not remove from empty tree!");
        return;
    }
    // called twice incase in first call m_root was updated second call should catch any 
    // nodes missed because of m_root being updated
    removeDeadRecursive(m_root);
    removeDeadRecursive(m_root);
}

bool Swarm::findBot(int id) const {
    if(m_root == nullptr){
            return false;
        }else{
            Robot* temp = m_root;
            // traverse until Robot is found or temp is nullptr
            while (temp != nullptr){
                if(id == temp -> getID()){
                    return true;
                }else if(id < temp -> getID()){
                    temp = temp -> getLeft();
                }else{
                    temp = temp -> getRight();
                }
            }
            return false;
        }
}

void Swarm::dumpTree() const {
    dump(m_root);
}

void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}

void Swarm::deleteTreeHelper(Robot* node) {
    if (node == nullptr){
        return;
    }
    /* first delete both subtrees */
    deleteTreeHelper(node -> getLeft());
    deleteTreeHelper(node -> getRight());
     
    /* then delete the node */
    delete node;
}

Robot* Swarm::insertRecursive(Robot* node, int val,const Robot& orginal){
    // creating and allocating, setting states and values of new node
    if (node == nullptr){
        Robot* node = new Robot(val);
        node -> setState(orginal.getState());
        node -> setType(orginal.getType());
        return node;
    }
    // setting left and then updating and rebalancing tree
    else if ( node -> getID() > val ){
        node -> setLeft(insertRecursive(node -> getLeft(), val, orginal));
        updateHeight(node);
        return rebalance(node);
    // setting right then updating and rebalancing
    }else if (node -> getID() < val ){
        node -> setRight(insertRecursive(node -> getRight(), val, orginal));
        updateHeight(node);
        return rebalance(node);
    }
    else{
        return node;
    }

}

Robot* Swarm::rightRotation(Robot* aBot){
    Robot* z = aBot -> getLeft();
    Robot* y = z -> getRight();
    // making right rotation
    z -> setRight(aBot);
    aBot -> setLeft(y);
    updateHeight(aBot);
    updateHeight(z);
    // returning newly rotated node
    return z;
}

Robot* Swarm::leftRotation(Robot* aBot){
    Robot* z = aBot -> getRight();
    Robot* y = z -> getLeft();
    // making left rotation
    z -> setLeft(aBot);
    aBot -> setRight(y);
    updateHeight(aBot);
    updateHeight(z);
    // updating newly rotated node
    return z;
}

void Swarm::listRobotsRecursive(Robot* aBot) const{
    if(aBot == nullptr){
        return;
    }
    // in order traversal for print
    listRobotsRecursive(aBot -> getLeft());
    cout << aBot -> getID() << ":" << aBot -> getStateStr() << ":" << aBot -> getTypeStr() << endl;
    listRobotsRecursive(aBot -> getRight());
}

Robot* Swarm::removeRecursive(Robot* aBot, int ID){
    Robot* temp;
    if(aBot == nullptr){
        return nullptr;
    }
    else if(ID < aBot -> getID()){
        aBot -> setLeft(removeRecursive(aBot -> getLeft(), ID));
    }
    //traverse to right
    else if(ID > aBot -> getID()){
        aBot -> setRight(removeRecursive(aBot -> getRight(),ID));
    // the case of the node with two children
    }else if(aBot -> getLeft() && aBot -> getRight()){
        // find right’s lowest value
        temp = findMin(aBot -> getRight(),ZERO);
        // copy its value to the node that we want to delete
        aBot -> setID(temp -> getID());
        aBot -> setState(temp -> getState());
        aBot -> setType(temp -> getType());
        aBot -> setRight(removeRecursive(aBot -> getRight() , aBot -> getID()));
    }
    else{
        temp = aBot;
        if(aBot -> getLeft() == nullptr){
            aBot = aBot -> getRight();
        }else if(aBot -> getRight() == nullptr){
            aBot = aBot -> getLeft();
        }
        delete temp;
    }
    updateHeight(aBot);
    // update m_root to check for imbalances of whole tree
    if(aBot != nullptr && aBot -> getID() == m_root -> getID()){
        m_root = rebalance(m_root);
    }else{
        aBot = rebalance(aBot);
    }
    return aBot;
}

Robot* Swarm::findMin(Robot* aBot, int root){
    // if root == 1 it indicates we are working with the root of the tree
    if(aBot -> getLeft() != nullptr && root == ONE){
        if(aBot -> getLeft() -> getLeft() == nullptr && root == ONE){
            Robot* temp = aBot -> getLeft();
            aBot -> getLeft() -> setLeft(nullptr);
            return temp;
        }
    }if (aBot == nullptr || aBot -> getLeft() == nullptr){
        return aBot;
    }else{
        return findMin(aBot -> getLeft(), root);
    }
}

void Swarm::removeDeadRecursive(Robot* aBot){
    if(aBot == nullptr){
        return;
    }else{
        // post order traversal whenever state is dead will remove that robot
        removeDeadRecursive(aBot -> getLeft());
        removeDeadRecursive(aBot -> getRight());
        if(aBot -> getStateStr() == "DEAD"){
            cout << "Removing dead robot with ID " << aBot -> getID() << endl;
            remove(aBot -> getID());
        }
    }
}

