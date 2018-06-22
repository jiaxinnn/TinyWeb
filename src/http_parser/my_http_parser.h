/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.generalsandman.cn
 */

/*---class HttpParser---
 *
 ****************************************
 *
 */

#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

// #include <tiny_base/log.h>

#include "http.h"

#include <iostream>
#include <stdio.h>
#include <map>
#include <list>
#include <boost/function.hpp>

#define CR '\r'
#define LF '\n'
#define isNum(c) ('0' <= (c) && (c) <= '9')
#define isAlpha(c) ('a' <= (c) && (c) <= 'z' || 'A' <= (c) && (c) <= 'Z')
#define isAlphaNum(c) (isNum(c) || isAlpha(c))

#define isLower(c) ('a' <= (c) && (c) <= 'z')
#define isUpper(c) ('A' <= (c) && (c) <= 'Z')

#define toLower(c) isUpper(c) ? (unsigned char)((c) | 0x20) : c
#define toUpper(c) isLower(c) ? (unsigned char)((c)&0xdf) : c

#define isHexChar(c) (isNum(c) || ('a' <= toLower(c) && toLower(c) <= 'f'))

#define isMarkChar(c) ((c) == '-' || (c) == '_' || (c) == '.' ||                              \
					   (c) == '!' || (c) == '~' || (c) == '*' || (c) == '\'' || (c) == '(' || \
					   (c) == ')')
#define isUserInfoChar(c) (isAlphaNum(c) || isMarkChar(c) || (c) == '%' ||                       \
						   (c) == ';' || (c) == ':' || (c) == '&' || (c) == '=' || (c) == '+' || \
						   (c) == '$' || (c) == ',')

#define isHostChar(c) (isAlphaNum(c) || (c) == '.' || (c) == '-' || (c) == '_')

#define isIpv4Char(c) (isNum(c) || (c) == '.')
#define isIpv6Char(c) (isHexChar(c) || (c) == ':' || (c) == '.')

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define getHash(hash, c) ((unsigned long long)((hash)*27 + (c)))

inline short int getHex(char c)
{
	if ('0' <= c && c <= '9')
		return (c - '0');
	else if ('A' <= c && c <= 'F')
		return (c - 'A' + 10);
	else if ('a' <= c && c <= 'f')
		return (c - 'a' + 10);
	return -1;
}

#define strcmp_3(m, s) ((*(m + 0) == *(s + 0)) && \
						(*(m + 1) == *(s + 1)) && \
						(*(m + 2) == *(s + 2)))

#define strcmp_4(m, s) ((*(m + 0) == *(s + 0)) && \
						(*(m + 1) == *(s + 1)) && \
						(*(m + 2) == *(s + 2)) && \
						(*(m + 3) == *(s + 3)))

#define strcmp_5(m, s) ((*(m + 0) == *(s + 0)) && \
						(*(m + 1) == *(s + 1)) && \
						(*(m + 2) == *(s + 2)) && \
						(*(m + 3) == *(s + 3)) && \
						(*(m + 4) == *(s + 4)))

#define strcmp_6(m, s) ((*(m + 0) == *(s + 0)) && \
						(*(m + 1) == *(s + 1)) && \
						(*(m + 2) == *(s + 2)) && \
						(*(m + 3) == *(s + 3)) && \
						(*(m + 4) == *(s + 4)) && \
						(*(m + 5) == *(s + 5)))

#define strcmp_7(m, s) ((*(m + 0) == *(s + 0)) && \
						(*(m + 1) == *(s + 1)) && \
						(*(m + 2) == *(s + 2)) && \
						(*(m + 3) == *(s + 3)) && \
						(*(m + 4) == *(s + 4)) && \
						(*(m + 5) == *(s + 5)) && \
						(*(m + 6) == *(s + 6)))

#define strcmp_8(m, s) ((*(m + 0) == *(s + 0)) && \
						(*(m + 1) == *(s + 1)) && \
						(*(m + 2) == *(s + 2)) && \
						(*(m + 3) == *(s + 3)) && \
						(*(m + 4) == *(s + 4)) && \
						(*(m + 5) == *(s + 5)) && \
						(*(m + 6) == *(s + 6)) && \
						(*(m + 7) == *(s + 7)))

#define strcmp_9(m, s) ((*(m + 0) == *(s + 0)) && \
						(*(m + 1) == *(s + 1)) && \
						(*(m + 2) == *(s + 2)) && \
						(*(m + 3) == *(s + 3)) && \
						(*(m + 4) == *(s + 4)) && \
						(*(m + 5) == *(s + 5)) && \
						(*(m + 6) == *(s + 6)) && \
						(*(m + 7) == *(s + 7)) && \
						(*(m + 8) == *(s + 8)))

