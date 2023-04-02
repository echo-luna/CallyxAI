#ifndef C_HOST_CONTAINER_H
#define C_HOST_CONTAINER_H

#include "AI_Container.h"

//this class exists mainly to supplement the AI_Container class to be the only one that can have bidirectional communication
//really doesnt add much more than a second HTTP service and a data type tracker

using namespace boost::network::http;

namespace CallyxAI
{
    struct e_http;
    typedef http::server<e_http> e_server;
    std::string extern_port = "2023" //this port is used for external processors

    struct e_http
    {
        void operator()(http_server::request const & req, http_server::connection_ptr connection)
        {
            http::server::request::headers_container_type headers = request.headers();
            std::string method;
            for (auto it = headers.begin(); it != headers.end(); ++it)
            {
                if(it->first == "method")
                {
                    method = it->second;
                    it = headers.end()
                }
            }
            if(method == "POST_DOWN")
            {
                //push tokenized data downstream
            }
            else if(method == "GET_DOWN")
            {
                //push tokenized data upstream
            }
            else if(method == "REG_DPID")
            {
                //checks if ID is registered, also can serve as framework health check
                //register ID
            }

        }
    };


    class Host_Container : public AI_Container
    {
        public:
            Host_Container();
            ~Host_Container();

        private:
            e_server ext_serv;
            std::vector<opennn:type> reg_data_procs;

    };
}

#endif // C_HOST_CONTAINER_H
