/**
  * @brief : This head file is used to get configure message from a *.pb.txt file to a proto
  * type configure value
  * @note  :
  * @param :
  * @return:
  */

#ifndef PBCONFIG_H_
#define PBCONFIG_H_

#include <fcntl.h>
#include <stdio.h>

#include <unistd.h>
#include <cstdio>

#include <fstream>
#include <string>

#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/text_format.h"


template<typename CfgMsgType>
class Configurer
{
public:
    Configurer() {}

    bool GetPbMsgFromCfgFile(CfgMsgType* cfg_msg, const std::string& cfg_file){
        return GetPbMsgFromASCIIFile(cfg_msg,cfg_file) ||
                GetPbMsgFromBinaryFile(cfg_msg,cfg_file);
    }


    bool SetPbMsgToASCIIFile(const CfgMsgType &message, const std::string &file_name) {
        int fd = open(file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        if (fd < 0) {
            std::cout << "Unable to open file " << file_name << " to write.";
            return false;
        }
        return SetPbMsgToASCIIFile(message, fd);
    }

    bool SetPbMsgToBinaryFile(const CfgMsgType &message,const std::string &file_name) {
        std::fstream output(file_name,
                            std::ios::out | std::ios::trunc | std::ios::binary);
        return message.SerializeToOstream(&output);
    }

private:

    bool GetPbMsgFromASCIIFile(CfgMsgType* cfg_msg, const std::string& cfg_file){
        using google::protobuf::TextFormat;
        using google::protobuf::io::FileInputStream;
        using google::protobuf::io::ZeroCopyInputStream;

        int file_descriptor = open(cfg_file.c_str(),O_RDONLY);
        if (file_descriptor < 0){
            return false;
        }

        ZeroCopyInputStream *input = new FileInputStream(file_descriptor);
        bool success = TextFormat::Parse(input,cfg_msg);
        if(!success){
            std::cout << "Failed to parse file " << cfg_file << " as text proto." << std::endl;
        }

        delete input;
        close(file_descriptor);
        return success;
    }

    bool GetPbMsgFromBinaryFile(CfgMsgType* cfg_msg, const std::string& cfg_file){
        std::fstream input(cfg_file,std::ios::in | std::ios::binary);
        if(!input.good()){
            std::cout << "Failed to open configure file:" << cfg_file << " in binary mode." << std::endl;
            return false;
        }

        if(!cfg_msg->ParseFromIstream(&input)){
            std::cout << "Failed to parse configure file:" << cfg_file << " as binary proto." << std::endl;
            return false;
        }

        return true;
    }

    bool SetPbMsgToASCIIFile(const CfgMsgType &message, int file_descriptor) {
        using google::protobuf::TextFormat;
        using google::protobuf::io::FileOutputStream;
        using google::protobuf::io::ZeroCopyOutputStream;
        if (file_descriptor < 0) {
            std::cout << "Invalid file descriptor.";
            return false;
        }
        ZeroCopyOutputStream *output = new FileOutputStream(file_descriptor);
        bool success = TextFormat::Print(message, output);
        delete output;
        close(file_descriptor);
        return success;
    }
};



#endif
