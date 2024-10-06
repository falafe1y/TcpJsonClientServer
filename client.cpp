#include <iostream>
#include <boost/asio.hpp>
#include <algorithm>
#include "json/single_include/nlohmann/json.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;

using namespace std;

void send_request(const json& request) {
    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(socket, resolver.resolve("127.0.0.1", "2906"));

    string request_data = request.dump() + '\0';
    boost::asio::write(socket, boost::asio::buffer(request_data));

    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, '\0');
    istream is(&buffer);
    string response_data;
    getline(is, response_data, '\0');

    json response = json::parse(response_data);
    cout << "\nResponse: " << response.dump(4) << endl;
}

int main() {
    string command;
    while (true) {
        cout << "Enter the command (read/write): ";
        cin >> command;
        transform(command.begin(), command.end(), command.begin(), ::tolower);      // command to lower case

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (command == "read") {
            string key;
            cout << "Enter key: ";
            cin >> key;

            json request = {{"request", "read"}, {"key", key}};
            send_request(request);
        } 
        
        else if (command == "write") {
            string key, value;
            cout << "Enter key: ";
            getline(cin, key);
            cout << "Enter value: ";
            getline(cin, value);    // read value with spaces

            json request = {{"request", "write"}, {"key", key}, {"value", value}};
            send_request(request);
        } 
        
        else {
            cout << "Unknown command." << endl;
        }
    }

    return 0;
}
