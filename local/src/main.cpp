#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>
#include <stdexcept>
#include "CLI11.hpp"

std::string get_dir_path(bool test) {
    std::string env = std::getenv("HOME");
    if (!test) {
        return env + "/.pastebin";
    } else {
        return env + "/.testbin";
    }
}

void view_paste(std::string ID, bool test) {
    std::string full_path = get_dir_path(test) + "/" + ID;

    std::ifstream file(full_path);

    if (!file) {
        throw std::runtime_error("file not found: " + ID);
    }

    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << std::endl;    
    }

    file.close();
}

void save_paste(std::string ID, std::string text, bool test) { 
    std::string path = get_dir_path(test);
    std::string full_path = path + "/" + ID;

    if (!std::filesystem::exists(path)) {
        try {
            std::filesystem::create_directory(path);
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cout << e.what() << std::endl;
        }
    }

    if (std::filesystem::exists(full_path)) {
        throw std::runtime_error("paste with ID already exists: " + ID);
    }

    std::ofstream file(full_path);

    if (!file) {
        throw std::runtime_error("error creating paste with ID: " + ID);
    }

    file << text;
    file.close();

    std::cout << "saved " << ID << std::endl;
}

void del_paste(std::string ID, bool test) {

    std::string full_path = get_dir_path(test) + "/" + ID;

    if (std::filesystem::exists(full_path)) {
        if (std::filesystem::remove(full_path)) {
            std::cout << "deleted " << ID << std::endl;
        } else {
            throw std::runtime_error("error removing path with ID: " + ID);
        }
    } else {
        throw std::runtime_error("no paste exists with ID: " + ID);
    }
}

void list_IDs(bool test) {
    std::string path = get_dir_path(test);

    try {
        for (const auto& file : std::filesystem::directory_iterator(path)) {
            std::cout << file.path().filename().string() << std::endl;
        }
    } 
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << e.what() << std::endl; 
    }
}

int main(int argc, char** argv) {
    CLI::App app{"commandline pastebin"};

    bool test{false};
    app.add_flag("-t", test, "test mode");

    CLI::App* view = app.add_subcommand("view", "view selected paste");
    std::string view_ID;
    view->add_option("ID", view_ID, "ID to view")->required();

    CLI::App* paste = app.add_subcommand("paste", "paste given text");
    std::string paste_ID;
    paste->add_option("ID", paste_ID, "ID to paste")->required();
    std::string paste_text;

    CLI::App* del = app.add_subcommand("rm", "delete selected paste");
    std::string del_ID;
    del->add_option("ID", del_ID, "ID to delete")->required();

    CLI::App* list = app.add_subcommand("ls", "list all paste IDs");

    app.require_subcommand(1, 1);
    
    CLI11_PARSE(app, argc, argv);

    if (*view) {

        try {
            view_paste(view_ID, test);
        }

        catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }

    } else if (*paste) {

        std::string line;
        std::string text = "";

        while (std::getline(std::cin, line)) {
            text += line + "\n"; 
        }

        try {
            save_paste(paste_ID, text, test);
        }
        
        catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;     
        }

    } else if (*del) {

        try {
            del_paste(del_ID, test);
        }

        catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }

    } else if (*list) {
        list_IDs(test);
    } else {
        std::cout << "A subcommand is required" << std::endl;
    }
    return 1;
}
