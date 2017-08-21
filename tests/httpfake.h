#ifndef HTTPFAKE_H_
#define HTTPFAKE_H_

#include "json/json.h"

#include "httpinterface.h"

const std::string test_manifest = "/tmp/test_aktualizr_manifest.txt";
const std::string tls_server = "https://tlsserver.com";
const std::string metadata_path = "tests/test_data";

enum ProvisioningResult { ProvisionOK, ProvisionFailure };

class HttpFake : public HttpInterface {
 public:
  HttpFake(const std::string test_dir_in) : provisioningResponse(ProvisionOK), test_dir(test_dir_in) {}

  ~HttpFake() { boost::filesystem::remove(metadata_path + "/repo/timestamp.json"); }

  void setCerts(const std::string &ca, const std::string &cert, const std::string &pkey) {
    (void)ca;
    (void)cert;
    (void)pkey;
  }

  HttpResponse get(const std::string &url) {
    std::cout << "URL:" << url << "\n";
    if (url.find(tls_server) == 0) {
      std::string path = metadata_path + url.substr(tls_server.size());
      std::cout << "filetoopen: " << path << "\n\n\n";
      if (url.find("timestamp.json") != std::string::npos) {
        std::cout << "CHECK PATH: " << metadata_path + "/timestamp.json\n";
        if (boost::filesystem::exists(path)) {
          boost::filesystem::copy_file("tests/test_data/timestamp2.json", path,
                                       boost::filesystem::copy_option::overwrite_if_exists);
        } else {
          boost::filesystem::copy_file("tests/test_data/timestamp1.json", path,
                                       boost::filesystem::copy_option::overwrite_if_exists);
        }
        return HttpResponse(Utils::readFile(path), 200, CURLE_OK, "");
      } else if (url.find("targets.json") != std::string::npos) {
        Json::Value timestamp = Utils::parseJSONFile(metadata_path + "/repo/timestamp.json");
        if (timestamp["signed"]["version"].asInt64() == 2) {
          return HttpResponse(Utils::readFile("tests/test_data/targets_noupdates.json"), 200, CURLE_OK, "");
        } else {
          return HttpResponse(Utils::readFile("tests/test_data/targets_hasupdates.json"), 200, CURLE_OK, "");
        }
      } else {
        return HttpResponse(Utils::readFile("tests/test_data/" + url.substr(tls_server.size())), 200, CURLE_OK, "");
      }
    }
    return HttpResponse(url, 200, CURLE_OK, "");
  }

  HttpResponse post(const std::string &url, const Json::Value &data) {
    (void)url;

    Utils::writeFile(test_dir + "/post.json", data);
    if (provisioningResponse == ProvisionOK) {
      return HttpResponse(Utils::readFile("tests/test_data/cred.p12"), 200, CURLE_OK, "");
    } else {
      return HttpResponse("", 400, CURLE_OK, "");
    }
  }

  HttpResponse put(const std::string &url, const Json::Value &data) {
    std::ofstream director_file(test_manifest.c_str());
    director_file << data;
    director_file.close();
    return HttpResponse(url, 200, CURLE_OK, "");
  }

  HttpResponse download(const std::string &url, curl_write_callback callback, void *userp) {
    (void)callback;
    (void)userp;
    std::cout << "URL: " << url << "\n";
    std::string path = test_dir + "/" + url.substr(url.rfind("/targets/") + 9);
    std::cout << "filetoopen: " << path << "\n\n\n";

    std::string content = Utils::readFile(path);

    // Hack since the signature strangely requires non-const.
    callback(const_cast<char *>(content.c_str()), content.size(), 1, userp);
    return HttpResponse(content, 200, CURLE_OK, "");
  }

  ProvisioningResult provisioningResponse;

 private:
  /**
   * These are here to catch a common programming error where a Json::Value is
   * implicitly constructed from a std::string. By having an private overload
   * that takes string (and with no implementation), this will fail during
   * compilation.
   */
  HttpResponse post(const std::string &url, const std::string data);
  HttpResponse put(const std::string &url, const std::string data);

  std::string test_dir;
};

#endif  // HTTPFAKE_H_
