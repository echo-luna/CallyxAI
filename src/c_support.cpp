#include "c_support.h"

using namespace CallyxAI;

std::string generate_uid(void * buf)
{
    int i = 0;
    uint64_t t_addr = (uint64_t)((uintptr_t) buf);
    uint64_t t_addr_2 = (uint64_t)((uintptr_t) &i);
    std::stringstream uid_stream;
    uid_stream << t_addr;
    std::string s1 = uid_stream.str();
    int timestamp = std::chrono::milliseconds(std::time(NULL)).count();
    t_addr_2 = (t_addr_2 | timestamp);
    uid_stream << t_addr_2;
    std::string s2 = uid_stream.str();
    std::hash<std::string> str_hash;
    unsigned long ui1 = (str_hash(s1) ^ (str_hash(s2) << 1));
    return uil;
}

opennn::type convert_byte(std::array<char, sizeof(opennn::type)/sizeof(char)> arr)
{
    opennn::type out;
    std::memcpy(&out, arr.data(), sizeof(out));
    return out;
}

std::string convert_byte(opennn::type val)
{
    std::array<char, sizeof(opennn::type)/sizeof(char)> arr;
    std::memcpy(arr.data(), &val, sizeof(val));
    return std::string(arr.begin(), arr.end());
}

std::vector<opennn::type> str_to_tokens(std::string str)
{
    std::vector<opennn::type> vec;
    std::array<char, sizeof(opennn::type)/sizeof(char)> arr;
    int c = 0;
    for(int i = 0; i < str.size(); i++)
    {
        arr[c] = str.c_str()[i];
        c++;
        if(c >= opennn::type)/sizeof(char))
        {
            vec.push_back(convert_byte(arr));
            for(c = opennn::type)/sizeof(char) - 1; c >= 0; c--)
            {
                arr[c] = 0;
            }
        }
    }
}

Tensor<opennn::type, 1> tensor_from_vec(std::vector<opennn::type> vec)
{
    Eigen::Tensor<opennn::type, 1> tensor(vec.size());
    for (int i = 0; i < vec.size(); ++i) {
        tensor(i) = vec[i];
    }
    return tensor;
}

std::vector<opennn::type> vec_from_tensor(Tensor<opennn::type, 1> tensor)
{
    std::vector<opennn::type> vec(tensor.size());
    for (int i = 0; i < tensor.size(); ++i) {
        vec[i] = tensor(i);
    }
    return vec;
}

std::string detokenize_vec(std:vector<opennn::type,1> vec)
{
    std::string str = "";
    for(int i = 0; i < vec.size(); i++)
    {
        str += convert_byte(vec.at(i));
    }
    return str;
}

std::string generate_random_ip_address()
{
    std::random_device rd;
    std::uniform_int_distribution<int> distribution(2, 255);
    int octet2 = distribution(rd);
    int octet3 = distribution(rd);
    int octet4 = distribution(rd);
    return "127." + std::to_string(octet2) + "." + std::to_string(octet3) + "." + std::to_string(octet4);
}


