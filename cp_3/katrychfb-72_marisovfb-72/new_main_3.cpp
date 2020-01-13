#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <iterator>
#include <bitset>
#include <map>
#include <cmath>
#include <cstdlib>

//D.Katrych lab 3, variant 11 ( 6 )
//one file of code !

//эту функцию я оставила, потому что она безобидна, это всего лишь переведение текста в вектор, даже если она с плагиатом
std::vector<uint32_t> get_text(std::string_view path)
{

    std::ifstream Stream(path.data());
    std::vector<uint32_t> text_vec{};
    text_vec.reserve(8000);

    uint32_t temp_let{};
    std::string str{};
    while(getline(Stream, str))
    {
        for(size_t i = 0; i < str.length(); ++i)
        {
            temp_let = str[i];
            temp_let = temp_let & 0b00011111;

            //these 2 ifs corrects missmatch between default alphabet with 'ъ' and alphabet used in this lab
            if(temp_let == 28)
            {
                temp_let = 26;
            }
            if(temp_let > 27) 
            {
                --temp_let;
            }
            
            text_vec.push_back(temp_let); 
        }

    }


    Stream.close();
    text_vec.shrink_to_fit();
    if(text_vec.size() % 2 == 1)
    {
        text_vec.push_back(26);
    }

return text_vec;
}

std::vector< std::pair<uint32_t, uint32_t> > CollectingBigrFreq(std::vector<uint32_t> text_vec)
{
    std::vector<std::pair<uint32_t, uint32_t> > bigr_freq(5, {0,0});// {0,0} - std::initializer_list to correctly invoke constructor
                                                                    // how to use std::make_pair() here
  

    uint32_t bigr_in_text[31][31]= {};//stats
    
    for(size_t i = 0; i < text_vec.size() - 1; ++i)
    {
        ++bigr_in_text[ text_vec[i] ][ text_vec[++i] ];//collecting stats
    }



    for(size_t most = 0; most < 5; ++most)
    {
        for(size_t i = 0; i < 31; ++i)
        {
            for(size_t j = 0; j < 31; ++j)
            {
                if( bigr_in_text[i][j] > bigr_in_text[bigr_freq[most].first][bigr_freq[most].second]  )
                {
                   bigr_freq[most].first  = i;
                   bigr_freq[most].second = j;
                }
            }
        }
        bigr_in_text[ bigr_freq[most].first ][ bigr_freq[most].second ] = 0;//deleting the greatest
    }

return bigr_freq;
}

//математика
void ObernMod(int32_t& op1, const uint32_t& mod)
{
    if(op1 < 0)
    {
        op1 = mod + (op1 ); 
    }
    else
    {
        op1 = op1 ;
    }
}

//математика
int32_t OM(int32_t op1)
{
    if(op1 < 0)
    {
        return op1 = 961 + (op1 % 961);
    }
    else
    {
        return op1 = op1 % 961;
    }
}

//математика
int32_t ExtendedEuclid(int32_t a, int32_t b, int32_t& u, int32_t& v) //(c) Handbook of Applied Cryptography
{
    if(b == 0){
    u = 1;
    v = 0;
    return a;
    }
    int32_t v_1 = 1;
    int32_t v_2 = 0;
    int32_t u_1 = 0;
    int32_t u_2 = 1;

    int32_t q = 0;
    int32_t r = 0;

    while( b > 0 )
    {
        q   =   a / b;
        r   =   a - (q *   b);
        u   = u_2 - (q * u_1);
        v   = v_2 - (q * v_1);
        a   =   b;
        b   =   r;
        u_2 = u_1;
        u_1 =   u;
        v_2 = v_1;
        v_1 =   v;
    }
    u = u_2;
    v = v_2;
    u < 0 ? u = 961 + u : u = u;
    v < 0 ? v = 961 + v : v = v;
 
return a;
}

//математика
int32_t LinComp(int32_t a, int32_t b, int32_t m, int32_t& u, int32_t& v, int32_t& add)
{

    int32_t x = 0;
    int32_t greatest_common_divisor = ExtendedEuclid(a, m, u, v);

    ObernMod(u, m);
    ObernMod(v, m);

    if( greatest_common_divisor == 1 ){
        if( ( (a*u) % m ) == 1 ){
            return x = ( u*b ) % m;
        }
        else{
            return x = ( v*b ) % m;
        }
    }
    else{
        if( (b % greatest_common_divisor) == 0 ){
            add = greatest_common_divisor;
            return x = LinComp( (a/greatest_common_divisor), (b/greatest_common_divisor), (m/greatest_common_divisor), u, v, add);
        }
        else{
            return x = 1111;
        }
    }   
        
}


