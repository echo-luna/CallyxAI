#ifndef CAI_AI_CORE_H
#define CAI_AI_CORE_H
#include <string>
#include <vector>
#include <iostream>
#include <atomic>
#include <memory>

#include <opennn/opennn.h>

//Boost Headers
#include <boost/serialization/access.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "safe_queue.h"
#include "c_msg.h"

namespace CallyxAI
{
    class AI_Core
    {
        public:
            AI_Core(std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >, //main_core_in
                    std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >, //node_in_src
                    std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >); //node_out_down
            ~AI_Core();

            void loop(); //this is the thread that is called in the thread; the thread is a lambda function so that the main logic can be handled safely. this loop calls the above functions

        protected:

            AI_Core();

            std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > > core_in; //all inputs go here
            std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > > core_out; //all outputs go here
            std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > > aux_one; //In AI_Core, this will be the second output to CTX_In_SRC. CTX_I?O_SRC

            std::string uid;
            unsigned int unique_data_sources; //determines the number of input
            std::atomic_bool run_flag; //thread synchronization

            //core logic functions
            std::vector<std::shared_ptr<c_msg> > collect(); //collects last X messages from upstream queue
            std::shared_ptr<opennn::DataSet> compile(std::vector<std::shared_ptr<c_msg> >); //places all token vectors into DataSets and pushes down stream
            std::shared_ptr<opennn::DataSet> predict(std::shared_ptr<opennn::DataSet>); //uses the current model to generate a prediction or classification
            std::pair<opennn::TrainingResults,std::shared_ptr<opennn::DataSet> >  train(std::shared_ptr<opennn::DataSet>); //online training based on if c_msg has training data or not
            std::vector<std::shared_ptr<c_msg> > decompile(std::shared_ptr<opennn::DataSet>); //converts each column back into a c_msg object


            //opennn stuffs
            opennn::NeuralNetwork core_nn;
            bool nn_init;

            //core functions
            void init();

            //boost serialization methods, should not need to change for subclasses
            friend class boost::serialization::access;

            template<class Archive>
            void const save(Archive &, const unsigned int);

            template<class Archive>
            void load(Archive &, const unsigned int);

            BOOST_SERIALIZATION_SPLIT_MEMBER()

    };
}
#endif // CAI_AI_CORE_H
