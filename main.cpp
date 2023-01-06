#include <iostream>
#include "json.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
using json = nlohmann::json;

json map;
json current_room;
std::vector<std::string> objects; /*this vector keeps the current objects possessed by the player*/
int health{100}, number{0};
int get_room(std::string str) /*you give the room name it returns its index if it is there*/
{
    int is_count{0};
    int count{0};
    for (auto it = map["rooms"].begin(); it != map["rooms"].end(); it++)
    {
        if ((*it)["id"] == str)
        {

            is_count = 1;
            return count;
        }

        count++;
    }
    return -1;
}

void room_description(int room_number);            /* handles all the printable stuff */
std::string check_enemy(int room_number, char ch); /* checks and returns and prints if there is an enemy in the room */
void follow_command(std::string command);
void kill(std::string name);
void move(std::string direction);
void pick(std::string object);
int can_be_kiled(json enemy);
int number_of_enemies();
void list();
void attack_by_enemy();
int room_objects();
void relocate_enemy(std::string name, std::string room);
void eject_used_objects(json enemy);
void help();
int dead_end();
void status();

int main(int argc, char *argv[])
{
    /*setting up my input stream */
    std::ifstream j(argv[1]);
    j >> map;

    /*initializing my variables*/
    int room_number{0};          /*alive becomes 0 when the player dies.*/
    json player = map["player"]; /*this object contains the details of the player*/
    room_number = get_room(player["initialroom"]);
    current_room = map["rooms"][room_number];
    std::string command;
    json check_room = map["rooms"][get_room(map["enemies"][0]["initialroom"])];
    int alive_enemies = map["enemies"].size();

    while (health > 0 && alive_enemies > 0)
    {
        if (check_room["id"] != current_room["id"])
        {
            room_description(get_room(current_room["id"]));
            check_room = current_room;
            if (dead_end())
            {
                break;
            }
        }

        getline(std::cin, command);
        follow_command(command);
        alive_enemies = number_of_enemies();
        command.clear();
    }
    if (alive_enemies == 0 && health > 0)
    {
        std::cout << "\n\n\n\nLucky guy! You made it out alive!\n";
    }
    else if (alive_enemies != 0)
    {
        std::cout << "\n\n\n\nOOPS!!!! BUSTED! Try again.......\n";
    }
    return 0;
}

int number_of_enemies() /*returns the number of alive enemies*/
{
    int count{0};
    for (auto it = map["enemies"].begin(); it != map["enemies"].end(); it++)
    {
        if ((*it)["id"] != "this enemy has died")
        {
            count++;
        }
    }

    return count;
}

int room_objects() /*returns the number of objects in the room*/
{
    int count{0};
    for (auto it = current_room["obj"].begin(); it != current_room["obj"].end(); it++)
    {
        if ((*it) != "")
        {
            count++;
        }
    }

    return count;
}

void room_description(int room_number) /*prints the room deacription*/
{
    std::cout << "\n"
              << map["rooms"][room_number]["desc"] << "\n";
    if (map["rooms"][room_number]["obj"].size() > 0 && room_objects() > 0)
    {
        std::cout << "\nThis rooms contains the following object/objects\n";
        for (int i{0}; i < map["rooms"][room_number]["obj"].size(); i++)
        {
            std::cout << i + 1 << ". " << map["rooms"][room_number]["obj"][i] << "  ";
        }
    }
    else
    {
        std::cout << "\nThis room contains no pickable objects\n";
    }
    check_enemy(room_number, 'y');

    std::cout << "\n"
              << ">>  ";
}

std::string check_enemy(int room_number, char ch) /*checks and prints if there is an enemy in the room. returns its name*/
{
    std::string name;
    for (auto it = map["enemies"].begin(); it != map["enemies"].end(); it++)
    {
        if (get_room((*it)["initialroom"]) == room_number)
        {
            name = (*it)["id"];
            if (ch == 'y' && name != "this enemy has died")
                std::cout << "\nThere is " << (*it)["id"] << " in this room\n";
        }
    }

    return name;
}

void pick(std::string object) /*picks up an object*/
{
    int i{0};
    int count{0};
    for (auto it = current_room["obj"].begin(); it != current_room["obj"].end(); it++)
    {
        if ((*it) == object)
        {
            count++;
            objects.push_back(object);
            (*it) = "";
            map["rooms"][get_room(current_room["id"])]["obj"][i] = "";
            std::cout << "\nYou have picked up " << objects.at(objects.size() - 1) << " successfully\n>>  ";
        }
        i++;
    }
    if (count == 0)
    {
        std::cout << "\nCould not pick object(s)! You can only pick one object at a time. Also enter the name correctly.\n>>  ";
    }
    return;
}

void move(std::string direction) /*updates the current room when the go command is used*/
{

    if (current_room["e"] != "" && direction == "east")
    {
        if ((check_enemy(get_room(current_room["id"]), 'n')).length() > 0 && check_enemy(get_room(current_room["id"]), 'n') != "this enemy has died")
        {
            if (rand() % 2)
                attack_by_enemy();
        }
        current_room = map["rooms"][get_room(current_room["e"])];
    }
    else if (current_room["s"] != "" && direction == "south")
    {
        if ((check_enemy(get_room(current_room["id"]), 'n')).length() > 0 && check_enemy(get_room(current_room["id"]), 'n') != "this enemy has died")
        {
            if (rand() % 2)
                attack_by_enemy();
        }
        current_room = map["rooms"][get_room(current_room["s"])];
    }
    else if (current_room["n"] != "" && direction == "north")
    {
        if ((check_enemy(get_room(current_room["id"]), 'n')).length() > 0 && check_enemy(get_room(current_room["id"]), 'n') != "this enemy has died")
        {
            if (rand() % 2)
                attack_by_enemy();
        }
        current_room = map["rooms"][get_room(current_room["n"])];
    }
    else if (current_room["w"] != "" && direction == "west")
    {
        if ((check_enemy(get_room(current_room["id"]), 'n')).length() > 0 && check_enemy(get_room(current_room["id"]), 'n') != "this enemy has died")
        {
            if (rand() % 2)
                attack_by_enemy();
        }
        current_room = map["rooms"][get_room(current_room["w"])];
    }
    else
        std::cout << "\nCan't move in that direction\n>>  ";
    return;
}

