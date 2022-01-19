
// Amarjot Gill
// 10/24/2021
// this file test edge cases, error cases, proves insertion and deletion is log(n)
// and test normal cases for the swarm class


#include "swarm.h"
#include <random>
const int ROBOTNUMBER = 25000;
const int TWO = 2;
const int ZERO = 0;
const int NEGATIVE_ONE = -1;

enum RANDOM {UNIFORM, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORM) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 and standard deviation of 20 
            m_normdist = std::normal_distribution<>(50,20);
        }
        else{
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(25);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
    }
    int getRandNum(){
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else{
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//uniform distribution

};

class Tester{
    public:
    bool testBST(Robot* currentNode);
    Robot* getRoot(const Swarm & team);
    bool testBalance(Robot* currentNode);
    // helpers to help test balance of tree
    int max(int a, int b);
    int getHeight(Robot* aNode);
    void testInsertion(Swarm &team, int idInsert);
    void testRemove(Swarm &team, int idRemove);
};

int main(){
    Tester tester;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    {   
        // for setting states to DEAD
        STATE d = DEAD;
        Swarm team;
        int teamSize = 10;
        int tempID = 0;
        int ID = 0;
        int secondTemp = 0;
        int thirdTemp = 0;
        for(int i=0;i<teamSize;i++){
            ID = idGen.getRandNum();
            // for testing insertion of node already in tree
            if(i == 1){
                secondTemp = ID;
            }
            if (i == teamSize / 2){
                tempID = ID;
            } //we store this ID for later use
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));

            // giving some robots dead state for dead removal testing
            if(i == 6 || i == 9 || i == 4){
                robot.setState(d);
            }
            // Saving other IDs for testing removal of tree
            if(i == 7){
                thirdTemp = ID;
            }
            team.insert(robot);
        }
        cout << "\nDump after inserting " << teamSize << " nodes:\n\n";
        team.dumpTree();
        cout << "\n\nList of Robots after inserting " << teamSize << " nodes:\n";
        team.listRobots();
        cout << endl;

        cout << "TESTING TREE IS BST AFTER INTIAL INSERTION" << endl;
        Robot* temp = tester.getRoot(team);
        if(tester.testBST(temp)){
            cout << "Test passed!" << endl;
        }else{
            cout << "Test failed" << endl;
        }

        cout << "TESTING TREE IS BALANCED AFTER INTIAL INSERTION" << endl;
        temp = tester.getRoot(team);
        if(tester.testBalance(temp)){
            cout << "Test passed!" << endl;
        }else{
            cout << "Test failed" << endl;
        }

        cout << "TRYING TO INSERT NODE " << secondTemp <<" THAT IS ALREADY IN TREE" << endl;
        Robot newRobot(secondTemp,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
        team.insert(newRobot);

        try{
            team.remove(tempID);
            cout << "\nDump after removing the node with ID: " << tempID << "\n\n";
            team.dumpTree();
            cout << "\n\nList of Robots after removing the node with ID: " << tempID << "\n";
            team.listRobots();
            cout << endl;

        }catch(std::out_of_range const &e){
            cout << e.what() << endl;
        }

        cout << "TRYING TO INSERT NODE WITH VALUE 0 " << endl;
        Robot robotNew(ZERO,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
        try{
            team.insert(robotNew);
        }catch(std::out_of_range const & e){
            cout << e.what() << endl;
        }

        cout << "TRYING TO INSERT NODE WITH VALUE -1 " << endl;
        Robot trialRobot(NEGATIVE_ONE,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
        try{
            team.insert(robotNew);
        }catch(std::out_of_range const & e){
            cout << e.what() << endl;
        }

        cout << "TRYING TO REMOVE NODE WITH ID " << tempID << " AGAIN " << endl; 
        try{
            team.remove(tempID);
        }catch(std::out_of_range const &e){
            cout << e.what() << endl;
        }
        cout << endl;

        {
            try{
                cout << "Removing Dead Robots" << endl;
                team.removeDead();
                cout << "Dump after removing dead" << endl;
                team.dumpTree();
                cout << endl;
                cout << "List of Robots after removing dead" << endl;
                team.listRobots();
                cout << endl;
            }catch(std::out_of_range const &e){
                cout << e.what() << endl;
        }

            cout << "TESTING IF BST AFTER MULTPLE DELETIONS" << endl;
            temp = tester.getRoot(team);
            if(tester.testBST(temp)){
                cout << "Test passed!" << endl;
            }else{
                cout << "Test failed" << endl;
            }
            cout << endl;

            cout << "TESTING IF TREE IS BALANCED AFTER MULTPLE DELETIONS" << endl;
            temp = tester.getRoot(team);
            if(tester.testBalance(temp)){
                cout << "Test passed!" << endl;
            }else{
                cout << "Test failed" << endl;
            }
            cout << endl;
        }

        Swarm newTeam;
        cout << "TRYING TO DEAD REMOVE FROM EMPTY TREE" << endl;
        try{
            newTeam.removeDead();
        }catch(std::out_of_range const &e){
            cout << e.what() << endl;
        }
        cout << endl;

        cout << "TRYING TO REMOVE FROM EMPTY TREE" << endl;
        try{
            newTeam.remove(TWO);
        }catch(std::out_of_range const &e){
            cout << e.what() << endl;
        }
        cout << endl;

        cout << "TRYING TO LIST EMPTY TREE" << endl;
        try{
            newTeam.listRobots();
        }catch(std::out_of_range const &e){
            cout << e.what() << endl;
        }
        cout << endl;

        cout << "TRYING TO REMOVE NODE NOT IN TREE" << endl;
        try{
            team.remove(TWO);
        }catch(std::out_of_range const &e){
            cout << e.what() << endl;
        }
        cout << endl;

        tester.testInsertion(team,ROBOTNUMBER);
        cout << endl;

        tester.testRemove(team,secondTemp);
        tester.testRemove(team,thirdTemp);
    }
    return 0;
}

bool Tester::testBST(Robot* currentNode){
   if (currentNode == nullptr){
       return true; 
   }
  // false if left is > than node 
  if (currentNode -> getLeft() != nullptr && currentNode ->getLeft() ->getID() > currentNode -> getID()){
    return false; 
  }
  // false if right is < than node 
  if (currentNode -> getRight() != nullptr && currentNode ->getRight() ->getID() < currentNode -> getID()){
    return false; 
  }
  // false if the left or right is not a BST 
  if (!testBST(currentNode->getLeft()) || !testBST(currentNode->getRight())){
    return false; 
  }   
  return true;
}

Robot* Tester::getRoot(const Swarm & team){
    return team.m_root;
}

bool Tester::testBalance(Robot* currentNode){
    if (currentNode == nullptr){
        return true;
    }

    int leftHeight;
    int rightHeight;
    // getting left and right tree height
    leftHeight = getHeight(currentNode -> getLeft());
    rightHeight = getHeight(currentNode -> getRight());
    
    // if tree heights are less than or equal to 1 and left and right are balanced then return true
    if (abs(leftHeight - rightHeight) <= 1 && testBalance(currentNode -> getLeft()) && testBalance(currentNode -> getRight())){
        return true;
    }

    return false;
}

int Tester::max(int a, int b){
    return (a >= b) ? a : b;
}

// helper to get heights of subtrees
int Tester::getHeight(Robot* aNode){
    if (aNode == nullptr){
        return 0;
    }
    return 1 + max(getHeight(aNode->getLeft()),getHeight(aNode->getRight()));
}

void Tester::testInsertion(Swarm &team, int idInsert){
    // constant factor to multiple values by
    const int a = 2;
    double T = 0.0;
    clock_t start, stop; 
    // creating robot and checking insertion time which proves log(n) time 
    Robot robot(idInsert);
    start = clock();
    team.insert(robot);
    stop = clock();
    T = stop - start;
    cout << "Inserting ID " << idInsert << " took " << T << " seconds" << endl;
    // multiplying by constant factor
    idInsert = idInsert * a;

    // second trial for insertion time
    Robot robotTwo(idInsert);
    start = clock();
    team.insert(robotTwo);
    stop = clock();
    T = stop - start;
    cout << "Inserting ID " << idInsert << " took " << T << " seconds" << endl;
    idInsert = idInsert * a;

    // third trial for insertion time
    Robot robotThree(idInsert);
    start = clock();
    team.insert(robotThree);
    stop = clock();
    T = stop - start;
    cout << "Inserting ID " << idInsert << " took " << T << " seconds" << endl;
    cout << "dumping tree after testing insertions" << endl;
    team.dumpTree();
    cout << endl;
}

void Tester::testRemove(Swarm &team, int idRemove){
    double T = 0.0;
    clock_t start, stop; 
    // checks the time taken to remove robot with idRemove, proves removal takes log(n) time
    start = clock();
    team.remove(idRemove);
    stop = clock();
    T = stop - start;
    cout << "Removing ID " << idRemove << " took " << T << " seconds" << endl;
    cout << "Displaying tree after removal" << endl;
    team.dumpTree();
    cout << endl;
}