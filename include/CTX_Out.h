#ifndef C_CTX_OUT_H
#define C_CTX_OUT_H

#include "AI_Core.h"

//effectively each core should assign their own queues, 
//each core should also its own logic circuit, so each 

class CTX_Out : public AI_Core
{
    public:
        CTX_Out(std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >, //node_out_down
                std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >, //node_out_src
                std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >); //node_out_up
        ~CTX_Out();

};

#endif // C_CTX_OUT_H
