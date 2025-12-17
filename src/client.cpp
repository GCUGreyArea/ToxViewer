#include "client.h"

#include <map>
#include <vector>
#include <iostream>
#include <fstream>

void serialise_all(std::vector<Tox> & toxlist,std::string file) {
    std::ofstream of;
    of.open(file);

    size_t size = toxlist.size();

    of.write((const char*) &size,sizeof(size));

    for(auto & tox : toxlist) {
        tox.serialise(of);
    }
}

void deserialise_all(std::vector<Tox> & toxlist, std::map<uint32_t, size_t> & toxmap, std::string file) {
    std::ifstream i;

    i.open(file,std::ios::in);

    // Get rid of the current toxes
    toxlist.clear();
    toxmap.clear();

    size_t size;

    i.read((char*)&size,sizeof(size));

    for(size_t idx=0; idx<size;idx++) {
        Tox t;
        t.deserialise(i);
        toxlist.push_back(t);
        toxmap[t.getId()] = idx;
    }
}

int main()
{
    Socket client("texttok.fzero.io", 4000);

    // constexpr size_t MAX_TOXES = 100;
    constexpr int MENU_GET_TOX = 1;
    constexpr int MENU_PRINT_TOXES = 2;
    constexpr int UPVOTE_TOX = 3;
    constexpr int DOWNVOTE_TOX = 4;
    constexpr int MENU_SAVE_TOX = 5;
    constexpr int MENU_LOAD_TOX = 6;
    constexpr int MENU_EXIT = 7;

    // Tox ID to index in vector
    std::map<uint32_t, size_t> toxes;
    std::vector<Tox> tox_list;

    bool run = true;
    while (run)
    {
        std::cout << "menu " << std::endl;
        std::cout << "1. Get next Tox" << std::endl;
        std::cout << "2. Print collected Toxes" << std::endl;
        std::cout << "3. Upvote Tox" << std::endl;
        std::cout << "4. Downvote Tox" << std::endl;
        std::cout << "5. Save toxes to file" << std::endl;
        std::cout << "6. Load toxes from file" << std::endl;
        std::cout << "7. Exit" << std::endl;
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case MENU_GET_TOX:
        {
            Tox new_tox;
            new_tox.get_latest(client);
            uint32_t id = new_tox.getId();
            if (toxes.find(id) == toxes.end())
            {
                toxes[id] = tox_list.size();
                tox_list.push_back(new_tox);
                std::cout << "Retrieved new tox ID: " << std::hex << id << std::dec << std::endl;
            }
            else
            {
                std::cout << "Tox ID: "  << std::hex << id << std::dec << " already retrieved." << std::endl;
            }
            break;
        }
        case MENU_PRINT_TOXES:
        {
            std::cout << "Collected Toxes: " << tox_list.size() << std::endl;
            for (auto &tox : tox_list)
            {
                tox.print();
            }
            break;
        }
        case UPVOTE_TOX:
        {
            std::cout << "Please end the ID of the Tox to upvote: ";
            uint32_t id;
            std::cin >>  std::hex >> id >> std::dec;
            auto tox = toxes.find(id);
            if (tox != toxes.end())
            {
                tox_list[tox->second].vote(client, true);
            }
            else
            {
                std::cout << "Tox ID: "  << std::hex << id << std::dec << " not found in collected toxes." << std::endl;
            }
            break;
        }
        case DOWNVOTE_TOX:
        {
            std::cout << "Please enter the ID of the Tox to downvote: ";
            uint32_t id;
            std::cin >>  std::hex >> id >> std::dec;
            auto tox = toxes.find(id);
            if (tox != toxes.end())
            {
                tox_list[tox->second].vote(client, false);
            }
            else
            {
                std::cout << "Tox ID: "  << std::hex << id << std::dec << " not found in collected toxes." << std::endl;
            }
            break;
        }
        case MENU_SAVE_TOX: {
            std::string fname;

            std::cout << "Please supply a file name: ";
            std::cin >> fname;
            serialise_all(tox_list,fname);
        }
        break;

        case MENU_LOAD_TOX: {
            std::string fname;

            std::cout << "Please supply a file name: ";
            std::cin >> fname;
            deserialise_all(tox_list,toxes,fname);
        }
        break;
        case MENU_EXIT:
        {
            run = false;
            break;
        }

        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    
    return 0;
}
