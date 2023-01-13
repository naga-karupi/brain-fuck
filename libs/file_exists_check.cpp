#include "file_exists_check.hpp"

void file_selected_check(int com_line){
	if((com_line-1)<=0) {
		std::cerr << "no file selected" << std::endl;
		exit(1);
	}
}


void file_exists_check(std::string file_name, std::shared_ptr<std::ifstream> &shrd) {
	std::filesystem::path path(file_name);

	if(!std::filesystem::exists(path)) {
		std::cerr << "the file do not exist" << std::endl;
		exit(1);
	}

	shrd = std::make_shared<std::ifstream>(file_name, std::ios_base::in);

	if(!(*shrd)) {
		std::cerr << "file open error" << std::endl;
		exit(1);
	}
}