#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>
#include "json/single_include/nlohmann/json.hpp"
#include <mutex>

using boost::asio::ip::tcp;
using json = nlohmann::json;
using namespace std;

unordered_map<string, string> store;
mutex store_mutex;

string handle_request(const string& request_data) {   // processing incoming requests on your TCP server
    try {
        json request = json::parse(request_data);
        string req_type = request["request"];
        string key = request["key"];
        
        if (req_type == "read") {           //if we want read
            lock_guard<mutex> lock(store_mutex);
            
            if (store.find(key) != store.end()) {       // check if such a key exists
                return json({{"status", "ok"}, {"value", store[key]}}).dump();  // this JSON to string
            } 
            
            else {
                return json({{"status", "error"}, {"description", "Key not found"}}).dump();
            }
        } 
        
        else if (req_type == "write") {   //if we want write
            string value = request["value"];
            lock_guard<mutex> lock(store_mutex);
            store[key] = value;             // add the value "value" with the key "key" in the hash-table "store"
            return json({{"status", "ok"}}).dump();
        } 
        
        else {
            return json({{"status", "error"}, {"description", "Error request type"}}).dump();
        }
    } 
    
    catch (const json::exception& e) {
        return json({{"status", "error"}, {"description", "Uncorrect JSON"}}).dump();
    }
}

void session(tcp::socket socket) {
    try {
        boost::asio::streambuf buffer;      // buffer for data from socket. This obj accumulates data coming in parts
        boost::asio::read_until(socket, buffer, '\0');
        istream is(&buffer);                // obj "is" read data from the obj buffer
        string request_data;                
        getline(is, request_data, '\0');    // here we write data from this thread "is" to string "request_data"

        string response_data = handle_request(request_data) + '\0';     // Get the status for client (OK or Error)

        boost::asio::write(socket, boost::asio::buffer(response_data)); // Sending the data to the client in parts (asio::buffer)
    } 
    
    catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void server(boost::asio::io_context& io_context, short port) {
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        thread(session, move(socket)).detach();
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        server(io_context, 2906);
    } 
    
    catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
