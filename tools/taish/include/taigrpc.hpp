/**
 * @file    taigrpc.hpp
 *
 * @brief   This module defines TAI gRPC server
 *
 * @copyright Copyright (c) 2018 Nippon Telegraph and Telephone Corporation
 *
 * @remark  Licensed under the Apache License, Version 2.0 (the "License"); you
 *          may not use this file except in compliance with the License. You may
 *          obtain a copy of the License at
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 * @remark  THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *          CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *          LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *          FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 * @remark  See the Apache Version 2.0 License for specific language governing
 *          permissions and limitations under the License.
 *
 */

#ifndef __TAIGRPC_HPP__
#define __TAIGRPC_HPP__

#include "tai.h"
#include "tai.grpc.pb.h"
#include <grpcpp/grpcpp.h>

struct tai_api_module_t
{
    tai_object_id_t id;
    tai_object_list_t hostifs;
    tai_object_list_t netifs;
};

struct tai_api_module_list_t
{
    uint32_t count;
    tai_api_module_t *list;
};

typedef tai_status_t (*tai_api_list_module_fn)(_Inout_ tai_api_module_list_t* const list);

struct tai_api_method_table_t
{
    tai_module_api_t* module_api;
    tai_host_interface_api_t* hostif_api;
    tai_network_interface_api_t* netif_api;
    tai_api_list_module_fn list_module;
};

class TAIAPIModuleList {
    public:
        TAIAPIModuleList(uint32_t module_size = 8, uint32_t hostif_size = 2, uint32_t netif_size = 1);
        ~TAIAPIModuleList();
        tai_api_module_list_t* const list() { return &m_list; }
    private:
        tai_api_module_list_t m_list;
        uint32_t m_module_size;
        uint32_t m_hostif_size;
        uint32_t m_netif_size;
};

class TAIServiceImpl final : public tai::TAI::Service {
    public:
        TAIServiceImpl(const tai_api_method_table_t* const api) : m_api(api) {};
        ::grpc::Status ListModule(::grpc::ServerContext* context, const ::tai::ListModuleRequest* request, ::grpc::ServerWriter< ::tai::ListModuleResponse>* writer);
        ::grpc::Status ListAttributeMetadata(::grpc::ServerContext* context, const ::tai::ListAttributeMetadataRequest* request, ::grpc::ServerWriter< ::tai::ListAttributeMetadataResponse>* writer);
        ::grpc::Status GetAttributeMetadata(::grpc::ServerContext* context, const ::tai::GetAttributeMetadataRequest* request, ::tai::GetAttributeMetadataResponse* response);
        ::grpc::Status GetAttribute(::grpc::ServerContext* context, const ::tai::GetAttributeRequest* request, ::tai::GetAttributeResponse* response);
        ::grpc::Status SetAttribute(::grpc::ServerContext* context, const ::tai::SetAttributeRequest* request, ::tai::SetAttributeResponse* response);
    private:
        const tai_api_method_table_t* const m_api;
};

#endif // __TAIGRPC_HPP__