//the values set to HttpParser::m_nType
enum httpParserType
{
	HTTP_REQUEST = 1,
	HTTP_RESPONSE,
	HTTP_BOTH
};

enum httpUrlField
{
	HTTP_UF_SCHEMA = 0,
	HTTP_UF_HOST = 1,
	HTTP_UF_PORT = 2,
	HTTP_UF_PATH = 3,
	HTTP_UF_QUERY = 4,
	HTTP_UF_FRAGMENT = 5,
	HTTP_UF_USERINFO = 6,
	HTTP_UF_MAX = 7
};

enum state
{
	s_error = 1,

	s_start_resp_or_requ,

	s_resp_start,
	s_resp_H,
	s_resp_HT,
	s_resp_HTT,
	s_resp_HTTP,
	s_resp_HTTP_slash,
	s_resp_version_major,
	s_resp_version_dot,
	s_resp_version_minor,
	s_resp_status_code_start,
	s_resp_status_code,
	s_resp_status_phrase_start,
	s_resp_status_phrase,
	s_resp_line_almost_done,
	s_resp_line_done,

	s_requ_start,
	s_requ_method_start,
	s_requ_method,
	s_requ_url_begin,
	s_requ_url, //add
	s_requ_schema,
	s_requ_schema_slash,
	s_requ_schema_slash_slash,
	s_requ_server_start,
	s_requ_server,
	s_requ_path,
	s_requ_server_at,
	s_requ_query_string_start,
	s_requ_query_string,
	s_requ_fragment_start,
	s_requ_fragment,

	s_requ_HTTP_start,
	s_requ_H,
	s_requ_HT,
	s_requ_HTT,
	s_requ_HTTP,
	s_requ_HTTP_slash,
	s_requ_version_major,
	s_requ_version_dot,
	s_requ_version_minor,
	s_requ_line_almost_done,
	s_requ_line_done,

	//heaser statue
	s_header_start,
	s_header,
	s_header_almost_done,
	s_header_done,
	s_headers_almost_done,
	s_headers_done,

	s_body_start,
	s_body,
	s_body_done,

	s_chunk

};

enum http_host_state
{
	s_http_host_error = 1,
	s_http_userinfo_start,
	s_http_userinfo,
	s_http_host_start,
	s_http_host_v6_start,
	s_http_host_v4,
	s_http_host_v6,
	s_http_host_v6_end,
	s_http_host_v6_zone_start,
	s_http_host_v6_zone,
	s_http_host_port_start,
	s_http_host_port
};

enum http_header_state
{
	s_http_header_error = 1,

	s_http_header_start,
	s_http_header_key_start,
	s_http_header_key,
	s_http_header_colon,
	s_http_header_space,
	s_http_header_value_start,
	s_http_header_value,
	s_http_header_almost_done,
	s_http_header_done,
	s_http_headers_almost_done,
	s_http_headers_done,
};

enum http_body_state
{
	s_http_body_error = 1,

	s_http_body_identify_by_length,
	s_http_body_identify_by_eof,

	s_http_body_chunk_size_start,
	s_http_body_chunk_size,
	s_http_body_chunk_size_almost_done,
	s_http_body_chunk_size_done,

	s_http_body_chunk_data,
	s_http_body_chunk_data_almost_done,
	s_http_body_chunk_data_done,
	s_http_body_chunks_done
};

typedef struct Url
{
	char *data;
	unsigned int offset;
	unsigned int len;

	unsigned int port : 16;
	unsigned int field_set : 16;

	struct
	{
		unsigned int offset : 16;
		unsigned int len : 16;
	} fields[HTTP_UF_MAX];
} Url;

enum httpHeaderField
{
	HTTP_HF__SCHEMA = 0,
	HTTP_HF__HOST = 1,
	HTTP_HF_KEEP_CONNECTION = 2,
	HTTP_HF_CONTENT_LENGTH = 3,
	HTTP_HF_UPGRADE = 4,
	HTTP_HF_TRANSFER_ENCODING_CHUNKED = 5,
	HTTP_HF__USERINFO = 6,
	HTTP_HF__MAX = 7
};

typedef struct HttpHeader
{
	int keyHash;
	std::string key; //TODO:take place with string_t
	std::string value;
} HttpHeader;

