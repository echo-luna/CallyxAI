#ifndef C_EXT_PROCESSOR_H
#define C_EXT_PROCESSOR_H
#include <boost/network/protocol/http/client.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <thread>
#include <opennn/opennn.h>

#include "c_msg.h"
#include "safe_queue.h"

namespace CallyxAI
{
    /**std::string data_to_sentence(const Tensor<opennn::type, 1>& data_vector) const
    {
        const Tensor<std::string,1> columns_names = get_columns_names();
        const Index columns_number = get_columns_number() - 1;

        Tensor<std::string, 1> tokens(columns_number);

        for(Index i = 0; i < columns_number; i++)
        {
            const std::string& column_name = columns_names(i);

            if(column_name != "input_sentence" && column_name != "output_sentence")
            {
                opennn::type value = data_vector(i);

                if(value != 0.0)
                {
                    Index frequency = round(value);

                    for(Index j = 0; j < frequency; j++)
                    {
                        tokens.append(column_name);
                    }
                }
            }
        }

        return tokens.to_string(" ");
    }**/


    template <typename T>
    class ext_processor
    {
        public:
            ext_processor(opennn::type); //accepts a preset DPID
            ext_processor(); //autogen a DPID
            ~ext_processor();

            void put_data(std::shared_ptr<T>) //puts data into the processor to be processed.
            std::shared_ptr<T> get_data() //pulls processed data out

            void start(); //start the external processor processes
            void stop(); //stops the external processor processes

        protected:
            std::atomic_bool run_flag;

            SafeQueue<std::shared_ptr<c_msg> > data_up; //data that has been received from downstream.
            SafeQueue<std::shared_ptr<c_msg> > data_down; //data to be sent downstream.

            SafeQueue<std::shared_ptr<T> > data_in_queue; //data to be tokenized
            SafeQueue<std::shared_ptr<T> > data_out_queue; //detokenized data

            void loop() //the logic loop that the processor uses. Typical scheme is one tokenized data in, one detokenized out

            void process_data_down(T); //thread friendly but not thread safe
            void process_data_up(); //thread friendly but not thread safe, blocking

            boost::network::http::client proc_client; //how the processor connects to the service
            opennn::type DPID; //Data Processor IDentifer

    };
}

#endif // C_EXT_PROCESSOR_H
