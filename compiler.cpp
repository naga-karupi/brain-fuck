#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>

/**
 * @brief arias
 * 
 */
namespace file = std::filesystem;
using start_while_pair = std::pair<size_t, size_t>;
using end_while_pair   = start_while_pair;
using sifstream = std::shared_ptr<std::ifstream>;

/**
 * @brief file selected check
 * 
 * @param com_line 
 */
void file_selected_check(int com_line){
	if((com_line-1)<=0) {
		std::cerr << "no file selected" << std::endl;
		exit(1);
	}
}

/**
 * @brief file exists check
 * 
 * @param file_name as string
 * @param shrd  as shared_ptr<ifstream>
 */
void file_exists_check(std::string file_name, sifstream shrd) {
	file::path path(file_name);

	if(!file::exists(path)) {
		std::cerr << "the file do not exist" << std::endl;
		exit(1);
	}

	shrd = std::make_shared<std::ifstream>(file_name, std::ios_base::in);

	if(!(*shrd)) {
		std::cerr << "file open error" << std::endl;
		exit(1);
	}
}


/**
 * @brief read to single string
 * 
 * @param shrd as shared_ptr<ifstream>&
 * @param f_str as string
 */
void full_read_line(const sifstream shrd, std::string& f_str){
	while(!shrd->eof()){
		std::string tmp_string;
		std::getline(*shrd, tmp_string);
		f_str += tmp_string;
	}
}

/**
 * @brief read each line to str_arr. 
 * 
 * @param shrd as shared_ptr<ifstream>&
 * @param str_arr as vector<string>&
 */
void read_line(const sifstream &shrd, std::vector<std::string>& str_arr){
	while(!shrd->eof()){
		std::string tmp_str;
		std::getline(*shrd, tmp_str);
		str_arr.push_back(tmp_str);
	}
}

std::string extract_str(std::string& f_str){
	static std::string check_str{'+', '-', '<', '>', '.', ',', '[', ']'};
	std::string bf_str;

	for(auto c: f_str){
		bool is_literal = false;

		for(auto check_c: check_str){
			is_literal |= (c == check_c);
			if(is_literal) break;
		}

		if(is_literal) bf_str.push_back(c);
	}

	return bf_str;
}

void search_while_pair(std::vector<std::string> str_v){
	uint64_t while_start_counter = 0, while_end_counter = 0;
	for(auto str: str_v){
		for(auto c: str){
			if(c == '[') ++while_start_counter;
			if(c == ']') ++while_end_counter;

			if(while_start_counter < while_end_counter) {
				std::cerr << "not enough while start '['" << std::endl;
				exit(1);
			}
		}
	}

	if(while_start_counter != while_end_counter){
		std::cerr << "not enough while end ']'" << std::endl;
		exit(1);
	}
}

void file_exe(const sifstream shrd){
	std::string f_str, bf_str;
	std::vector<std::string> file_place;
	using func_v  = std::function<void(std::vector<char>&, size_t&)>;
	
	enum {ADD, MIN, MEM_ADD, MEM_MIN, OUTPUT, INPUT};

	size_t count_line;

	std::vector<char>mem_str{0};
	func_v add     = [](std::vector<char>&v,size_t& pos) {++v.at(pos);};
	func_v min     = [](std::vector<char>&v,size_t& pos) {--v.at(pos);};
	func_v mem_add = [](std::vector<char>&v,size_t& pos) {++pos;};
	func_v mem_min = [](std::vector<char>&v,size_t& pos) {--pos;};
	func_v output  = [](std::vector<char>&v,size_t& pos) {std::cout << v.at(pos);};
	func_v input   = [](std::vector<char>&v,size_t& pos) {std::cin >> v.at(pos);};

	read_line(shrd, file_place);

//TODO
}

int main(int argc, char*argv[]) {
	sifstream target_file;
	std::string f_str, bf_str;
	std::vector<std::string> file_place;

	using func_v  = std::function<void(std::vector<char>&, size_t&)>;
	
	enum {ADD, MIN, MEM_ADD, MEM_MIN, OUTPUT, INPUT};

	size_t count_line;

	std::vector<char>mem_str{0};

	std::array<func_v, 6> func{
		[](std::vector<char>&v,size_t& pos) {++v.at(pos);},
		[](std::vector<char>&v,size_t& pos) {--v.at(pos);},
		[](std::vector<char>&v,size_t& pos) {++pos;},
		[](std::vector<char>&v,size_t& pos) {--pos;},
		[](std::vector<char>&v,size_t& pos) {std::cout << v.at(pos);},
		[](std::vector<char>&v,size_t& pos) {std::cin >> v.at(pos);},
	};

	file_selected_check(argc);
	file_exists_check(argv[1], target_file);
	read_line(target_file, file_place);
	file_exe(target_file);
}

void read_file(const sifstream& target_file, std::string& file_e) {
	std::string check_str{'+', '-', '<', '>', '.', ',', '[', ']'};
	std::string all_str;
	while(!target_file->eof()) {
		bool is_literal = false;
		char tmp_char;
		tmp_char =target_file->get();
		
		for(auto c: check_str){
			is_literal |= (c == tmp_char);
			if(is_literal) {
				file_e += tmp_char;
				break;
			}
		}
	}
}

template<class f, class ...f_other>
static inline void func_caller(std::vector<char>&v,size_t& pos,  f func, f_other ...fo){
	func();
}

void analysis_code(std::string& str){
	size_t pos = 0;

	using v_char  = std::vector<char>;
	using func_v  = std::function<void(v_char&, size_t&)>;
//	using while_t = std::function<void(std::vector<char>&, size_t&, std::tuple<func_v, while_t>)>;

	std::vector<char>mem_str{0};
	func_v add     = [&](v_char&v,size_t& pos) {++v.at(pos);};
	func_v min     = [&](v_char&v,size_t& pos) {--v.at(pos);};
	func_v mem_add = [&](v_char&v,size_t& pos) {++pos;};
	func_v mem_min = [&](v_char&v,size_t& pos) {--pos;};
	func_v output  = [=](v_char&v,size_t& pos) {std::cout << v.at(pos);};
	func_v input   = [&](v_char&v,size_t& pos) {std::cin >> v.at(pos);};
/*
	while_t bf_while = [&](std::vector<char>& buf, size_t& p, std::tuple<func_v>funcs) {
		while(buf.at(p)){
			for(int i = 0, )
		}
	} ;
	*/

}
