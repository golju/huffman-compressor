#include "huffman.hpp"
#include <iostream>
#include <list>

bool compare (const Vertex * a, const Vertex * b)
{
    return (a -> priority) < (b -> priority);
}

Vertex::Vertex(int priority_, unsigned char symbol_,
               Vertex *left_child_ = nullptr, Vertex *right_child_ = nullptr):
               priority(priority_), symbol(symbol_),
               left_child(left_child_), right_child(right_child_){}

std::map < unsigned char, int > make_freq_count (std::ifstream & input_file,
                                                 int &size)
{
    std::map < unsigned char, int >  freq_count;
    size = 0;
    char input_char = 0;

    while (input_file.read(& input_char, sizeof(char)))
    {
        size++;
        freq_count[static_cast<unsigned char>(input_char)]++;

    }

    std::cout << size << std::endl;

    return freq_count;
}


Vertex * make_tree(std::map < unsigned char, int > freq_count)
{
    std::list < Vertex * > tree;
    for (std::map <unsigned char, int >::iterator i = freq_count.begin();
         i != freq_count.end(); ++i)
    {
        Vertex * temporary_pointer = new Vertex(i -> second, i -> first, nullptr, nullptr);
        tree.push_back(temporary_pointer);
    }

    while (tree.size() != 1)
    {
        tree.sort(compare);

        Vertex * Left_child = tree.front();
        tree.pop_front();

        Vertex * Right_child = tree.front();
        tree.pop_front();

        Vertex * Parent = new Vertex((Left_child -> priority)
                                     + (Right_child -> priority),
                                     '0', Left_child, Right_child);

        tree.push_back(Parent);
    }

    return tree.back();
}

void make_table (Vertex * tree, std::map < unsigned char,
                 std::string > & table, std::string sequence)
{
    if (tree -> left_child == nullptr && tree -> right_child == nullptr)
    {
        table[tree -> symbol] = sequence;
    } else
    {

        make_table(tree -> left_child, table, sequence + '0');
        make_table(tree -> right_child, table, sequence + '1');
    }
}


void recording_uint32_num_input_file (std::ofstream & out_file, int number)
{
    out_file << (uint8_t)((number & 4278190080) >> 24) <<
                (uint8_t)((number & 16711680) >> 16) <<
                (uint8_t)((number & 65280) >> 8) <<
                (uint8_t)(number & 255);
}


void make_Huffman_code (std::map < unsigned char, std::string > table,
                        std::ifstream & input_file,
                        std::ofstream & out_file,
                        int size_of_initial_file,
                        std::map < unsigned char, int >  freq_count,
                        bool is_v)
{
    recording_uint32_num_input_file(out_file, size_of_initial_file);
    recording_uint32_num_input_file(out_file, table.size());

    for (std::map < unsigned char, int >:: iterator i = freq_count.begin();
         i != freq_count.end();
         ++i)
    {
        out_file << (i -> first);
        recording_uint32_num_input_file(out_file, (i->second));

    }

    std::string conv_to_bin = "";
    int size_of_compressed_file = 0;
    char input_char = 0;
    uint8_t to_file = 0;
    int amount_of_bit = 0;
    int bits = 8;
    while (input_file.read(& input_char, sizeof(char)))
    {
        conv_to_bin = table[static_cast<unsigned char>(input_char)];

        for (size_t i = 0; i < conv_to_bin.length(); ++i)
        {
            to_file = to_file*2 + (uint8_t)(conv_to_bin[i] - 48);
            amount_of_bit++;
            if (amount_of_bit == bits)
            {
                out_file << to_file;
                amount_of_bit = 0;
                to_file = 0;
                size_of_compressed_file++;
            }

        }

    }

    if (amount_of_bit != 0)
    {
        while (amount_of_bit < bits)
        {
            to_file *= 2;
            amount_of_bit++;
        }
        out_file << to_file;
        size_of_compressed_file++;
    }

    int additional_information = sizeof(uint32_t);
    std::cout << size_of_compressed_file << std::endl;
    std::cout << additional_information + additional_information +
                 table.size() + table.size()*additional_information << std::endl;

    if (is_v)
    {
        for (std::map < unsigned char, std::string >:: iterator
             i = table.begin(); i != table.end(); ++i)
        {
            std::cout << (i -> second) << " " << (int)(i -> first) << std::endl;
        }
    }

}

