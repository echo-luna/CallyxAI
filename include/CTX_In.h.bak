#ifndef C_CTX_IN_H
#define C_CTX_IN_H

#include "AI_Core.h"
//the CTX classes arent drastically different, mainly jsut allocation of the queues, and how they're processed.

class CTX_In : public AI_Core
{
    public:
        CTX_In(std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >, //node_in_up
               std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >, //node_in_src
               std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >, //main_core_in
               std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > >); //node_out_down
        ~CTX_In();

    protected:
        std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > > aux_two; //CTX_In_Down
};

#endif // C_CTX_IN_H
