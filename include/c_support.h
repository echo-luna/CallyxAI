#ifndef C_SUPPORT_H
#define C_SUPPORT_H

#include <string>
#include <sstream>
#include <chrono>
#include <functional>
#include <bitset>
#include <sstream>
#include <ostream>
#include <assert.h>
#include <array>

namespace CallyxAI
{
    std::string generate_uid(void *); //generates a unique id from a memory address

    std::vector<opennn::type> str_to_tokens(std::string); //divides string data into four bit chunks

    Tensor<opennn::type, 1> tensor_from_vec(std::vector<opennn::type>); //takes vector from previous function, and converts it to a tensor to be used in construction of a new dataset

    std::vector<opennn::type> vec_from_tensor(Tensor<opennn::type, 1>); //takes a tensor slice and converts it to a data vector

    std::string detokenize_vec(std:vector<opennn::type>);

    opennn::type convert_byte(std::array<char, sizeof(opennn::type)/sizeof(char)>); //converts "bytes" of chars from some string data and converts each token into and each value is added

    std::string convert_byte(opennn::type); //converts a individual token into a four byte string

    std::string generate_random_ip_address(); //generates a random IP on the 127.X.X.X loopback
}


class LOGGER {
public:
    // destructor is called when output is collected and
    // the temporary Logger() dies (after the ";").
    ~LOGGER() {
        if(RDEBUG)
        {LOGGER::mutex_.lock(); // make sure I am the only one writing
        std::cout << buffer_.str() << std::endl; // write
        std::cout << std::flush; // push to output
        LOGGER::mutex_.unlock();} // I am done, now other threads can write
    }

    // funny construct you need to chain logs with "<<" like
    // in std::cout << "var = " << var;
    template <typename T>
    LOGGER &operator<<(T input) {
        if(RDEBUG)
        {buffer_ << input;
        return *this;}
    }

private:
    // collect the output from chained << calls
    std::ostringstream buffer_;

    // mutex shared between all instances of Logger
    static std::mutex mutex_;
};

#endif
