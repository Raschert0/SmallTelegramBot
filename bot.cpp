#include <fstream>
#include <thread>
#include <chrono>
#include <string>

#include <cstdlib>
#include <cerrno>

#include "json.hpp"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

using json = nlohmann::json;

std::string bot_key;

int getLatestUpdateId()
{
	try{
		curlpp::Easy request;
		std::list<std::string> header;
		request.setOpt(new curlpp::options::Url("https://api.telegram.org/bot"+bot_key+"/getUpdates"));

                curlpp::Forms formParts;
                formParts.push_back(new curlpp::FormParts::Content("limit", "1"));

                request.setOpt(new curlpp::options::HttpPost(formParts));

                std::stringstream ss;
                ss << request;

                json j_complete = json::parse(ss);
				if(!j_complete["result"][0].is_null()){
					return  j_complete["result"][0]["update_id"].get<int>();
				}else{
					return 1;
				}
	}
        catch ( json::exception & e ) {
                std::cout << "message: " << e.what() << '\n'<< "exception id: " << e.id << std::endl;
        }
        catch ( curlpp::LogicError & e ) {
                std::cout << e.what() << std::endl;
        }
        catch ( curlpp::RuntimeError & e ) {
                std::cout << e.what() << std::endl;
        }
	return 0;
}

int main()
{
	try{
		curlpp::Cleanup cleaner;
		std::ifstream bot_key_file("./bot_key");
		bot_key_file >> bot_key;

		int latest_id = getLatestUpdateId() + 1;
		curlpp::Easy request;
		request.setOpt(new curlpp::options::Url("https://api.telegram.org/bot"+bot_key+"/getUpdates"));
		curlpp::Forms formParts;
		for(;;)
		{
			formParts.push_back(new curlpp::FormParts::Content("offset", std::to_string(latest_id)));
			request.setOpt(new curlpp::options::HttpPost(formParts));

			std::stringstream ss;
			ss << request;
			json j_complete = json::parse(ss);
			if(!j_complete.empty() && !j_complete["result"][0].is_null()){
				if(!j_complete["result"][0]["message"]["text"].is_null() && j_complete["result"][0]["message"]["text"].get<std::string>().find("/pidor") != std::string::npos){
					int reply_to = j_complete["result"][0]["message"]["chat"]["id"].get<int>();
					std::string reply = "You, ";
					if(!j_complete["result"][0]["message"]["from"]["username"].is_null()){
						reply += "@";
						reply += j_complete["result"][0]["message"]["from"]["username"];
					}else{
						reply += j_complete["result"][0]["message"]["from"]["first_name"];
						if(!j_complete["result"][0]["message"]["from"]["last_name"].is_null()){
							reply += " ";
							reply += j_complete["result"][0]["message"]["from"]["last_name"];
						}
					}
					reply += ".";
					curlpp::Easy respone;
					respone.setOpt(new curlpp::options::Url("https://api.telegram.org/bot"+bot_key+"/sendMessage"));
					curlpp::Forms formPartsResponse;
					formPartsResponse.push_back(new curlpp::FormParts::Content("chat_id", std::to_string(reply_to)));

					formPartsResponse.push_back(new curlpp::FormParts::Content("text", reply));
					respone.setOpt(new curlpp::options::HttpPost(formPartsResponse));
					std::ofstream myfile("/dev/null");
					myfile << respone << std::endl;
				}
				latest_id = j_complete["result"][0]["update_id"].get<int>() + 1;
			}

			formParts.clear();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	catch ( json::exception & e ) {
		std::cout << "message: " << e.what() << '\n'<< "exception id: " << e.id << std::endl;
	}
	catch ( curlpp::LogicError & e ) {
		std::cout << e.what() << std::endl;
	}
	catch ( curlpp::RuntimeError & e ) {
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