int reading_uint32_num_from_file (std::ifstream & input_file)
{
    int number = 0;
    char input_char = 0;
    input_file.read(& input_char, sizeof(char));
    number += (int)static_cast<unsigned char>(input_char) << 24;

    input_file.read(& input_char, sizeof(char));
    number += (int)static_cast<unsigned char>(input_char) << 16;

    input_file.read(& input_char, sizeof(char));
    number += (int)static_cast<unsigned char>(input_char) << 8;

    input_file.read(& input_char, sizeof(char));
    number += (int)static_cast<unsigned char>(input_char);

    return number;
}

void delete_tree (Vertex * tree)
{
    if (tree -> left_child == nullptr && tree -> right_child == nullptr)
    {
        delete tree;
    } else
    {
        delete_tree(tree -> left_child);
        delete_tree(tree -> right_child);
        delete tree;
    }
}

void make_initial_code (std::ifstream & input_file, std::ofstream & out_file, bool is_v)
{    
    input_file.seekg(0, std::ios::end);
    int file_size = input_file.tellg();
    if(file_size == 0){
        std::cout << "0\n0\n0\n";
        return;
    }
    input_file.clear();
    input_file.seekg(0, std::ios_base::beg);

    int size_of_initial_file = reading_uint32_num_from_file(input_file);
    int cardinality_of_alphabet = reading_uint32_num_from_file(input_file);

    std::map < unsigned char, int > freq_count;
    for (size_t i = 0; i < cardinality_of_alphabet; ++i)
    {
        char symb = 0;
        input_file.read(& symb, sizeof(char));

        int freq_of_num = reading_uint32_num_from_file(input_file);

        freq_count[symb] = freq_of_num;
    }

    int pos = input_file.tellg();
    Vertex * tree_for_unpaking = make_tree(freq_count);

    input_file.seekg(pos);

    std::map < unsigned char, std::string > table_for_unpaking;
    std::string sequence_for_make_table;
    freq_count.size() <= 1 ? sequence_for_make_table = "0" : sequence_for_make_table = "";
    make_table(tree_for_unpaking, table_for_unpaking, sequence_for_make_table);

    char unpacking_char = 0;
    input_file.read(& unpacking_char, sizeof(char));
    int size_of_decompressed_file = 1;
    int current_size = 0;
    Vertex * temp_tree = tree_for_unpaking;
    int current_bit = 0;
    char current_char = 0;

    while (current_size < size_of_initial_file)
    {
        while (temp_tree->left_child != nullptr &&
               temp_tree->right_child != nullptr){
            if (current_bit == 8)
            {
                input_file.read(& unpacking_char, sizeof(char));
                size_of_decompressed_file++;
                current_bit = 0;
            }

            current_char = ((int)static_cast<unsigned char>(unpacking_char) &
                            (1 << (7 - current_bit)));

            if (current_char == 0) {
                temp_tree = temp_tree->left_child;
            }
            else
            {
                temp_tree = temp_tree->right_child;
            }
            current_bit++;
        }

        if (freq_count.size() == 1)
        {
            if (current_bit == 8)
            {
                input_file.read(& unpacking_char, sizeof(char));
                size_of_decompressed_file++;
                current_bit = 0;
            }

            current_char = ((int)static_cast<unsigned char>(unpacking_char) &
                            (1 << (7 - current_bit)));
            current_bit++;
        }
        out_file << temp_tree->symbol;
        current_size++;
        temp_tree = tree_for_unpaking;
    }
    std::cout << size_of_decompressed_file << std::endl;
    std::cout << size_of_initial_file << std::endl;

    int additional_information = sizeof(uint32_t);

    std::cout << additional_information + additional_information +
                 table_for_unpaking.size() +
                 table_for_unpaking.size()*additional_information << std::endl;

    if (is_v)
    {
        for (std::map < unsigned char, std::string >:: iterator
             i = table_for_unpaking.begin(); i != table_for_unpaking.end(); ++i)
        {
            std::cout << (i -> second)<< " " << (int)(i -> first) << std::endl;
        }
    }
    delete_tree(tree_for_unpaking);
}
