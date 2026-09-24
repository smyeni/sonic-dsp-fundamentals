//#include <print>
#include <vector>
#include "convolution.h"

std::vector<double>  sonic::convolve( const std::vector<double> &s, const std::vector<double> &h )
{
    std::size_t Lh = s.size() + h.size() - 1;
    std::vector<double>  y(Lh,0);

    for (size_t n=0; n<Lh; ++n)
    {
        //Pin down n and sum over all k
        for ( size_t k=0; k < h.size(); k++ )
        {
            if ( n < k ) //No -ve indexing
            {
                break;
            }

            if ( (n-k) < s.size() )
            {
                y[n] += h[k] * s[n-k]; 
            }
        }
    }

    return y;
}


std::vector<double> sonic::load_column(const std::string& path, int col_idx) 
{
    std::vector<double> out;
    std::ifstream f(path);
    if (!f) throw std::runtime_error("can't open " + path);

    std::string line;
    bool first = true;
    while (std::getline(f, line)) {
        if(line.empty()) continue;
        // skip header if any alpha in line
        if(first){
            first = false;
            bool has_alpha = false;
            for(char c: line) if(std::isalpha((unsigned char)c)) { has_alpha=true; break; }
            if(has_alpha) continue;
        }
        std::vector<std::string> cells;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) cells.push_back(cell);

        if(col_idx < (int)cells.size()){
            std::string v = cells[col_idx];
            // trim
            v.erase(0, v.find_first_not_of(" \t\r\n"));
            v.erase(v.find_last_not_of(" \t\r\n")+1);
            if(!v.empty()) out.push_back(std::stod(v));
        }
    }
    return out;
}
