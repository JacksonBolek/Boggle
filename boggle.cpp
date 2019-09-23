#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iomanip>

struct Tile {
    char letter;
    bool visited;
};

const int MAX = 6;
void create_boggle (std::ifstream& infile, Tile grid[MAX][MAX], int& max_rows, int& max_cols);
void reinitialize_grid (Tile grid[MAX][MAX], int max_rows, int max_cols);
void print_grid (const Tile grid[MAX][MAX], int max_rows, int max_cols);
void find_word (Tile grid[MAX][MAX], int max_rows, int max_cols,
                std::string word, bool& found, int row, int col, int i, std::string new_word);
void print_to_output_file (std::ofstream& outfile, std::string word, bool& found);
void find_first_letter (Tile grid[MAX][MAX], int max_rows, int max_cols, int& row, int& col, std::string word, bool& found);


int main(int argc, char * argv[])
{
    using namespace std;
    if (argc != 3) {
        cerr << " inputfile.dat outputfile.dat" << endl;
        exit (1);
    }

    ifstream infile (argv[1]);
    ofstream outfile (argv[2]);

    if(!infile) {
        cerr << "Cannot open inputfile " << argv[1] << endl;
        exit(1);
    }
    if(!outfile) {
        cerr << "Cannot open outputfile " << argv[2] << endl;
        exit(1);
    }

    Tile grid[MAX][MAX];
    int max_rows=5, max_cols=5;
    string word;
    bool found = false;

    create_boggle(infile, grid, max_rows, max_cols);
    print_grid(grid, max_rows, max_cols);

    infile >> word;


    while (!infile.eof()){

        int row=1, col=1;

        find_first_letter(grid, max_rows, max_cols, row, col, word,found);
        reinitialize_grid(grid, max_rows, max_cols);
        print_to_output_file(outfile, word, found);
        found = false;
        infile >> word;
    }


    infile.close();
    outfile.close();

    return 0;
}

void find_word (Tile grid[MAX][MAX], int max_rows, int max_cols, std::string word, bool& found, int row, int col, int i, std::string new_word) {

    using namespace std;

    if (grid[row][col].letter == '#')
        return;

    if (new_word == word) {
        found = true;
        return;
    }

    if (new_word.length() > word.length()) {
        found = false;
        return;
    }

    if (grid[row][col].visited == true || grid[row][col].letter != word.at(i))
        return;

    if (grid[row][col].letter == word.at(i) && grid[row][col].visited == false) {
        grid[row][col].visited = true;

        find_word (grid, max_rows, max_cols, word, found, row-1, col-1, i+1, new_word+word.at(i));
        if (!found)
            find_word (grid, max_rows, max_cols, word, found, row-1, col, i+1, new_word+word.at(i));
        if (!found)
            find_word (grid, max_rows, max_cols, word, found, row-1, col+1, i+1, new_word+word.at(i));
        if (!found)
            find_word (grid, max_rows, max_cols, word, found, row, col-1, i+1, new_word+word.at(i));
        if (!found)
            find_word (grid, max_rows, max_cols, word, found, row, col+1, i+1, new_word+word.at(i));
        if (!found)
            find_word (grid, max_rows, max_cols, word, found, row+1, col-1, i+1, new_word+word.at(i));
        if (!found)
            find_word (grid, max_rows, max_cols, word, found, row+1, col, i+1, new_word+word.at(i));
        if (!found)
            find_word (grid, max_rows, max_cols, word, found, row+1, col+1, i+1, new_word+word.at(i));
    }

}

void find_first_letter (Tile grid[MAX][MAX], int max_rows, int max_cols, int& row, int& col, std::string word, bool& found){

    std::string new_word="";
    int i=0;

    for (row= 1; row <= max_rows; row++){
        for (col = 1; col <= max_cols; col++){
            find_word(grid, max_rows, max_cols, word, found, row, col, i, new_word);
        }

    }
}

void reinitialize_grid (Tile grid[MAX][MAX], int max_rows, int max_cols) {
    using namespace std;
    int row_index, col_index;

    for (row_index = 1; row_index <= max_rows; row_index++) {
        grid[row_index][0].visited = false;
        for (col_index = 1; col_index <= max_cols; col_index++){
            grid[row_index][col_index].visited = false;
        }
         grid[row_index][max_cols+1].visited = false;
    }
    for (col_index = 0; col_index <= max_cols+1; col_index++){
        grid[0][col_index].visited = false;
        grid[max_rows+1][col_index].visited = false;
    }
}

void create_boggle (std::ifstream& infile, Tile grid[MAX][MAX], int& max_rows, int& max_cols) {

    using namespace std;
    int row_index, col_index;

    for (row_index = 1; row_index <= max_rows; row_index++) {

        // Set left border character
        grid[row_index][0].letter = '#';
        grid[row_index][0].visited = false;

        for (col_index = 1; col_index <= max_cols; col_index++){
            infile >> grid[row_index][col_index].letter;
            grid[row_index][col_index].visited = false;
        }
        grid[row_index][max_cols+1].letter = '#';
        grid[row_index][max_cols+1].visited = false;
    }

    for (col_index = 0; col_index <= max_cols+1; col_index++) {
        grid[0][col_index].letter = '#';
        grid[0][col_index].visited = false;

        grid[max_rows+1][col_index].letter = '#';
        grid[max_rows+1][col_index].visited = false;
    }
}

void print_grid (const Tile grid[MAX][MAX], int max_rows, int max_cols) {
    using namespace std;
    int row_index, col_index;

    cout << endl;

    for (row_index = 1; row_index <= max_rows; row_index++) {
        for (col_index = 1; col_index <= max_cols; col_index++)
            cout << grid[row_index][col_index].letter << " ";
        cout << endl;
    }
}

void print_to_output_file (std::ofstream& outfile, std::string word, bool& found) {
    if (found)
        outfile << word << " not found" << std::endl;
    else
        outfile << word << " found" << std::endl;
}
