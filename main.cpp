//Talan He
//lab #5


#include <iostream>
#include <string>
#include <iomanip>
#include <random>
#include <limits>
#include <cctype>

using namespace std;

const string CREATURE_DEFAULT_NAME = "n/a";
const string CREATURE_DEFAULT_TYPE = "Unknown";
const int CREATURE_DEFAULT_STRENGTH = 10;
const int CREATURE_DEFAULT_HEALTH = 10;

const int CREATURE_MIN_STAT = 10;
const int CREATURE_MAX_STAT = 100;

const string CREATURE_TYPES[4] = { "Unknown", "Orc", "Elf", "Human" };

const string ARMY_DEFAULT_NAME = "n/a";
const int ARMY_DEFAULT_SIZE = 0;

const int ARMY_MIN_NAME_CHARS = 3;
const int ARMY_MIN_SIZE = 5;

//return a random number between low and high
int randomInt(int low, int high)
{
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(low, high);
    return dist(gen);
}

//pick one creature type at random
string randomCreatureType()
{
    return CREATURE_TYPES[randomInt(0, 3)];
}

//checkif a stat is inside the valid range
bool validStat(int value)
{
    return value >= CREATURE_MIN_STAT && value <= CREATURE_MAX_STAT;
}

//checking if the army name is at least 3 letters and only has letters
bool validArmyName(const string& name)
{
    if (static_cast<int>(name.size()) < ARMY_MIN_NAME_CHARS)
    {
        return false;
    }

    for (unsigned char ch : name)
    {
        if (!isalpha(ch))
        {
            return false;
        }
    }

    return true;
}

//checks if the army size is big enough
bool validArmySize(int size)
{
    return size >= ARMY_MIN_SIZE;
}

//repeatidly asking until the user enters a whole number
int readInt(const string& prompt)
{
    int value;

    while (true)
    {
        cout << prompt;

        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "invalid input enter a whole number\n";
    }
}

// stores a single creature
class Creature
{
private:
    string name = CREATURE_DEFAULT_NAME;
    string type = CREATURE_DEFAULT_TYPE;
    int strength = CREATURE_DEFAULT_STRENGTH;
    int health = CREATURE_DEFAULT_HEALTH;

public:
    Creature()
    {
        setCreature();
    }

    explicit Creature(const string& newName, const string& newType)
    {
        setCreature(newName, newType);
    }

    explicit Creature(const string& newName, const string& newType, int newStrength, int newHealth)
    {
        setCreature(newName, newType, newStrength, newHealth);
    }

    Creature(const Creature& other)
    {
        setCreature(other.name, other.type, other.strength, other.health);
    }

    ~Creature()
    {
        cout << name << " " << type << " destructor\n";
    }

    string getName() const
    {
        return name;
    }

    string getType() const
    {
        return type;
    }

    int getStrength() const
    {
        return strength;
    }

    int getHealth() const
    {
        return health;
    }

    //make the creature back to default values
    void setCreature()
    {
        setCreature(CREATURE_DEFAULT_NAME, CREATURE_DEFAULT_TYPE,
                    CREATURE_DEFAULT_STRENGTH, CREATURE_DEFAULT_HEALTH);
    }

    // this sets the name and type and gives random stats
    void setCreature(const string& newName, const string& newType)
    {
        setCreature(newName, newType,
                    randomInt(CREATURE_MIN_STAT, CREATURE_MAX_STAT),
                    randomInt(CREATURE_MIN_STAT, CREATURE_MAX_STAT));
    }

    //set al creature data in one place
    void setCreature(const string& newName, const string& newType, int newStrength, int newHealth)
    {
        if (!validStat(newStrength) || !validStat(newHealth))
        {
            cout << "invalid creature values setting default values\n";
            name = CREATURE_DEFAULT_NAME;
            type = CREATURE_DEFAULT_TYPE;
            strength = CREATURE_DEFAULT_STRENGTH;
            health = CREATURE_DEFAULT_HEALTH;
            return;
        }

        name = newName;
        type = newType;
        strength = newStrength;
        health = newHealth;
    }

    //cjange one value while keeping the rest the same
    void setName(const string& newName)
    {
        setCreature(newName, type, strength, health);
    }

    void setType(const string& newType)
    {
        setCreature(name, newType, strength, health);
    }

    void setStrength(int newStrength)
    {
        setCreature(name, type, newStrength, health);
    }