int32_t unified_key_part_finder(bool isa, int32_t a, int32_t b, int32_t x, int32_t y, int32_t& u, int32_t& v, int32_t& add)
{
    int32_t key_value = 0;
    if(isa == false)
    {
            if( ( y - (x*a) ) > 0 ){
            return ( (y - (x*a)) % (961) );
            }
            else{
            return ( (961) + ( ( y - (x*a) ) % (961) ) );
            }

    }
    else
    {
         return LinComp(x, y, 961, u, v, add);
    }


    return key_value;
}


void obototen(int& val, int u, int v)
{

    (( (val*u) % 961 ) == 1) ? val = u : val = v;  
    
}
//ф-я расшифровки осталась без особых перемен
void Decrypt(const std::map<std::string, uint32_t>& alph, std::string_view path_pt, const std::vector<uint32_t>& text_ct, 
             int32_t keyA, int32_t keyB, std::vector<uint32_t>& legacy_text)
{
    std::ofstream pt_stream(path_pt.data());
    
    int32_t u = 0;
    int32_t v = 0;
    int32_t car = 0;
    pt_stream << "a:" << keyA << ", b:" << keyB << std::endl;

    car = ExtendedEuclid(keyA, 961, u, v);

    obototen(keyA, u, v);
    int32_t curr_bigr = 0;
    
    size_t ij = 0;
    for(size_t i = 0; i < text_ct.size(); i += 2)
    {
        curr_bigr = text_ct[i] * 31 + text_ct[i + 1];//getting ct bigram
        curr_bigr = (curr_bigr - keyB);
        if(curr_bigr < 0){
            curr_bigr = 961 + (curr_bigr % 961);
        }
        curr_bigr = ((keyA * (curr_bigr)) % (961));//getting pt bigram

    
        for(auto im = alph.begin(); im != alph.end(); ++im)
        {
            if(im->second == ((curr_bigr - (curr_bigr % 31) ) / 31))
            {
                legacy_text[ij] = ((curr_bigr - (curr_bigr % 31)) / 31);
                ++ij;
                pt_stream << im->first;
            }
        }
    
        for(auto im = alph.begin(); im != alph.end(); ++im)
        {
            if(im->second == curr_bigr % 31)
            {
                legacy_text[ij] = (curr_bigr % 31) ;
                ++ij;
                pt_stream << im->first;
            }
        }
    
    }

    pt_stream << std::endl << std::endl;
    pt_stream.close();
}




double CalculateIndex(std::vector<uint32_t> cand_pt,   const std::map<std::string, uint32_t>& alphabet)
{
    std::vector<uint32_t> statistics(31, 0);

    for(auto it = cand_pt.begin(); it != cand_pt.end(); ++it)
    {
        ++statistics[*it];
    }

    double Index = 0.0;
    for(auto it = statistics.begin(); it != statistics.end(); ++it)
    {
        if(*it != 0)
        {
            Index += ( ( 1 / (double)(cand_pt.size() * (cand_pt.size() - 1))) * ( (*it)*(*it - 1)) ); 
        }
    }
   
    return Index;
}

void obor(int32_t& op ,int a, int b)
{

    (a >= b) ? op = (a - b) : op = (961 + (a - b));
}

void poisk_ego_togo_kotoriy_osmislenniy(bool& is_osm, std::vector<uint32_t> legacy_text, std::map<std::string, uint32_t> alphabet)
{
    double In = 0.0;


    In = CalculateIndex(legacy_text, alphabet);
    std::cout << In << std::endl ;
    if(In > 0.054 && In < 0.056)
    {
    is_osm = true ;
    }
    else{
    is_osm = false;
    }
}