typedef struct HttpHeaders
{
	char *data;
	unsigned int offset;
	unsigned int len;

	HttpHeader *host;
	HttpHeader *connection;
	HttpHeader *if_modified_since;
	HttpHeader *if_unmodified_since;
	HttpHeader *user_agent;
	HttpHeader *referer;

	HttpHeader *content_lenght;
	HttpHeader *content_type;
	HttpHeader *transfer_encoding;
	HttpHeader *accept_encoding;

	HttpHeader *upgrade;
	HttpHeader *expect;

	std::list<HttpHeader *> generals; //take place in list_t
} HttpHeaders;

typedef struct HttpBody
{
	void *data;
	unsigned int offset;
	unsigned int len;
} HttpBody;

void printHttpHeaders(const HttpHeaders *headers);

void printUrl(const Url *url);

#include <memory>
typedef struct HttpRequest
{
	unsigned short method : 8;
	unsigned short http_version_major : 8;
	unsigned short http_version_minor : 8;

	std::string method_s;

	unsigned short headerNum;

	// std::unique_ptr<Url> url;
	// std::shared_ptr<HttpHeaders> headers;
	// std::shared_ptr<HttpBody> body;
	//FIXME:There is great possiblility of memory lacking.
	Url *url;
	HttpHeaders *headers;
	HttpBody *body;

} HttpRequest;

typedef boost::function<int()> HttpCallback;
typedef boost::function<int()> HttpDataCallback;

class HttpParserSettings
{
  private:
	std::map<std::string, HttpCallback> m_nReflection;

	HttpCallback m_fGetMessage;
	HttpCallback m_fGetUrl;
	HttpCallback m_fGetStatus;
	HttpCallback m_fGetRequestLine;

	HttpCallback m_fGetHeader;
	HttpCallback m_fHeaderKey;
	HttpCallback m_fHeaderValue;

	HttpCallback m_fGetBody;
	HttpCallback m_fEndMessage;

	HttpCallback m_fGetChunk;
	HttpCallback m_fEndChunk;

  public:
	HttpParserSettings()
	{
		// std::cout << "class HttpParserSettings constructor\n";
	}

	void setGetMessageCallback(const HttpCallback &c)
	{
		m_fGetMessage = c;
		m_nReflection["getMessage"] = c;
	}

	void setGetRequestLineCallback(const HttpCallback &c)
	{
		m_fGetRequestLine = c;
		m_nReflection["getRequestLine"] = c;
	}

	void setGetHeaderCallback(const HttpCallback &c)
	{
		m_fGetHeader = c;
		m_nReflection["getHeader"] = c;
	}

	void setGetBodyCallback(const HttpCallback &c)
	{
		m_fGetBody = c;
		m_nReflection["getBody"] = c;
	}

	void setGetEndMessageCallback(const HttpCallback &c)
	{
		m_fEndMessage = c;
		m_nReflection["endMessage"] = c;
	}

	HttpCallback getMethodByName(const std::string &fname)
	{
		auto p = m_nReflection.find(fname);
		// if (p == m_nReflection.end())
		// 	return nullptr;
		// else
		return m_nReflection[fname];
		//FIXME:
	}

	~HttpParserSettings()
	{
		// std::cout << "class HttpParserSettings destructor\n";
	}

	friend class HttpParser;
};

class HttpParser
{
  private:
	HttpParserSettings *m_pSettings;

	unsigned int m_nType : 2; //Http request or response
	unsigned int m_nFlags : 8;
	unsigned int m_nState;
	unsigned int m_nHeaderState;

	unsigned long long m_nRead;
	unsigned long long m_nContentLength;

	unsigned short m_nHttpVersionMajor;
	unsigned short m_nHttpVersionMinor;
	unsigned int m_nStatusCode : 16;
	unsigned int m_nMethod : 8;
	unsigned int m_nErrno : 7;
	unsigned int m_nIsUpgrade : 1;

	void *m_pData;

  public:
	HttpParser(HttpParserSettings *set = nullptr)
		: m_pSettings(set),
		  m_nType(0), //FIXME:
		  m_nFlags(0),
		  m_nState(0),
		  m_nHeaderState(0),
		  m_nRead(0),
		  m_nContentLength(0),
		  m_nHttpVersionMajor(0),
		  m_nHttpVersionMinor(0),
		  m_nStatusCode(0),
		  m_nMethod(0),
		  m_nErrno(0),
		  m_nIsUpgrade(0),
		  m_pData(nullptr)
	{
		// std::cout << "class HttpParser constructor\n";
	}

	void setType(enum httpParserType type);

	enum http_errno getErrno()
	{
		return (enum http_errno)m_nErrno;
	}

	int invokeByName(const char *funName,
					 const char *data,
					 unsigned int len);

