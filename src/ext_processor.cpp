#include "ext_processor.h"

//so long there is a discernable "syntax" and the data has the appropriate flags
//things should work

using namespace CallyxAI;

template <typename T>
ext_processor<T>::ext_processor(opennn::type did)
{
    this->did = did;
}

template <typename T>
ext_processor<T>::ext_processor()
{
    int * temp_i = 0;
    std::string temp = generate_uid(temp_i);
    std::array<char, sizeof(opennn::type) / sizeof(char)> tarr;
    for(int i = 0; i < sizeof(opennn::type) / sizeof(char) && i < temp.size(); i++)
    {
        tarr[i] = temp.c_str()[i];
    }
    this->did = convert_byte(tarr);
}

template <typename T>
ext_processor<T>::~ext_processor()
{
}

template <typename T>
void ext_processor<T>::process_data_down(T data_input)
{
    std::cout<<"This is a direct I/O buffer. Please do not use this in production."<<std::endl;

    //logic to process data, most likely?
    //perhaps some calls to some c_support functions

    std::shared_ptr<c_msg> t_msg(new c_msg(this->did));

    data_down.push(t_msg);
}

template <typename T>
void ext_processor<T>::process_data_up()
{
    //do the inverse of above, pretty simple in theory
}

template <typename T>
void ext_processor<T>::loop()
{
    //overall control loop, think arduino, what does this controller have to achieve endlessly?
}

template <typename T>
void ext_processor<T>::start()
{
    //start the thread
}

template <typename T>
void ext_processor<T>::stop()
{
    //throw run_flag to false
}

template <typename T>
void ext_processor<T>::put_data(std::shared_ptr<T> data)
{
    //puts shared_ptr<T> into a data queue to be tokenized
}

template <typename T>
std::shared_ptr<T> ext_processor<T>::get_data()
{
    //gets detokenized std::shared_ptr<T> data out
}
