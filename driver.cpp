#include "swarm.h"
#include <random>

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
            m_generator = std::mt19937(10);// 10 is the fixed seed value
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
    
};

int main(){
    Tester tester;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    {
        Swarm team;
        int teamSize = 10;
        int tempID = 0;
        int ID = 0;
        for(int i=0;i<teamSize;i++){
            ID = idGen.getRandNum();
            if (i == teamSize / 2) tempID = ID;//we store this ID for later use
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
        }
        cout << "\nDump after inserting " << teamSize << " nodes:\n\n";
        team.dumpTree();
        cout << "\n\nList of Robots after inserting " << teamSize << " nodes:\n";
        team.listRobots();
        cout << endl;

        team.remove(tempID);
        cout << "\nDump after removig the node with ID: " << tempID << "\n\n";
        team.dumpTree();
        cout << "\n\nList of Robots after removing the node with ID: " << tempID << "\n";
        team.listRobots();
        cout << endl;
    }
    return 0;
}