	//parse host
	enum http_host_state parseHostChar(const char ch,
									   enum http_host_state s);
	int parseHost(const char *stream,
				  int at,
				  int len,
				  Url *&result,
				  bool has_at_char);

	//parse url
	enum state parseUrlChar(const char ch,
							enum state s);
	int parseUrl(const char *stream,
				 int at,
				 int len,
				 Url *result);

	//parse header
	enum http_header_state parseHeaderChar(const char ch,
										   enum http_header_state s);
	int parseHeader(const char *stream,
					int at,
					int len,
					HttpHeaders *result);
	int parseHeaders(const char *stream,
					 int at,
					 int len,
					 HttpHeaders *result);

	//parse body
	int parseBody(const char *stream,
				  int at,
				  int len,
				  bool isChunked);

	inline enum http_method getMethod(const char *begin,
									  unsigned short len)
	{
		switch (len)
		{
		case 3:
		{
			if (strcmp_3(begin, "GET"))
				return HTTP_METHOD_GET;

			if (strcmp_3(begin, "PUT"))
				return HTTP_METHOD_PUT;

			if (strcmp_3(begin, "ACL"))
				return HTTP_METHOD_ACL;
		}
		break;

		case 4:
		{
			if (strcmp_4(begin, "HEAD"))
				return HTTP_METHOD_HEAD;

			if (strcmp_4(begin, "POST"))
				return HTTP_METHOD_POST;

			if (strcmp_4(begin, "COPY"))
				return HTTP_METHOD_COPY;

			if (strcmp_4(begin, "LOCK"))
				return HTTP_METHOD_LOCK;

			if (strcmp_4(begin, "MOVE"))
				return HTTP_METHOD_MOVE;

			if (strcmp_4(begin, "BIND"))
				return HTTP_METHOD_BIND;

			if (strcmp_4(begin, "LINK"))
				return HTTP_METHOD_LINK;
		}
		break;

		case 5:
		{
			if (strcmp_5(begin, "TRACE"))
				return HTTP_METHOD_TRACE;

			if (strcmp_5(begin, "MKCOL"))
				return HTTP_METHOD_MKCOL;

			if (strcmp_5(begin, "MERGE"))
				return HTTP_METHOD_MERGE;

			if (strcmp_5(begin, "PATCH"))
				return HTTP_METHOD_PATCH;

			if (strcmp_5(begin, "PURGE"))
				return HTTP_METHOD_PURGE;
		}
		break;

		case 6:
		{
			if (strcmp_6(begin, "DELETE"))
				return HTTP_METHOD_DELETE;

			if (strcmp_6(begin, "SEARCH"))
				return HTTP_METHOD_SEARCH;

			if (strcmp_6(begin, "UNLOCK"))
				return HTTP_METHOD_UNLOCK;

			if (strcmp_6(begin, "REBIND"))
				return HTTP_METHOD_REBIND;

			if (strcmp_6(begin, "UNBIND"))
				return HTTP_METHOD_UNBIND;

			if (strcmp_6(begin, "REPORT"))
				return HTTP_METHOD_REPORT;

			if (strcmp_6(begin, "NOTIFY"))
				return HTTP_METHOD_NOTIFY;

			if (strcmp_6(begin, "UNLINK"))
				return HTTP_METHOD_UNLINK;

			if (strcmp_6(begin, "SOURCE"))
				return HTTP_METHOD_SOURCE;
		}
		break;

		case 7:
		{
			if (strcmp_7(begin, "CONNECT"))
				return HTTP_METHOD_CONNECT;

			if (strcmp_7(begin, "OPTIONS"))
				return HTTP_METHOD_OPTIONS;

			if (strcmp_7(begin, "MSEARCH"))
				return HTTP_METHOD_MSEARCH;
		}
		break;

		case 8:
		{
			if (strcmp_8(begin, "PROPFIND"))
				return HTTP_METHOD_PROPFIND;

			if (strcmp_8(begin, "CHECKOUT"))
				return HTTP_METHOD_CHECKOUT;
		}
		break;

		case 9:
		{
			if (strcmp_9(begin, "PROPPATCH"))
				return HTTP_METHOD_PROPPATCH;

			if (strcmp_9(begin, "CHECKOUT"))
				return HTTP_METHOD_CHECKOUT;

			if (strcmp_9(begin, "SUBSCRIBE"))
				return HTTP_METHOD_SUBSCRIBE;
		}
		break;
		}
	}

	int execute(const char *stream,
				int at,
				int len,
				HttpRequest *request);

	~HttpParser()
	{
		// std::cout << "class HttpParser destructor\n";
	}
};

#endif