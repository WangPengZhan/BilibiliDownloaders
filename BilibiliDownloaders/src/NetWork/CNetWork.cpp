#include <curl/curl.h>

#include "nlohmann/json.hpp"

#include "CNetWork.h"

namespace
{
    size_t OnWriteDate(void* data, size_t size, size_t nmemb, void* stream)
    {
        if (!stream)
        {
            return 0;
        }

        std::string* str = static_cast<std::string*>(stream);
        (*str).append(static_cast<char*>(data), size * nmemb);
        return size * nmemb;
    }

    constexpr char chrome[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36";
    constexpr char firefox[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:68.0) Gecko/20100101 Firefox/68.0";
    constexpr char edge[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299";
    constexpr char opera[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 OPR/45.0.2552.8 (EdgE)";
    constexpr char ie[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299";
    constexpr char safari[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 OPR/45.0.2552.8 (EdgE)";

    constexpr char const accept[] = "Accept: application/json";
    constexpr char const accept_encoding[] = "Accept-Encoding: gzip, deflate, br";
    constexpr char const accept_language[] = "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7";
    constexpr char const connect_type[] = "Connection: keep-alive";

    constexpr char const mainUrl[] = "https://bilibili.com/";
}

CNetWork::CNetWork()
    : m_headers(nullptr)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

CNetWork::~CNetWork()
{
    curl_global_cleanup();
    curl_slist_free_all(m_headers);
}

CNetWork& CNetWork::GetInstance()
{
    static CNetWork network;
    return network;
}

void CNetWork::HttpGet(const std::string& url, ParamType params, std::string& response)
{
    std::string strParam;
    for (const auto& param : params)
    {
        strParam += param.first + "=" + param.second + "&";
    }

    if (std::string::npos == url.find("?"))
    {
        strParam = "?" + strParam;
    }

    strParam.erase(strParam.end() -1);

    return HttpGet(url, strParam, response);
}

void CNetWork::HttpGet(const std::string& url, const std::string& params, std::string& response)
{
    return HttpGet(url + params, response);
}

void CNetWork::HttpGet(const std::string& url, std::string& response)
{
    CURL* curlHandle = curl_easy_init();
    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curlHandle, CURLOPT_HEADER, m_headers);
    curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, OnWriteDate);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(curlHandle, CURLOPT_ACCEPT_ENCODING, "gzip");
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYHOST, false);
    CURLcode retCode = curl_easy_perform(curlHandle);
    if (retCode != CURLE_OK)
    {
        // ...
    }
}

void CNetWork::HttpPost(const std::string& url, ParamType params, std::string& response)
{
    nlohmann::json jsonParams;
    
}

void CNetWork::HttpPost(const std::string& url, const std::string& params, std::string& response)
{
    CURL* curlHandle = curl_easy_init();
    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curlHandle, CURLOPT_HEADER, m_headers);
    curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, OnWriteDate);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, params.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_ACCEPT_ENCODING, "gzip");
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYHOST, false);
    CURLcode retCode = curl_easy_perform(curlHandle);
    if (retCode != CURLE_OK)
    {
        // ...
    }
}

void CNetWork::HttpPost(const std::string& url, std::string& response)
{
    CURL* curlHandle = curl_easy_init();
    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curlHandle, CURLOPT_HEADER, m_headers);
    curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, OnWriteDate);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(curlHandle, CURLOPT_ACCEPT_ENCODING, "gzip");
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYHOST, false);
    CURLcode retCode = curl_easy_perform(curlHandle);
    if (retCode != CURLE_OK)
    {
        // ...
    }
}


void CNetWork::InitHeaders()
{
    std::string userAgent = std::string("user-agent: ") + chrome;
    m_headers = curl_slist_append(m_headers, accept_encoding);
    m_headers = curl_slist_append(m_headers, accept_language);
    m_headers = curl_slist_append(m_headers, userAgent.c_str());
}


#include "loger/Loger.h"

#include <QEventLoop>

namespace
{

    const QString GetRpcUri(int listenPort = 6800)
    {
        return QString("http://localhost:%1/jsonrpc").arg(listenPort);
    }

} // namespace aria2net



AriaClient::AriaClient(QObject* parent)
    : QThread(parent)
    , m_syncNetworkAccessManager(new QNetworkAccessManager(this))
    , m_asyncNetworkAccessManager(nullptr)
{
    start();
}

AriaClient::~AriaClient()
{
    quit();
    wait();
}

QByteArray AriaClient::Request(const QString& url, const QString& parameters, int retry, ResponseType responseType)
{
    QByteArray result;

    switch (responseType)
    {
    case AriaClient::Async:
        RequestAsync(url, parameters, retry);
        break;
    case AriaClient::Sync:
        result = RequestSync(url, parameters, retry);
        break;
    default:
        break;
    }

    return result;
}

QByteArray AriaClient::RequestAsync(const QString& url, const QString& parameters, int retry)
{
    QByteArray sendData = parameters.toLocal8Bit();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, sendData.size());

    QNetworkReply* reply = m_asyncNetworkAccessManager->post(request, sendData);

    return QByteArray();
}

QByteArray AriaClient::RequestSync(const QString& url, const QString& parameters, int retry)
{
    QByteArray sendData = parameters.toLocal8Bit();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, sendData.size());


    QNetworkReply* reply = m_syncNetworkAccessManager->post(request, sendData);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);      // ��������

    return Response(reply);
}


QByteArray AriaClient::Request(const std::string& parameters, int retry)
{
    return Request(GetRpcUri(), QString::fromLocal8Bit(parameters.data()), retry);
}

QByteArray AriaClient::Response(QNetworkReply* reply)
{

    if (reply == nullptr)
    {

        return QByteArray();
    }
    if (reply->error() == QNetworkReply::OperationCanceledError)
    {
        reply->abort();
        return QByteArray();
    }
    if (reply->error() != QNetworkReply::NoError)
    {
        return QByteArray();
    }

    QByteArray rep = reply->readAll();

    reply->deleteLater();
    reply = nullptr;

    return rep;
}

void AriaClient::SetDataStream(QDataStream& dataStream)
{
    //m_asyncDataStream = dataStream;
}

void AriaClient::run()
{
    m_asyncNetworkAccessManager = new QNetworkAccessManager(this);
    connect(m_asyncNetworkAccessManager, &QNetworkAccessManager::finished, this, &AriaClient::Response);
    exec();
}