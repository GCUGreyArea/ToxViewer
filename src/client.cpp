#include "client.h"

#include <map>
#include <vector>
#include <iostream>

int main()
{
    Socket client("texttok.fzero.io", 4000);

    constexpr size_t MAX_TOXES = 100;
    constexpr int MENU_GET_TOX = 1;
    constexpr int MENU_PRINT_TOXES = 2;
    constexpr int UPVOTE_TOX = 3;
    constexpr int DOWNVOTE_TOX = 4;
    // constexpr int GET_RATING = 5;
    constexpr int MENU_EXIT = 5;

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
        std::cout << "5. Exit" << std::endl;
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
        // case GET_RATING:
        // {
        //     std::cout << "Please enter the ID of the Tox to get rating: ";
        //     uint32_t id;
        //     std::cin >>  std::hex >> id >> std::dec;
        //     auto tox = toxes.find(id);
        //     if (tox != toxes.end())
        //     {
        //         tox_list[tox->second].get_rating(client);
        //         std::cout << "Tox ID: " << std::hex << id << std::dec << " has rating: " << tox_list[tox->second].getRating() << std::endl;
        //     }
        //     else
        //     {
        //         std::cout << "Tox ID: "  << std::hex << id << std::dec << " not found in collected toxes." << std::endl;
        //     }
        //     break;
        // }
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
