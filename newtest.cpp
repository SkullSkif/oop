#include <iostream>
#include <vector>

#define stay 1
#define flying 2
#define moving 3

#define TABLESIZE 10

#define ran (rand() % TABLESIZE)

#ifdef _WIN32
#include <Windows.h>
#include <stdlib.h>
#endif

#ifdef __linux__
#include <stdlib.h>
#include <unistd.h>
#define Sleep(x) sleep((x / 1000))
#define system(cls) system("clear")
#endif

using std::vector;

using namespace std;

class Alive {
protected:
    double hunger = 0;

public:
    virtual void changeHunger(double amount) = 0;
};

class Creature {
protected:
    int x = 0, y = 0;
    char state = 0;
    char symbol = 0;
public:
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    void setX(int a)
    {
        x = a;
    }
    void setY(int b)
    {
        y = b;
    }
    virtual void spawn(char** table, int a, int b) = 0;
    virtual void action(char** table) = 0;
    virtual void printinfo(const char* name)
    {
        cout << "obj: " << name << " | X:" << this->x << " Y:" << this->y
             << endl;
    }
};

class Plant : public Creature {
    char symbol = '*';
    char state = stay;
    int timeleft = 10;
    int living = 1;
public:
    int num = 0;
    void timepass()
    {
        this->timeleft--;
    }
    Plant()
    {
        x = rand() % (TABLESIZE);

        y = rand() % (TABLESIZE);
    }
    Plant(int a, int b)
    {
        x = a;

        y = b;
    }
    int isAlive()
    {
        return living;
    }
    void spawn(char** table, int a, int b)
    {
        x = a;
        y = b;
        table[x][y] = symbol;
    }
    void spawn(char** table)
    {
        while (table[x][y] != '#') {
            this->x = rand() % (TABLESIZE);
            this->y = rand() % (TABLESIZE);
        }
        table[x][y] = symbol;
    }
    int getNum()
    {
        return num;
    }
    void setNum(int n)
    {
        num = n;
    }
    void action(char** table)
    {
        timeleft--;
        if (timeleft == 0) {
            table[this->x][this->y] = '#';
            living = 0;
        }
        if (table[x][y] != '*') {
            living = 0;
            table[x][y] = '#';
        }
    }
};

class Predator : public Creature, Alive {
    char symbol = '@';
    char state = flying;
    int living = 1;

public:
    Predator()
    {
        hunger = 1.5;
        x = rand() % (TABLESIZE);
        y = rand() % (TABLESIZE);
    }
    Predator(int a, int b)
    {
        x = a;

        y = b;
    }
    void changeHunger(double amount)
    {
        this->hunger += amount;
    }
    void spawn(char** table, int a, int b)
    {
        x = a;
        y = b;
        while (x==y){
            x = ran;
            y = ran;
        }
        table[x][y] = symbol;
    }
    void action(char** table)
    {
        table[x][y] = '#';
        x++;
        y++;
        if (x >= TABLESIZE){
            x = 0;
            y = TABLESIZE - y;
        }
        if (y >= TABLESIZE){
            x = TABLESIZE - x;
            y = 0;
        }
        if ((x >= TABLESIZE) && (y >= TABLESIZE)){
            x = 0;
            y = 1;
        }
        if (table[x][y] == '*') {
            x++;
            y++;
        }

        if (table[x][y] == '%')
            changeHunger(0.2);

        table[x][y] = symbol;
    }
};

