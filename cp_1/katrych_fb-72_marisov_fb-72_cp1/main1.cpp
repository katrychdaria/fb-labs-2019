#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <fstream>
#include <bitset>

//1. в качестве источников текста я использовала книги спираченые на royallib.ru, но я никакую из них не читала (кроме 
//"Бедные люди" Льва Толстого ). 
//2. из наполнения : пару книг Толстого + все подряд книги из раздела любовные романы
//любовные романы в основном это перевод с английского, поэтому значения статистики могут быть слегка (незначительно) испорчены
//иностранными сочетаниями букв, не свойственными русскому языку
//3. кодировка - utf8

std::vector<uint32_t> do_clean_text( std::string_view pathin, std::string_view pathout, const size_t& num_of_passages, bool leave_spaces,
                    const std::map<std::string, uint32_t>& alph)
{
    std::ofstream out_stream(pathout.data());
    std::string holder_pathin = pathin.data();

    uint32_t to_lower_ut{};
    std::string str{};
    std::string letter{};
    std::vector<uint32_t> text_vec{};

    for(size_t i = 1; i < num_of_passages + 1; ++i)
    {
       holder_pathin =  pathin.data() + std::to_string(i) + ".txt";
       std::cout << holder_pathin << std::endl;
       std::ifstream in_stream(holder_pathin);


       while(getline(in_stream, str))
       {
        if(str.length() % 2 == 1)
        {
            str += " ";
        }
        for(size_t i = 0; i < str.length(); ++i)//str.length() - 1 ???????
        {
            letter  = str[i];
            if( (i + 1) == str.length() )
            {
                break;
            }
            letter += str[i + 1];

            if(leave_spaces == true)
            {
               if(str[i] == ' ')
               {    
                    out_stream << " ";
                    text_vec.push_back(31);
                    continue;
               }
            }


            //experimental feature
            /*to_lower_ut = (uint32_t)str[i] & 0xFF;
                std::cout << std::bitset<32>(to_lower_ut) << std::endl;
            to_lower_ut =  (to_lower_ut << 8) | ((uint32_t)str[i+1] & 0xFF);
                std::cout << std::bitset<32>(to_lower_ut) << std::endl;
            if(((to_lower_ut & 0xFF) <= 0xaf) && ((to_lower_ut & 0xFF) >= 0x90) )
            {
                std::cout << std::bitset<32>(to_lower_ut) << std::endl;
                to_lower_ut += 32;
                letter[ 1 ] = to_lower_ut & 0xFF;
                letter[ 0 ] = (to_lower_ut >> 8) & 0xFF;
            }

            std::cout << letter[0] << letter[1] << " : " << std::bitset<32>(to_lower_ut) << std::endl;
            return text_vec;
            */
            //auto it = std::map::find(alph.begin(), alph.end(), letter);
            auto it = alph.find(letter);
            if(it == alph.end())
            {
                continue;
            }

            text_vec.push_back(it->second);
            out_stream << it->first;
            if(text_vec.size() %  100 == 0)
            {
                out_stream << '\n';

            }
           
            //these 2 ifs corrects missmatch between default alphabet with 'ъ' and alphabet used in this lab
            /*
            if(temp_let == 28)
            {
                temp_let = 26;
            }
            if(temp_let > 27) 
            {
                --temp_let;
            }
            */
            
         }
       }


    /*
    if(text_vec.size() % 2 == 1)
    {
        text_vec.push_back(26);
    }
    */



       in_stream.close();
    }



    out_stream.close();    
    return text_vec;
}

std::vector<uint32_t> let_freq(const std::vector<uint32_t>& t_v)
{
    std::vector<uint32_t> let_fr(32, 0);

    for(auto it = t_v.begin(); it != t_v.end(); ++it)
    {
        ++let_fr[*it];
    }


return let_fr;
}

std::vector<std::vector<uint32_t>> bi_freq_peresech(const std::vector<uint32_t>& t_v)
{
    std::vector<std::vector<uint32_t>> bi_fr_per(32, std::vector<uint32_t>(32));
    

    for(size_t i = 1; i < t_v.size(); ++i)
    {
        ++bi_fr_per[t_v[i - 1]][ t_v[i] ];

    }

    return bi_fr_per;
}

std::vector<std::vector<uint32_t>> bi_freq(const std::vector<uint32_t>& t_v)
{
    std::vector<std::vector<uint32_t>> bi_fr (32, std::vector<uint32_t>(32) );

    for(size_t i = 1; i < t_v.size(); i+=2)
    {
        ++bi_fr[t_v[i - 1]][t_v[ i ] ];

    }



    return bi_fr;
}