    void setHealth(int newHealth)
    {
        setCreature(name, type, strength, newHealth);
    }
};

//store an army of creatures
class Army
{
private:
    string name = ARMY_DEFAULT_NAME;
    int size = ARMY_DEFAULT_SIZE;
    Creature** creatures = nullptr;

    //delete all creatures and the pointer array
    void clear()
    {
        if (creatures != nullptr)
        {
            for (int i = 0; i < size; ++i)
            {
                delete creatures[i];
                creatures[i] = nullptr;
            }

            delete[] creatures;
            creatures = nullptr;
        }

        size = ARMY_DEFAULT_SIZE;
    }

    //create a new creature array and fills it
    Creature** makeArmyArray(int newSize)
    {
        Creature** temp = nullptr;

        try
        {
            temp = new Creature*[newSize];

            for (int i = 0; i < newSize; ++i)
            {
                temp[i] = new Creature(
                    "Creature_" + to_string(i + 1),
                    randomCreatureType(),
                    randomInt(CREATURE_MIN_STAT, CREATURE_MAX_STAT),
                    randomInt(CREATURE_MIN_STAT, CREATURE_MAX_STAT)
                );
            }
        }
        catch (...)
        {
            if (temp != nullptr)
            {
                for (int i = 0; i < newSize; ++i)
                {
                    delete temp[i];
                }

                delete[] temp;
            }

            throw;
        }

        return temp;
    }

    //make a deep copy of another army
    void copyFrom(const Army& other)
    {
        Creature** temp = nullptr;

        try
        {
            if (other.size > 0)
            {
                temp = new Creature*[other.size];

                for (int i = 0; i < other.size; ++i)
                {
                    temp[i] = new Creature(*other.creatures[i]);
                }
            }
        }
        catch (...)
        {
            if (temp != nullptr)
            {
                for (int i = 0; i < other.size; ++i)
                {
                    delete temp[i];
                }

                delete[] temp;
            }

            throw;
        }

        clear();
        name = other.name;
        size = other.size;
        creatures = temp;
    }

public:
    Army()
    {
        setArmy();
    }

    explicit Army(const string& newName, int newSize)
    {
        setArmy(newName, newSize);
    }

    Army(const Army& other)
    {
        copyFrom(other);
    }

    ~Army()
    {
        cout << "army " << name << " destructor in progress\n";
        clear();
    }

    //reset the army to default values
    void setArmy()
    {
        clear();
        name = ARMY_DEFAULT_NAME;
    }

    //creates the army if the input is valid
    void setArmy(const string& newName, int newSize)
    {
        if (!validArmyName(newName) || !validArmySize(newSize))
        {
            cout << "invalid army data setting default values\n";
            clear();
            name = ARMY_DEFAULT_NAME;
            return;
        }

        Creature** temp = makeArmyArray(newSize);

        clear();
        name = newName;
        size = newSize;
        creatures = temp;
    }

    //print the army in a tables
    void printArmy() const
    {
        if (creatures == nullptr || size == 0)
        {
            cout << "no army to display\n";
            return;
        }

        cout << left
             << setw(15) << "name"
             << setw(15) << "type"
             << setw(12) << "strength"
             << setw(10) << "health"
             << '\n';

        cout << string(52, '-') << '\n';

        for (int i = 0; i < size; ++i)
        {
            cout << left
                 << setw(15) << creatures[i]->getName()
                 << setw(15) << creatures[i]->getType()
                 << setw(12) << creatures[i]->getStrength()
                 << setw(10) << creatures[i]->getHealth()
                 << '\n';
        }
    }
};

// menu for the progrram
int main()
{
    Army army;
    int choice = 0;

    do
    {
        cout << "\narmy menu\n";
        cout << "1 create army\n";
        cout << "2 print army\n";
        cout << "3 quit\n";

        choice = readInt("enter choice: ");

        switch (choice)
        {
        case 1:
        {
            string armyName;
            int armySize;

            cout << "enter army name: ";
            getline(cin >> ws, armyName);

            armySize = readInt("enter army size: ");

            try
            {
                army.setArmy(armyName, armySize);
            }
            catch (const bad_alloc&)
            {
                cout << "memory allocation failed\n";
            }

            break;
        }
        case 2:
            army.printArmy();
            break;

        case 3:
            cout << "goodbye\n";
            break;

        default:
            cout << "invalid menu choice\n";
            break;
        }

    } while (choice != 3);

    return 0;
}