class Animal : public Creature, Alive {
    char symbol = '%';
    char state = moving;
    int living = 1;
public:
    Animal()
    {
        hunger = 1.0;
        x = rand() % (TABLESIZE);
        y = rand() % (TABLESIZE);
    }
    Animal(int a, int b)
    {
        x = a;
        y = b;
    }
    int isAlive()
    {
        return this->living;
    }
    double getHunger()
    {
        return this->hunger;
    }
    void changeHunger(double amount)
    {
        this->hunger += amount;
    }
    void spawn(char** table, int a, int b)
    {
        x = a;
        y = b;
        if (table[x][y] == '#')
            table[x][y] = symbol;
    }
    void action(char** table)
    {
        int moveX, moveY;
        int count = 10;
        if (table[x][y] != '%')
            living = 0;

        do {
            if (count == 0)
                break;

            moveX = x;
            moveY = y;
            moveX += ((rand() % 3) - 1);
            moveY += ((rand() % 3) - 1);
            count--;
        } while (table[x][y] != '#');

        if (count != 0) {
            table[x][y] = '#';
            x = moveX;
            y = moveY;
            if (table[x][y] == '*')
                changeHunger(0.2);
            else 
                changeHunger(-0.2);

            table[x][y] = '%';
        }
        if (hunger<=0)
            living = 0;
    }
};

char** createTable(int x, int y)
{
    char** table = new char*[x];
    for (int i = 0; i < x; i++)
        table[i] = new char[y];

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++)
            table[i][j] = '#';
    }
    return table;
}

void printTable(char** table)
{
    int x, y;
    x = TABLESIZE;
    y = TABLESIZE;
    for (int i = 0; i < x; i++) {
        cout << endl;
        for (int j = 0; j < y; j++)
            cout << table[j][i] << ' ';
    }
}

int main()
{
    srand(time(0));
    char** Table = createTable(TABLESIZE+1, TABLESIZE+1);
    int time = 1000;
    std::vector<Plant*> plants;
    std::vector<Animal*> animals;
    std::vector<Predator*> predators;
    Plant* newPlant = new Plant();

    newPlant->spawn(Table);

    plants.push_back(newPlant);

    for (int i = 0; i < 3; i++) {
        Predator* newPredator = new Predator();
        newPredator->spawn(Table, ran, ran);
        predators.push_back(newPredator);
    }

    for (int i = 0; i < 5; i++){
        Animal* newAnimal = new Animal();
        newAnimal->spawn(Table, ran, ran);
        animals.push_back(newAnimal);
    }
    int chance = 0;
    int count = 0;
    int moveX, moveY, Ax, Ay;

    while (time != 0) {
        time--;
        printTable(Table);
            for (long unsigned int i = 0; i < animals.size(); i++){
            if ((animals[i]->isAlive())==0)
                animals.erase(animals.begin()+i);
        }
    if (predators.size()<3) {
        Predator* newPredator = new Predator();
        newPredator->spawn(Table, ran, ran);
        predators.push_back(newPredator);
    }

        for (long unsigned int i = 0; i < plants.size(); i++)
            plants[i]->action(Table);


        for (long unsigned int n = 0; n < animals.size(); n++){
            chance = (rand() % 4);
            if (chance == 0) {
                
                for (int i = 0; i < 2; i++) {
                    Ax = animals[n]->getX();
                    Ay = animals[n]->getY();
                    if (Table[(Ax-1)][Ay]=='#'){
                        Animal* newAnimal = new Animal((Ax-1), Ay);
                        newAnimal->spawn(Table, (Ax-1), Ay);
                        animals.push_back(newAnimal);
                        newAnimal->changeHunger(-0.4);
                    } else {
                    if (Table[Ax][(Ay-1)]=='#'){
                        Animal* newAnimal = new Animal(Ax, (Ay-1));
                        newAnimal->spawn(Table, Ax, (Ay-1));
                        animals.push_back(newAnimal);      
                        newAnimal->changeHunger(-0.4);                     
                        }
                    }
                }
            }
        }

        chance = rand() % 2;

        if (chance == 0) {
            Plant* newPlant = new Plant();
            newPlant->spawn(Table);
            plants.push_back(newPlant);
        }

        for (long unsigned int i = 0; i < predators.size(); i++)
            predators[i]->action(Table);

        for (long unsigned int i = 0; i < animals.size(); i++)
            animals[i]->action(Table);

        Sleep(1 * 1000);
        system("cls");
        for (long unsigned int i = 0; i < animals.size(); i++){
            if ((animals[i]->isAlive())==0)
                animals.erase(animals.begin()+i);
        }
    }

    return 0;
}