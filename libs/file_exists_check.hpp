#include <iostream>
#include <string>
#include <memory>
#include <filesystem>
#include <fstream>

/**
 * @brief file selected check
 * 
 * @param com_line type is int
 */
void file_selected_check(int com_line);

/**
 * @brief file exists check
 * 
 * @param file_name type is string
 * @param shrd  type is shared_ptr<ifstream>
 */
void file_exists_check(std::string file_name, std::shared_ptr<std::ifstream> &shrd);
