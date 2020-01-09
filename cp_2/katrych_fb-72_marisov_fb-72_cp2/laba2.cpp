//D.Katrych var6

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <iterator>
#include <random>
#include <string_view>


std::vector<double> EncryptAndCalculateIndex(std::vector<uint32_t> plain_text, std::string_view path, 
                                             std::map<char16_t, size_t> alphabet );

uint32_t FindKeySize(std::vector<uint32_t> cipher_text,  std::vector<double> Precalc_Index);

void DecryptAndWriteToFile( std::vector<uint32_t>& cipher_text, uint32_t key_size, std::map<char16_t, size_t> alphabet,
                            std::string_view path, std::vector<uint32_t>& key_main, std::vector<uint32_t>& key_raw);

std::vector<uint32_t> GetTextConvertToVector(std::string_view path, std::map<char16_t, size_t> &alphabet);

void Special(const std::vector<uint32_t>& key_main,  std::vector<uint32_t>& key_raw, const std::map<char16_t, size_t>& alph,
            const std::vector<uint32_t>& text_vec);

int main()
{

    auto CT_file = "/home/daria/littleDcr2/CipherText.txt"; 
    auto My_OT_file = "/home/daria/littleDcr2/my_OT.txt";
    auto Final_OT_file = "/home/daria/littleDcr2/Final_OT.txt";
    auto My_CT_file = "/home/daria/littleDcr2/my_CT.txt";

    std::map<char16_t, size_t> alphabet_map = { {'а', 0}, {'б', 1}, {'в', 2}, {'г', 3 }, {'д', 4 }, {'е', 5 }, {'ж', 6 }, 
    {'з', 7 }, {'и', 8 }, {'й', 9 }, {'к', 10 }, {'л', 11 }, {'м', 12 }, {'н', 13 }, {'о', 14 }, {'п', 15 }, {'р', 16 }, {'с', 17 },
    {'т', 18 }, {'у', 19 }, {'ф', 20 }, {'х', 21 }, {'ц', 22 }, {'ч', 23 }, {'ш', 24 }, {'щ', 25 }, {'ъ', 26 }, {'ы', 27 },
    {'ь', 28}, {'э', 29 }, {'ю', 30 }, {'я', 31 } };

    std::vector<uint32_t> text_vec{};
    std::vector<double>   Index{};
    text_vec.reserve(7000);
    Index.reserve(20);


    //1st and 2nd tasks 
    text_vec = GetTextConvertToVector(My_OT_file, alphabet_map);
    Index    = EncryptAndCalculateIndex(text_vec, My_CT_file, alphabet_map);

    //3rd task
    text_vec.reserve(7000);
    text_vec = GetTextConvertToVector(CT_file, alphabet_map);
    uint32_t key_size = 0;
    key_size = FindKeySize(text_vec, Index);
    
    


    
    std::vector<uint32_t> key_main{};
    std::vector<uint32_t> key_raw{};
    DecryptAndWriteToFile(text_vec, key_size, alphabet_map, Final_OT_file, key_main, key_raw);

    
    Special(key_main, key_raw, alphabet_map, text_vec);
    

    return 0;
}

void Special(const std::vector<uint32_t>& key_main,  std::vector<uint32_t>& key_raw, const std::map<char16_t, size_t>& alph,
            const std::vector<uint32_t>& text_vec)
{
    std::cout << "**************************** Do you want to start special part? only var 6 ! (y/n) ***********************" << std::endl;
    char answ = 'a';
    std::cin >> answ;
    if(answ != 'y'){return;}
    key_raw[15] = 4;


    std::cout << "good key : ";
    for(auto i = 0; i < 17; ++i)
    {
     std::cout << key_main[i] << ",   ";   
    }
    std::cout << std::endl;

    std::cout << "raw  key : ";
    for(auto i = 0; i < 17; ++i)
    {
     std::cout << key_raw[i] << ",   ";   
    }
    std::cout << std::endl;


    //key[15] = 13;
    std::vector<uint32_t> stats(32, 0);

    for(size_t i = 15; i < text_vec.size(); i += 17)
    {
        ++stats[text_vec[i]];
    }


    char costil{};
    std::cout << "Problem is in block 16.  Below you can see statistics for this block in PT : " << std::endl;
    for(size_t i = 0; i < alph.size(); ++i)
    {
        for(auto it = alph.begin(); it != alph.end(); ++it)
        {
            if( i == it->second)
            {
                std::cout << i << "  ";
                costil = ((it->first >> 8) & 0b0000000011111111);
                std::cout << costil ;
                costil = (it->first & 0b0000000011111111);
                std::cout << costil;
                std::cout << " : " << stats[i] << std::endl;

            }
        }
    }

    std::cout << "size of text : " << text_vec.size() << ",  size of block : " << text_vec.size() / 17 << std::endl;
    std::cout << "in this block , the most frequent letter is 'e', not 'o'" << std::endl;


}





