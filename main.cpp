#include "huffman.hpp"
#include <iostream>
int main (int argc, char **argv)
{
    bool verbose = (std::string(argv[1]) == "-v");
    bool compress = (std::string(argv[1]) == "-c" || std::string(argv[2]) == "-c");

    if (compress)
    {
        std::ifstream input_file(argv[argc - 2], std::ios_base::binary);
        std::ofstream output_file(argv[argc - 1], std::ios_base::binary);
        int size_of_file = 0;
        std::map < unsigned char, int >  freq_count = make_freq_count(input_file, size_of_file);
        if (freq_count.size() == 0)
        {
            std::cout << "0\n0\n";
            return 0;
        }
        std::map < unsigned char, std::string > mytable;
        std::string sequence_for_make_table;
        freq_count.size() <= 1 ? sequence_for_make_table = "0" : sequence_for_make_table = "";
        input_file.clear();
        input_file.seekg(0, std::ios::beg);
        Vertex * mytree = make_tree(freq_count);
        make_table(mytree, mytable, sequence_for_make_table);
        make_Huffman_code(mytable, input_file, output_file,
                          size_of_file, freq_count, verbose);
        delete_tree(mytree);
    }
    else
    {
        std::ifstream input_file(argv[argc - 2], std::ios_base::binary);
        std::ofstream output_file(argv[argc - 1], std::ios_base::binary);
        make_initial_code(input_file, output_file, verbose);
    }

    return 0;
}