void follow_command(std::string str) /*does what you ask it to do. calls the pick, kill, help... etc functions for you*/
{
    std::string command;
    std::string instruction;
    int i{0};
    std::string character;
    for (i = 0; str[i] != ' '; i++)
    {
        character = str[i];
        command += character;
        character.clear();
        if (command == "list" || command == "help" || command == "status")
        {
            break;
        }
    }
    i++;

    for (; i < str.size(); i++)
    {
        character = str[i];
        instruction += character;
        character.clear();
    }

    if (command == "kill")
    {
        kill(instruction);
    }
    else if (command == "go")
    {
        move(instruction);
    }
    else if (command == "pick")
    {
        pick(instruction);
    }
    else if (command == "list")
    {
        list();
    }
    else if (command == "help")
    {
        help();
    }
    else if (command == "status")
    {
        status();
    }
    else
    {
        std::cout << "\nInvalid input. Please enter your instruction again!\n>>  ";
    }
}

int can_be_killed(json enemy) /*checks if the enemy can be killed with your current possessions*/
{
    int count{0};
    for (auto it = objects.begin(); it != objects.end(); it++)
    {
        for (auto jt = enemy["killedby"].begin(); jt != enemy["killedby"].end(); jt++)
        {
            if ((*it) == (*jt))
            {
                count++;
            }
        }
    }
    if (count == enemy["killedby"].size())
    {
        return 1;
    }
    else
    {
        std::cout << "You don't have the required weapons to kill the enemy. Run!!!\n";
        return 0;
    }
}

void attack_by_enemy() /*reduces your health when you are attacked*/
{
    health -= 75;
    if (health > 0)
        std::cout << "\nYou were attacked by the enemy! Your health reduced by 75 hit points...\n";
    return;
}

void relocate_enemy(std::string name, std::string room) /*relocates the enemy to a different room. the enemy might stay in the same room as well, you never know*/
{
    int index = rand() % map["enemies"].size();
    for (auto it = map["enemies"].begin(); it != map["enemies"].end(); it++)
    {
        if ((*it)["id"] == name && (*it)["initialroom"] == room)
        {
            (*it)["initialroom"] = map["enemies"][index]["initialroom"];
            std::cout << "Enemy relocated...\n>>  ";
        }
    }
    return;
}

void eject_used_objects(json enemy) /* removes the weapons you used to kill an enemy*/
{
    for (auto it = enemy["killedby"].begin(); it != enemy["killedby"].end(); it++)
    {
        for (auto jt = objects.begin(); jt != objects.end(); jt++)
        {
            if ((*jt) == (*it))
            {
                (*jt) = "";
            }
        }
    }
    return;
}

void kill(std::string name) /* kills the enemy if you have the required objects*/
{
    int is_killed = 0, count = 0;
    std::string test;
    for (auto it = map["enemies"].begin(); it != map["enemies"].end(); it++)
    {
        if ((*it)["id"] == name && check_enemy(get_room(current_room["id"]), 'n') == name)
        {
            if (can_be_killed((*it)))
            {
                eject_used_objects((*it));
                (*it)["id"] = "this enemy has died";
                std::cout << "\nYou killed " << name << "\n>>  ";
                is_killed = 1;
                number++;
                return;
            }
            else
            {
                attack_by_enemy();
                relocate_enemy(name, current_room["id"]);
                return;
            }
        }
    }
    if (is_killed == 0 || test == check_enemy(current_room["id"], 'n'))
    {
        std::cout << "Maybe you have mis-spelt the enemy name or maybe the enemy is'nt there...\n>>  ";
    }
    return;
}

void list() /* prints out your current objects thet you possess*/
{
    int i{0};
    if (objects.size() == 0)
    {
        std::cout << "\nCurrently, you have nothing!! Use the 'pick' command to pick up objects...\n>>  ";
    }
    else
    {
        std::cout << "Currently, you have the following object(s)...\n";
        for (i = 0; i < objects.size(); i++)
        {
            std::cout << "* " << objects[i] << "\n";
        }
        std::cout << ">>  ";
    }

    return;
}

void help() /* prints the instructions that you can use*/
{
    std::cout << "Use 'pick' to pick up objects\nUse 'kill' command to kill enemies\nUse 'go' to move to another room\nUse 'list' command to list out your items\n>>  ";
    return;
}

int dead_end() /*checks if there is no doors in the room*/
{
    if (get_room(current_room["n"]) >= 0 || get_room(current_room["w"]) >= 0 || get_room(current_room["e"]) >= 0 || get_room(current_room["s"]) >= 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
    return 0;
}

void status() /*shows your status*/
{
    std::cout << "\nYou are at " << current_room["id"] << "\n";
    std::cout << "\nYour health : " << health << "\n";
    std::cout << "\nYou have killed " << number << " enemy(s)\n";
    list();
    return;
}