std::vector<uint32_t> GetTextConvertToVector(std::string_view path, std::map<char16_t, size_t> &alphabet)
{

    std::ifstream my_file_stream(path.data());//TODO add check of invalid path !!

    std::vector<uint32_t> text_vec{};
    std::string str{};
    char16_t temp_container; 
    char16_t low_cont;

    
    while( getline(my_file_stream, str) )
    {
        for(size_t i = 0; i < str.length(); ++i) 
        {
                if( i % 2 == 1)
                {
                    temp_container = str[i - 1];
                    low_cont = str[i] & 0b0000000011111111;
                    temp_container = (temp_container << 8) | low_cont;
                
                   if( alphabet.find(temp_container) != alphabet.end() )
                   {
                    text_vec.push_back( (alphabet.find(temp_container)->second) ); // how to get ->second in map
                   }

                }
        }

    }

//    std::cout << text_vec.size() << std::endl; //DEBUG feature
    text_vec.shrink_to_fit();

    my_file_stream.close();

    return text_vec;
}










std::vector<double> EncryptAndCalculateIndex(std::vector<uint32_t> plain_text, std::string_view path, 
                                             std::map<char16_t, size_t> alphabet)
{
    std::ofstream out_Stream(path.data());
    
    std::random_device procNoise;//gets access to noises of the CPU
    std::minstd_rand gen(procNoise());//takes random noise and transforms into "good random uin32_t"
    std::uniform_int_distribution<uint32_t> keyDistr(0,31);// % 32

    std::vector<double> Index{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<uint32_t> key{};
    std::vector<uint32_t> statistics{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


    
    for(size_t i = 0; i < 20; ++i)
    {
        key.push_back(keyDistr(gen));
    }
    key[0] = 0;// first shift is 0, so that we get the same plain text after encryption
    
    std::string str{};
    str.reserve(key.size() * 2 );

    char buff;
    
    for(size_t s = 1; s < (key.size()*2); s += 2)
    {
        for(auto it_m = alphabet.begin(); it_m != alphabet.end(); ++it_m)
        {
            if( key[ (s - 1) / 2] == it_m->second)
            {
                buff = ( (it_m->first >> 8) & 0b0000000011111111);
                str.push_back( buff );
                buff = (it_m->first & 0b0000000011111111);
                str.push_back( buff );

            }
        }
    }
    
   

    for(size_t i = 1; i < 21; ++i)
    {
        out_Stream << std::endl << "    Key size = " << i - 1 << std::endl;
        for(size_t k = 1; k < i; ++k)
        {
            out_Stream << "k_" << k-1 << " : " << str[(k - 1)*2] << str[(( k -1 )*2) + 1] << std::endl;
            
        }


        for(size_t j = 0; j < plain_text.size(); ++j)
        {
            ++statistics[((plain_text[j] + key[j % i]) % 32)];
            for(auto it = alphabet.begin(); it != alphabet.end(); ++it)
            {
                if(it->second == ( (plain_text[j] + key[j % i]) % 32 ) )
                {
                    buff = ((it->first >> 8) & 0b0000000011111111);
                    out_Stream << buff ;
                    buff = (it->first & 0b0000000011111111);
                    out_Stream << buff ;
                }
            }
            if(j % 100 == 0 && j != 0){out_Stream << std::endl;}
          
        }
        out_Stream << std::endl << std::endl;

        
               
        for(auto it = statistics.begin(); it != statistics.end(); ++it)
        {
            if(*it != 0)
            {
                Index[i - 1] += ( ( 1 / (double)(plain_text.size() * (plain_text.size() - 1))) * ( (*it)*(*it - 1)) ); 
            }
            *it = 0;
        }


       std::cout << "Key size : " << i << ",   Index : " << Index[i - 1] << std::endl << std::endl;

    }
   
    out_Stream.close();
   
    return Index;
}



uint32_t FindKeySize(std::vector<uint32_t> cipher_text, std::vector<double> Precalc_Index)
{
    uint32_t key_size = 0;
    double Index = 0.0;

    std::vector<uint32_t> statistics{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //2nd method
    for(size_t i = 0; i < cipher_text.size(); ++i)
    {
        ++statistics[ cipher_text[i] ];
    }

    for(auto it = statistics.begin(); it != statistics.end(); ++it)
    {
        Index += ( ( 1 / (double)(cipher_text.size() * (cipher_text.size() - 1))) * ( (*it)*(*it - 1)) ); 
        *it = 0;
    }

    std::cout << "Index of the cipher_text : " << Index << "  desired ranges r = 2-5 (" << 
              Precalc_Index[1] << " - " << Precalc_Index[4] << ")"<< std::endl;
    std::cout << "(if the Index is out of this range => key size greater than 6" << std::endl;


    //1st method 
    double Index_holder = 0.0;
    double Index_mean_value = 0.0;
    Index = 0.0;


    for(size_t r = 2; r < 24; ++r)
    {
        for(size_t Yi = 0; Yi < r; ++Yi)
        {
            for(size_t letters_per_block = Yi; letters_per_block < cipher_text.size();letters_per_block += r)
            {
                ++statistics[ cipher_text[letters_per_block] ];
            }
            /*
            for(size_t lpb = 0; lpb < (cipher_text.size()/r); ++lpb)
            {
                ++statistics[ cipher_text[ (lpb*r) + Yi ] ];
            }
            */

            for(auto it = statistics.begin(); it != statistics.end(); ++it)
            {
                Index += ( ( 1 / (double)((cipher_text.size() / r) * ((cipher_text.size() / r)  - 1))) * ( (*it)*(*it - 1)) ); 
                *it = 0;
            }
            Index_mean_value += Index/r;
            Index = 0.0;

        }
        if(Index_mean_value > Index_holder)
        {
            Index_holder = Index_mean_value;
            key_size = r; 
        }
        std::cout << "r : " << r  << ", Index : " << Index_mean_value << std::endl;
        Index_mean_value = 0.0;

    }

return key_size;
}




void DecryptAndWriteToFile( std::vector<uint32_t>& cipher_text, uint32_t key_size, std::map<char16_t, size_t> alphabet,
                            std::string_view path, std::vector<uint32_t>& key_main, std::vector<uint32_t>& key_raw)
{
    std::ofstream out_stream(path.data());
    
    std::vector<uint32_t> statistics{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    std::vector<uint32_t> key{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint32_t temp_stat = 0;
    char costil;
    
    for(size_t Yi = 0; Yi < key_size; ++Yi)
    {
        for(size_t j = Yi; j < cipher_text.size(); j += key_size)
        {
            ++statistics[ cipher_text[j] ];
        }
        
        for(size_t i = 0; i < statistics.size(); ++i)
        {
            if(statistics[i] > temp_stat)
            {
                temp_stat = statistics[i];
                key[Yi] = (i - 14) % 32;
                key[Yi] < 0 ? key[Yi] += 32 : key[Yi] += 0 ;
            }
            statistics[i] = 0;
        }
        temp_stat = 0;

        key_raw = key;
        //ispravlenija klucha
        key[15] = 13;
        key_main = key;


        out_stream << "k " << Yi << " : ";
        for(auto it = alphabet.begin(); it != alphabet.end(); ++it)
        {
            if(it->second == key[Yi])
            {
                costil = ((it->first >> 8) & 0b0000000011111111);
                out_stream << costil ;
                costil = (it->first & 0b0000000011111111);
                out_stream << costil;
            }
        }
        out_stream << std::endl;
    
       
    }

    for(size_t v = 0 ; v < cipher_text.size(); ++v)
    {
        cipher_text[v] = (cipher_text[v] - key[ v % key_size]) % 32;
        cipher_text[v] < 0 ? cipher_text[v] += 32 : cipher_text[v] += 0;
        for(auto it = alphabet.begin(); it != alphabet.end(); ++it)
        {
            if(it->second == cipher_text[v])
            {
                costil = ((it->first >> 8) & 0b0000000011111111);
                out_stream << costil ;
                costil = (it->first & 0b0000000011111111);
                out_stream << costil;
            }
        }
        if(v % 100 == 0 && v != 0){out_stream << std::endl;}

    }




    out_stream.close();
}
