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
 * @brief function type alias
 * 
 */
enum class FUNCS: int {ADD, DIFF, MEM_ADD, MEM_DIFF, OUTPUT, INPUT, WHILE_BEGIN, WHILE_END};

/**
 * @brief file selected check
 * 
 * @param com_line type is int
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
 * @param file_name type is string
 * @param shrd  type is shared_ptr<ifstream>
 */
void file_exists_check(std::string file_name, sifstream &shrd) {
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
 * @brief read each line to str_arr. 
 * 
 * @param shrd type is shared_ptr<ifstream>&
 * @param str_arr type is vector<string>&
 */
void read_line(const sifstream &shrd, std::vector<std::string>& str_arr){
	while(!shrd->eof()){
		std::string tmp_str;
		std::getline(*shrd, tmp_str);
		str_arr.push_back(tmp_str);
	}
}

/**
 * @brief the function extracts +, -, <, >, [, ], , and .
 * 
 * @param f_str type is string&
 * @return std::string 
 */
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

std::shared_ptr<std::vector<FUNCS>> create_func_line(std::vector<std::string>& file_line){
	auto shrd_v = std::make_shared<std::vector<FUNCS>>();

		for(auto& str: file_line){
		auto str_extract = extract_str(str);

		for(auto c: str_extract){
			switch(c){
				case '+':
					shrd_v->push_back(FUNCS::ADD);
					break;
				case '-':
					shrd_v->push_back(FUNCS::DIFF);
					break;
				case '>':
					shrd_v->push_back(FUNCS::MEM_ADD);
					break;
				case '<':
					shrd_v->push_back(FUNCS::MEM_DIFF);
					break;
				case '.':
					shrd_v->push_back(FUNCS::OUTPUT);
					break;
				case ',':
					shrd_v->push_back(FUNCS::INPUT);
					break;
				case '[':
					shrd_v->push_back(FUNCS::WHILE_BEGIN);
					break;
				case ']':
					shrd_v->push_back(FUNCS::WHILE_END);
					break;
			}
		}
	}

	return shrd_v;
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

void file_exe(std::vector<std::string>& file_line){
	using func_v  = std::function<void(std::vector<uint8_t>&, size_t&)>;
	std::vector<uint8_t> mem_array(1);

	std::array<func_v, 8> func{
		[](std::vector<uint8_t>&v, size_t& pos) {++v.at(pos);},
		[](std::vector<uint8_t>&v, size_t& pos) {--v.at(pos);},
		[](std::vector<uint8_t>&v, size_t& pos) {
			++pos;
			if(pos == v.size()){
				v.push_back(0);
			}
		},
		[](std::vector<uint8_t>&v, size_t& pos) {
			long long checker = static_cast<long long>(pos);
			if(--checker < 0){
				std::cerr << "wrong mem place" << std::endl;
				exit(1);
			}
			--pos;
		},
		[](std::vector<uint8_t>&v, size_t& pos) {std::cout << v.at(pos);},
		[](std::vector<uint8_t>&v, size_t& pos) {std::cin >> v.at(pos);},
		[](std::vector<uint8_t>&v, size_t& pos) {},//dummy
		[](std::vector<uint8_t>&v, size_t& pos) {},//dummy
	};

	auto shrd_func_line = create_func_line(file_line);

	size_t mem_place = 0;
	size_t while_num = 0;
	std::vector<size_t> while_place(10);

	for(long unsigned i = 0; i < shrd_func_line->size(); i++) {
		if(shrd_func_line->at(i) == FUNCS::WHILE_BEGIN) {
			if(while_place.size() == while_num){
				while_place.push_back(i);
			}				
			else
				while_place.at(while_num-1) = i;
			++while_num;
		}

		if(shrd_func_line->at(i) == FUNCS::WHILE_END){
			if(mem_array.at(i))
				i = while_place.at(while_num-1);
			else 
				--while_num;
		}

		func[(int)shrd_func_line->at(i)](mem_array, mem_place);
	}

}

int main(int argc, char*argv[]) {
	sifstream target_file;
	std::vector<std::string> file_lines;

	/**
	 * @brief judge that the file is not invalid
	 */
	file_selected_check(argc);
	file_exists_check(argv[1], target_file);

	/**
	 * @brief read all file
	 */
	read_line(target_file, file_lines);

	/**
	 * @brief compile error check
	 */
	search_while_pair(file_lines);

	/**
	 * @brief execute source file
	 */
	file_exe(file_lines);

	return 0;
}