void Lineal_Equasions_System(std::string out_plain_text, std::vector<int32_t> hardcoded_rus_bigr, std::vector<int32_t> atext_bigr_st,
                             std::vector<uint32_t> legacy_text, std::vector<uint32_t> text_vec, std::map<std::string, uint32_t> alphabet)
{

    int32_t ah, a, b, addi, u, v, dx, dy;
    u = 0;
    v = 0;
    addi = 0;
    dx = 0;
    dy = 0;
    ah = 0;
    bool is_osm = false;


    auto itx = hardcoded_rus_bigr.begin();
    auto ity = atext_bigr_st.begin();
uint32_t oracul_predskazatel = 0;
uint32_t volya_sudbi_1 = 0;
uint32_t volya_sudbi_2 = 0;


while(true)
{
            oracul_predskazatel = rand() % 5;
            volya_sudbi_1 = oracul_predskazatel;
            auto itx1 = itx + oracul_predskazatel;
            
            oracul_predskazatel = rand() % 5;
            volya_sudbi_2 = oracul_predskazatel;
            auto ity1 = ity + oracul_predskazatel;

            

            oracul_predskazatel = rand() % (5 - volya_sudbi_1);
            auto itx2 = itx1 + oracul_predskazatel;
            oracul_predskazatel = rand() % (5 - volya_sudbi_2);
            auto ity2 = ity1 + oracul_predskazatel;




            while(itx2 == itx1)
            {
            oracul_predskazatel = rand() % (5 - volya_sudbi_1);
            itx2 = itx1 + oracul_predskazatel;
            if(volya_sudbi_1 == 4)
            {
                itx2 = itx;
            }

            }
            while(ity2 == ity1)
            {
            oracul_predskazatel = rand() % (5 - volya_sudbi_2);
            ity2 = ity1 + oracul_predskazatel;
            if(volya_sudbi_2 == 4)
            {
                ity2 = ity;
            }

            }
        
                   obor(dx, *itx1, *itx2);
                   obor(dy, *ity1, *ity2);


                       a = unified_key_part_finder(true, 0, 0, dx, dy, u, v, addi);
                       if(a != 1111)
                       {
                            ah = a;
                            if(addi == 0)
                            {
                                b = unified_key_part_finder(false, a, 0, *itx1, *ity1, u, v, addi);
                                
                                Decrypt(alphabet, out_plain_text, text_vec, a, b, legacy_text);
                                std::cout << "(" << a << ", " << b <<"),another index : " ;

                                poisk_ego_togo_kotoriy_osmislenniy(is_osm, legacy_text, alphabet);
                                if(is_osm){return;}

                                for(auto it = legacy_text.begin(); it != legacy_text.end(); ++it)
                                {
                                    *it = 0;
                                }
                                    




                            }else
                            {
                                for(int32_t k = 0; k < addi; ++k)
                                {
                                    a = ah + (k*(961/addi));
                                b = unified_key_part_finder(false, a, 0, *itx1, *ity1, u, v, addi);
                                Decrypt(alphabet, out_plain_text, text_vec, a, b, legacy_text);

                                std::cout << "(" << a << ", " << b <<"),another index : " ;
                                poisk_ego_togo_kotoriy_osmislenniy(is_osm, legacy_text, alphabet);
                                if(is_osm){return;}

                                for(auto it = legacy_text.begin(); it != legacy_text.end(); ++it)
                                {
                                    *it = 0;
                                }

                                }
                            }
                       }
                       addi=0;

}


}


int main()
{
    auto const ascii_location_text = "/home/daria/littleDcr3/crypto_cp_3/variants/12.txt";
    auto const out_plain_text = "/home/daria/littleDcr3/out_PT.txt";
     
    std::ofstream flusher_stream(out_plain_text);
    flusher_stream << "new day -> new try\n" ; //to erase previous texts
    flusher_stream.close();
    
    std::vector<uint32_t> text_vec{};
    std::vector< std::pair<uint32_t, uint32_t> > text_bigr_st{};
    std::vector<int32_t> hardcoded_rus_bigr{545, 417, 572, 403, 168};
//    legacy_text.push_back(0); // специально заношу первую букву в расшифрованый текст, что бы при расшифровке индексы
                              // сдвинулись на 1 и функция CollectingBigrFreq(..) считала биграмы пересечения относительно
                              // биграм шифровки (условие из методички стр.3 абзац 2)


    //26 is missing
    std::map<std::string, uint32_t> alphabet = { {"а", 0 }, {"б", 1 }, {"в", 2 }, {"г", 3 }, {"д", 4 }, {"е", 5 }, {"ж", 6 }, {"з", 7 }, 
                                                 {"и", 8 }, {"й", 9 }, {"к", 10}, {"л", 11}, {"м", 12}, {"н", 13}, {"о", 14}, {"п", 15}, 
                                                 {"р", 16}, {"с", 17}, {"т", 18}, {"у", 19}, {"ф", 20}, {"х", 21}, {"ц", 22}, {"ч", 23}, 
                                                 {"ш", 24}, {"щ", 25}, {"ь", 26}, {"ы", 27}, {"э", 28}, {"ю", 29}, {"я", 30}  }; 

    //step 1 : getting CT from ../crypto_cp_3/variants
    text_vec = get_text(ascii_location_text);
    std::cout << text_vec.size() << std::endl;

    std::vector<uint32_t> legacy_text(text_vec.size(), 0);
    //step 2 : collecting the most frequent bigrams in CT
    text_bigr_st = CollectingBigrFreq(text_vec);
    
    //step 3 : 
    std::vector<int32_t> atext_bigr_st;
    for(size_t i = 0; i < 5; ++i)
    {
       atext_bigr_st.push_back(text_bigr_st[i].first * 31 + text_bigr_st[i].second);
    }

    //step 4 : решение системы линейных уравнений
    Lineal_Equasions_System(out_plain_text, hardcoded_rus_bigr, atext_bigr_st, legacy_text, text_vec, alphabet);

    std::cout << "everything should be done, check out_PT " << std::endl;

return 0;
}
