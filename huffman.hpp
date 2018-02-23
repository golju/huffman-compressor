#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <fstream>
#include <map>
#include <string>


struct Vertex
{
    Vertex(int priority_, unsigned char symbol_, Vertex * left_child_, Vertex * right_child_);
    int priority;
    unsigned char symbol;
    Vertex * left_child;
    Vertex * right_child;

};

bool compare (const Vertex * a, const Vertex * b);
std::map < unsigned char, int > make_freq_count (std::ifstream & input_file,
                                                 int &size);
Vertex * make_tree(std::map < unsigned char, int > freq_count);
void make_table (Vertex * tree, std::map < unsigned char,
                 std::string > & table, std::string sequence);
void make_table (Vertex * tree, std::map < unsigned char,
                 std::string > & table, std::string sequence);
void recording_uint32_num_input_file (std::ofstream & out_file, int number);
void make_Huffman_code (std::map < unsigned char, std::string > table,
                        std::ifstream & input_file,
                        std::ofstream & out_file,
                        int size_of_initial_file,
                        std::map < unsigned char, int >  freq_count,
                        bool is_v);
int reading_uint32_num_from_file (std::ifstream & input_file);
void delete_tree (Vertex * tree);
void make_initial_code (std::ifstream & input_file, std::ofstream & out_file, bool is_v);
#endif
