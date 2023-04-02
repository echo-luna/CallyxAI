#include "AI_Core.h"


#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

using namespace CallyxAI;
AI_Core::AI_Core(std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > > s1, //main_core_in
                    std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > > s2, //node_in_src
                    std::shared_ptr<SafeQueue<std::shared_ptr<c_msg> > > s3)
                    {
                        this->core_in = s1;
                        this->core_out = s2;
                        this->aux_one = s3;
                        this->uid = generate_uid();
                        this->unique_data_sources = 0;
                        init();
                    }

AI_Core::~AI_Core()
{
}

AI_Core::AI_Core()
{
}

template<class Archive>
void const AI_Core::save(Archive & ar, const unsigned int version)
{
    std::string subdir = "cores";
    std::string filename = "cores/"+"core_"+uid+".rnn";
    #ifdef _WIN32
    int ret = _mkdir(dirname.c_str());
    #else
    int ret = mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    #endif

    FILE* file = std::fopen(filename.c_str(), "w");

    ar << BOOST_SERIALIZATION_NVP(uid);
    ar << BOOST_SERIALIZATION_NVP(unique_data_sources);

    tinyxml2::XMLPrinter c_xml_out(file);
    core_nn->write_XML(c_xml_out);
    std::fclose(file);

    ar << BOOST_SERIALIZATION_NVP(filename);
}

template<class Archive>
void AI_Core::load(Archive & ar, const unsigned int version)
{
    std::string filename;
    ar >> uid;
    ar >> unique_data_sources;
    ar >> filename;

    FILE* file = std::fopen(filename.c_str(), "r");

    tinyxml2::XMLDocument c_xml_in(file);
    core_nn->from_XML(c_xml_in);
    nn_init = true;
}

void AI_Core::init()
{
    //initialization of the actual network is going to go here
}

void AI_Core::loop()
{
    std::vector<std::shared_ptr<c_msg> > msg_log = collect();
    if(msg_log.size() > 0)
    {
        std::pair<std::shared_ptr<opennn::DataSet>,std::shared_ptr<opennn::DataSet> > ds_pair = compile(msg_log);
        Tensor<opennn::type, 2> res = predict(ds_pair.first);
        msg_log = decompile(res);
        train(ds_pair.second());
        for(auto m = msg_log->begin(); m != msg_log->end(); ++m)
        {
            core_out->push(*m);
            aux_one->push(*m);
        }
    }
}

std::vector<std::shared_ptr<c_msg> > AI_Core::collect()
{
    std::vector<std::shared_ptr<c_msg> > vec;
    for(int i = 0; i < 5; i++)
    {
        if(core_in->size() > 0)
        {
            msg_log.push_back(core_in->get_front());
        }
    }
    return vec;
}

std::pair<Tensor<opennn::type, std::shared_ptr<opennn::DataSet> > AI_Core::compile(std::vector<std::shared_ptr<c_msg> > vec)
{
    std::vector<Tensor<opennn::type, 1> > unproc_sep;
    std::vector<Tensor<opennn::type, 1> > proc_sep;
    int proc_size = 0;

    for(auto m = vec->begin(); m != vec->end(); ++m)
    {
        Tensor<opennn::type, 1> unproc_tensor = tensor_from_vec(*m->get_unproccessed_vector());
        Tensor<opennn::type, 1> proc_tensor = tensor_from_vec(*m->get_proccessed_vector());
        if(*m->get_unproccessed_vector().size() > proc_size)
        {
            proc_size = *m->get_unproccessed_vector().size()
        }
        if(*m->get_proccessed_vector().size() > proc_size)
        {
            proc_size = *m->get_proccessed_vector().size()
        }
    }

    Eigen::Tensor<opennn::type, 2> unproc_tensor(vec.size(), unproc_size);
    for (int i = 0; i < unproc_sep.size(); ++i)
    {
        Tensor<opennn::type, 1> t1 = unproc_sep.at(i);
        for(int j = 0; j < t1.size(); j++)
        {
            unproc_tensor(i,j) = t1(j);
        }
    }
    Eigen::Tensor<opennn::type, 2> proc_tensor(vec.size() * 2, proc_size);
    int oi = 0;
    for (int i = 0; i < proc_sep.size(); ++i)
    {
        Tensor<opennn::type, 1> t1 = proc_sep.at(i);
        Tensor<opennn::type, 1> t2 = unproc_sep.at(i);
        for(int j = 0; j < t1.size(); j++)
        {
            proc_tensor(oi,j) = t1(j);
            oi++;
            proc_tensor(oi,j) = t1(j);
            oi++;
        }
    }

    std::pair<std::shared_ptr<opennn::DataSet>,std::shared_ptr<opennn::DataSet> > ds_pair;
    //make sure to set the inputs/outputs on this dataset...alternate, input, output, input, output, should always be in pairs if formatted correctly
    std::shared_ptr<opennn::DataSet> ds_2(new openn::DataSet(proc_tensor));
    for(int i = 0; i < ds_2->get_columns_number(); i++)
    {
        ds_2->set_column_use(i, opennn::DataSet::VariableUse::)
    }
    ds_pair.first = unproc_tensor;
    ds_pair.second = ds_2;

    return ds_pair;
}

Tensor<opennn::type, 2> AI_Core::predict(Tensor<opennn::type, 2> data)
{
    return core_nn.calculate_outputs(data);
}

std::pair<opennn::TrainingResults,std::shared_ptr<opennn::DataSet> >  AI_Core::train(std::shared_ptr<opennn::DataSet> ds)
{
    //trains data with the recurrent data (if working correctly)
    opennn::TrainingStrategy strat(core_nn, ds.get())
    strat.set_loss_method(opennn::TrainingStrategy::LossMethod::NORMALIZED_SQUARED_ERROR);
    strat.set_optimization_method(opennn::TrainingStrategy::OptimizationMethod::QUASI_NEWTON_METHOD);
    strat.set_maximum_epochs_number(1000);
    strat.set_maximum_selection_failures(500);
    std::pair<opennn::TrainingResults,std::shared_ptr<opennn::DataSet> > ret;
    if(nn_init != true){return ret}
    ret.first = strat.perform_training();
    ret.second = ds;
    return ret;
}

std::vector<std::shared_ptr<c_msg> > AI_Core::decompile(Tensor<opennn::type, 2> data)
{
    std::vector<std::shared_ptr<c_msg> > vec;
    for(int c = 0; c < data.dimension(1); c++)
    {
        std::vector<opennn::type> temp1;\
        std::vector<opennn::type> temp2;
        for(int r = 0; r < data.dimension(0); r++)
        {
            temp1.push_back(data(r,c));
            temp2.push_back(data(r,c+1));
        }
        c++; //lol
        std::shared_ptr<c_msg> temp_msg(new c_msg(temp_1.at(0))); //if formatted correctly,  the first piece of data should always be the UID of the data source
        temp_msg->set_unprocessed_vector(temp1);
        temp_msg->set_processed_vector(temp2);
        vec.push_back(temp_msg);
    }
    return vec;
}
