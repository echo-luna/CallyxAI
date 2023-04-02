#ifndef C_MSG_H
#define C_MSG_H

//Boost Headers
#include <boost/serialization/access.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

//STL Headers
#include <memory>
#include <string>

#include <opennn/opennn.h>

namespace CallyxAI
{
    class c_msg
    {
        public:
            c_msg(int, opennn::type); //permits messages to be reconstructed, if necessary, with new data
            c_msg(opennn::type); //standard constructor, message ID are usually autogen
            void get_mid(); //get the "unique" message ID
            void get_did(); //get the unique data processor id
            std::vector<opennn::type> get_unprocessed_vector(); //get the unprocessed data vector
            std::vector<opennn::type> get_processed_vector(); //get the processed data vector
            void set_unproccessed_vector(std::vector<opennn::type>); //set the unprocessed data vector
            void set_processed_vector(std::vector<opennn::type>); //set the processed data vector

        private:

            c_msg();

            int msg_id; //the id of the message
            opennn::type src_id; //the source of the data, uses the type that the current model uses. It is recommended to register all new IDs with the host container before sending data
            std::vector<opennn::type> unprocessed_data; //the token data to be evaluated through the AI
            std::vector<opennn::type> processed_data; //the data used as the input training data against the new token data as the "expected" outcome

            friend class boost::serialization::access;

            template<class Archive>
            void const save(Archive &, const unsigned int);

            template<class Archive>
            void load(Archive &, const unsigned int);

            BOOST_SERIALIZATION_SPLIT_MEMBER()
    };

}

#endif // C_MSG_H
