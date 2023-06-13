#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_set>

static const std::string INPUT_FILE = "results.txt";
static const int ITER = 1000;

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (std::getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

int main()
{
    const std::string OUTPUT_FILE = "out.csv";
    std::ofstream ofs(OUTPUT_FILE);

    std::string line;
    std::ifstream file_stream (INPUT_FILE);
    std::string prev_mtx="";
    std::unordered_set<std::string> mtx_files;

    if (file_stream.is_open())
    {
        int total_count = 0;
        int success_count = 0;
        long long prev_nnz_count = 0;
        std::vector<double> improvs;
        while ( std::getline (file_stream, line) )
        {
            // std::cout << line << '\n';
            const auto& tokens = split(line, ',');
            const auto mtx = split(tokens[0], '=').back();
            mtx_files.insert(prev_mtx);

            const auto improv = std::stod(split(tokens[1], '=').back());
            const auto iter = std::stoi(split(tokens[4], '=').back());
            // std::cout << "mtx: " << mtx << std::endl;
            // std::cout << "iter: " << iter << std::endl;
            
            if(mtx == prev_mtx) {
                if(iter == ITER) {
                    improvs.push_back(improv);

                    if(improv > 0) {
                        ++success_count;
                    }

                    ++total_count;
                }
            } else {
                if(success_count/static_cast<double>(total_count) >= 0.7) {
                    std::stringstream ss;
                    for(const auto improv : improvs) {
                        ss << improv << ',';  
                    }
                    std::string ss_str = ss.str();
                    ss_str.pop_back();

                    ofs << prev_nnz_count << ',' << prev_mtx << ',' << ss_str << '\n'; 
                }

                total_count = 0;
                success_count = 0;
                improvs = {improv};

                if(improv > 0) {
                    ++success_count;
                }
                ++total_count;
            }

            prev_mtx = mtx;
            prev_nnz_count = std::stoll(split(tokens[8], '=').back());
        }
        file_stream.close();
    } else {
        std::cerr << "Unable to open file";
    }
    
    std::cout << "total mtx file count scanned: " << mtx_files.size() << std::endl;
}
