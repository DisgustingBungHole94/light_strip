#pragma once

#include <homecontroller/net/api/api_request_maker.h>

#include <string>

class device {
    public:
        device() 
            : m_device_id("")
        {}

        ~device() {}

        void login(const std::string& username, const std::string& password);
        std::string register_device(const std::string& device_name, const std::string& device_type);

        void upgrade_connection();

        const hc::net::ssl::client_conn_ptr& get_conn_ptr() { return m_conn_ptr; }

    private:
        void add_headers(hc::http::http_request& request_ref);

        std::string m_device_id;

        hc::net::ssl::client_conn_ptr m_conn_ptr;

        std::string m_token;

        bool m_connected;
        bool m_logged_in;

        hc::http::http_parser m_http_parser;
};