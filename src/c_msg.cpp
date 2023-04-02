#include "c_msg.h"

using namespace CallyxAI;

c_msg::c_msg(int mid, opennn::type did)
{
    msg_id = mid;
    src_id = did;
}

c_msg::c_msg(opennn::type did)
{
    int * i = 0;
    this->msg_id = generate_uid(i);
}

c_msg::c_msg()
{
}

void c_msg::get_mid()
{
    return msg_id;
}

void c_msg::get_did()
{
    return src_id;
}

std::vector<opennn::type> c_msg::get_unprocessed_vector()
{
    return unprocessed_data;
}

std::vector<opennn::type> c_msg::get_processed_vector
{
    return processed_data;
}

void c_msg::set_unproccessed_vector(std::vector<opennn::type> vec)
{
    unprocessed_data = vec;
}

void c_msg::set_processed_vector(std::vector<opennn::type> vec)
{
    processed_data = vec;
}

template<class Archive>
void const c_msg::save(Archive & ar, const unsigned int version)
{
    ar << BOOST_SERIALIZATION_NVP(msg_id);
    ar << BOOST_SERIALIZATION_NVP(src_id);
    ar << BOOST_SERIALIZATION_NVP(unprocessed_data);
    ar << BOOST_SERIALIZATION_NVP(processed_data);
}

template<class Archive>
void c_msg::load(Archive & ar, const unsigned int version)
{
    ar >> msg_id;
    ar >> src_id;
    ar >> unprocessed_data;
    ar >> processed_data;
}