double entr_single(const std::vector<uint32_t>& let_fr, double col)
{
    double entr = 0.0;

    for(auto it = let_fr.begin(); it != let_fr.end(); ++it)
    {
        if(*it != 0)
        {
            entr += - ( ((double)*it / col ) * (log2((double)*it / col))); 
        }

    }

    return entr;
}


double entr_bigr(const std::vector<std::vector<uint32_t>>& bi_fr, double col)
{
    double entr = 0.0;
    
    for(size_t i = 0 ; i < 32; ++i)
    {
        for(size_t j = 0; j < 32; ++j)
        {

            if(bi_fr[i][j] != 0)
            {
                entr += - ( ( (double)bi_fr[i][j] / col) * (log2((double)bi_fr[i][j] / col))); 
            }

        }
    }




    return entr;
}

void print_fr_single(const std::vector<uint32_t>& pr, const std::map<std::string, uint32_t>& alph)
{
    for(size_t i = 0; i < pr.size(); ++i)
    {
        for(auto ita = alph.begin(); ita != alph.end(); ++ita)
        {
            if(ita->second == i)
            {
                std::cout << ita->first << " : ";
            std::cout << pr[i] << std::endl;
            }
        }

    }



}



int main()
{
    const auto path_raw              = "/home/daria/d1/raw_text/tr/";
    const auto path_clean_with_sp    = "/home/daria/d1/clean_text_with_spaces/clean.txt";
    const auto path_clean_without_sp = "/home/daria/d1/clean_text_without_spaces/clean.txt";

    std::map<std::string, uint32_t> alphabet = { {"а", 0 }, {"б", 1 }, {"в", 2 }, {"г", 3 }, {"д", 4 }, {"е", 5 }, {"ж", 6 }, {"з", 7 }, 
                                                 {"и", 8 }, {"й", 9 }, {"к", 10}, {"л", 11}, {"м", 12}, {"н", 13}, {"о", 14}, {"п", 15}, 
                                                 {"р", 16}, {"с", 17}, {"т", 18}, {"у", 19}, {"ф", 20}, {"х", 21}, {"ц", 22}, {"ч", 23}, 
                                                 {"ш", 24}, {"щ", 25}, {"ь", 26}, {"ы", 27}, {"э", 28}, {"ю", 29}, {"я", 30}, {" ", 31} }; 

    std::map<char16_t, size_t> alphabet_map = { {'а', 0}, {'б', 1}, {'в', 2}, {'г', 3 }, {'д', 4 }, {'е', 5 }, {'ж', 6 }, 
    {'з', 7 }, {'и', 8 }, {'й', 9 }, {'к', 10 }, {'л', 11 }, {'м', 12 }, {'н', 13 }, {'о', 14 }, {'п', 15 }, {'р', 16 }, {'с', 17 },
    {'т', 18 }, {'у', 19 }, {'ф', 20 }, {'х', 21 }, {'ц', 22 }, {'ч', 23 }, {'ш', 24 }, {'щ', 25 }, {'ъ', 26 }, {'ы', 27 },
    {'ь', 28}, {'э', 29 }, {'ю', 30 }, {'я', 31 } };//remember  ь26 ы27 э28


    std::vector<uint32_t> t_vec{}; 

    t_vec = do_clean_text(path_raw, path_clean_with_sp,    12, true,  alphabet); //' ' - are nice
   std::vector<uint32_t> let_fr = let_freq(t_vec);    
    std::vector<std::vector<uint32_t>> bi_fr_per = bi_freq_peresech(t_vec);

    double H1 = entr_single(let_fr, (double)t_vec.size());
    print_fr_single( let_fr, alphabet);
    std::cout << "H1 = " << H1 << std::endl;
    double H2 = entr_bigr(bi_fr_per, (double)t_vec.size());
    std::cout << "H2 = " << H2 << std::endl;

    std::vector<std::vector<uint32_t>> bi_fr = bi_freq(t_vec);
    double H2i = entr_bigr( bi_fr, ((double)t_vec.size()/ (double)2) );
    std::cout << "H2i = " << H2i << std::endl;



    t_vec = do_clean_text(path_raw, path_clean_without_sp, 12, false, alphabet); //' ' - are not allowed1
    let_fr = let_freq(t_vec);
    bi_fr_per = bi_freq_peresech(t_vec);

    H1 = entr_single(let_fr, (double)t_vec.size());
    print_fr_single( let_fr, alphabet);
    std::cout << "H1 = " << H1 << std::endl;
    H2 = entr_bigr(bi_fr_per, (double)t_vec.size());
    std::cout << "H2 = " << H2 << std::endl;

    bi_fr = bi_freq(t_vec);
    H2i = entr_bigr( bi_fr, ((double)t_vec.size()/ (double)2) );
    std::cout << "H2i = " << H2i << std::endl;


    std::cout << "das ist ein lab " << std::endl;

return 0;
}
