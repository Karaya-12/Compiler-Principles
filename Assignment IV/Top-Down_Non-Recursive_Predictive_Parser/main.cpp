#include "global.h"
#include "file_IO.h"
#include "proc_production.h"
#include "cal_first.h"
#include "cal_follow.h"
#include "generate_pt.h"
#include "non_recursive_parser.h"

#include <unistd.h>

/*                 Assumptions
 * 1. Epsilon Εε is represented by ‘#’
 * 2. Grammar is not left recursive
 * 3. Only upper case letters are Non-Terminals(NT), thus everything else is a Terminal(T)
 */

int raw_Line = 0;                        // Raw Production Line Counter
int input_Line = 0;                      // Input Text Counter
char inputText[MAX_ROWS][MAX_COLS];      // Input Text
char production[MAX_ROWS][MAX_COLS];     // Processed Productions
char raw_production[MAX_ROWS][MAX_COLS]; // Raw Productions
char FIRST_Result[MAX_ROWS][MAX_COLS];   // FIRST Set Result
char FOLLOW_Result[MAX_ROWS][MAX_COLS];  // FOLLOW Set Result
char parsing_table[MAX_ROWS][MAX_COLS];  // LL(1) Grammar Parsing Table
set<char>::iterator iter, iter_curr, iter_index;

int main(int argc, char **argv)
{
    /*Get Current Working Directory*/
    char cwd[256];
    getcwd(cwd, sizeof(cwd)); // Store In String "cwd"
    string result_dir = cwd, tsPath = cwd, firPath = cwd, flwPath = cwd, inputPath = cwd, outputPath = cwd;
    result_dir += "/IO_Text";
    cout << "Current Working Directory: " << cwd << "\n";
    cout << "Results Output Directory: " << result_dir << endl
         << endl;

    /*Generate Project Local Files Directory*/
    string test_set = "/IO_Text/Test_Set.txt";          // Test Set File Path
    string firSuffix = "/IO_Text/FIRST_Set.txt";        // FIRST Set File Path
    string flwSuffix = "/IO_Text/FOLLOW_Set.txt";       // FOLLOW Set File Path
    string inputSuffix = "/IO_Text/Input_Text.txt";     // Input Text File Path
    string outputSuffix = "/IO_Text/Output_Result.txt"; // Output Text File Path
    tsPath += test_set;
    firPath += firSuffix;
    flwPath += flwSuffix;
    inputPath += inputSuffix;
    outputPath += outputSuffix;

    /*Load Local CFG Grammar Into Production Arrays & Preprocess Given Productions*/
    load2array(tsPath, true); // CFG Grammar
    raw_Line--;
    production_preprocess(); // Split Given Productions

    /*Display Loaded CFG Grammar Productions*/
    display_productions();

    load2array(inputPath, false); // Custom Input Text
    input_Line--;

    /*Generate LL(1) Parsing Table*/
    pt_index();
    pt_initialize();

    /*Calculate The FIRST & FOLLOW Sets of Given Grammar Rules*/
    /*Part I --> Calculate FIRST Set & Generate Parsing Table Based On Each Production*/
    fun_first_set();
    output2file(firPath, FIRST_Result);

    /*Part II --> Calculate FOLLOW Set Based On Each Production*/
    fun_follow_set();
    output2file(flwPath, FOLLOW_Result);

    pt_follow(); // Generate Parsing Table Based On FOLLOW Set Result
    displayParsingTable();

    /*Parse User Input Text*/
    non_recursive_parse();
    output2file(outputPath);

    return 0;
}
