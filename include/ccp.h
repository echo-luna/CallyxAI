#ifndef C_CCP_H
#define C_CCP_H

///Callyx Communication Protocol (R1.0.1 will have this file renamed to RNC but for now rpc)

#include <queue>
#include <memory>
#include <atomic>
#include <string>
#include <mutex>

#include "zmqpp/zmqpp.hpp"
#include "zmqpp/proxy.hpp"

namespace CallyxAI
{
    namespace ccp
    {

        std::string port_a = "4242";
        std::string port_b = "6969";

        typedef zmqpp::message rmsg;


        class c_con
        {

            public:

                c_con();

                void close();

                void start_wd();

                ~c_con();

            private:

                static zmqpp::context z_con;
                zmqpp::socket z_xpub;
                zmqpp::socket z_xsub; //forward


                std::shared_ptr<std::thread> prox_thr;

                void start_proxy();

                void proxy_func();
        };

        class pub
        {
            public:
                pub(std::string);

                ~pub();

                bool send(rmsg & data, bool block = false)
                {
                    return z_soc.send(data,!block);
                }

            private:

                zmqpp::context z_con;
                zmqpp::socket z_soc;
        };

        class sub
        {

            zmqpp::context z_con;
            zmqpp::socket z_soc;

            public:
                sub();
                ~sub();

                bool rec(rmsg&, bool = false);

                void subscribe(std::string);
                void subscribe(auto & , auto & );
                void connect();

        };
    }
}

#endif // C_CPC_H
