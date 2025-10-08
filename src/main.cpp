#include <iostream>
#include <string>
#include "WareHouse.h"

// Define the global backup pointer used by Backup/Restore actions.
WareHouse* backup = nullptr;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: warehouse <config_file_path>\n";
        return 1;
    }

    const std::string configurationFile = argv[1];

    // Create and run the warehouse simulation.
    WareHouse wareHouse(configurationFile);
    wareHouse.start();

    // Clean up global backup (if created during runtime).
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }

    return 0;
}
