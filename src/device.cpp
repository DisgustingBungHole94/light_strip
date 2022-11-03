#include "device.h"

#include <homecontroller/net/endpoints.h>
#include <homecontroller/exception/exception.h>

#include <homecontroller/api/json/request/login_api_request.h>
#include <homecontroller/api/json/response/login_api_response.h>

#include <homecontroller/api/json/request/register_device_api_request.h>
#include <homecontroller/api/json/response/register_device_api_response.h>

#include <iostream>

void device::connect() {
    hc::net::ssl::tls_client client;
    client.init();

    m_conn_ptr = client.connect(hc::net::endpoints::API_HOST, hc::net::endpoints::API_PORT);
    m_connected = true;
}

void device::disconnect() {
    if (m_connected) {
        m_conn_ptr->close();
        m_connected = false;
    }
}

void device::login(const std::string& username, const std::string& password) {
    if (!m_connected) {
        throw hc::exception("not initialized", "device::login");
    }
    
    hc::api::json::login_api_request req_json;
    req_json.set_username(username);
    req_json.set_password(password);

    hc::http::http_request http_req("POST", "/login", req_json.str());
    add_headers(http_req);

    m_conn_ptr->send(http_req.str());
    std::string response = m_conn_ptr->recv();

    m_http_parser.init(hc::http::http_parser::parser_type::RESPONSE);
    m_http_parser.parse(response);

    hc::api::json::login_api_response res_json;
    if (!res_json.parse(m_http_parser.get_response().get_body())) {
        throw hc::exception(res_json.get_error(), "device::login");
    }

    m_token = res_json.get_token();
    m_logged_in = true;
}

std::string device::register_device(const std::string& device_name, const std::string& device_type) {
    if (!m_connected || !m_logged_in) {
        throw hc::exception("not initialized", "device::register_device");
    }

    hc::api::json::register_device_api_request req_json;
    req_json.set_device_name(device_name);
    req_json.set_device_type(device_type);

    hc::http::http_request http_req("POST", "/register_device", req_json.str());
    add_headers(http_req);

    m_conn_ptr->send(http_req.str());
    std::string response = m_conn_ptr->recv();

    m_http_parser.parse(response);

    hc::api::json::register_device_api_response res_json;
    if (!res_json.parse(m_http_parser.get_response().get_body())) {
        throw hc::exception(res_json.get_error(), "device::register_device");
    }

    m_device_id = res_json.get_device_id();
    return m_device_id;
}

void device::upgrade_connection() {
    if (!m_connected || !m_logged_in) {
        throw hc::exception("not initialized", "device::upgrade_connection");
    }
}

void device::add_headers(hc::http::http_request& request_ref) {
    request_ref.add_header("Host", hc::net::endpoints::API_HOST);
    request_ref.add_header("Content-Length", std::to_string(request_ref.get_body().length()));
    request_ref.add_header("Content-Type", "application/json");
    request_ref.add_header("Connection", "keep-alive");

    if (m_logged_in) {
        request_ref.add_header("Authorization", "HomeController " + m_token);
    }
}