#include "strip.hpp"

#include <cctype>   // for isspace
#include <iostream> // for debugging

#define COMMENT_TYPE_NONE   0
#define COMMENT_TYPE_SINGLE 1
#define COMMENT_TYPE_MULTI  2


typedef void (*stripFunction)(const std::string& str, size_t start, size_t end, std::string& out);


void stripWithoutWhitespace(const std::string& str, size_t start, size_t end, std::string& out) {
    // Do nothing.
    return;
}

void stripWithWhitespace(const std::string& str, size_t start, size_t end, std::string& out) {
    for (size_t i = start; i < end; i++) {
        char ch = str[i];

        if (isspace(ch)) {
            out.push_back(ch);
        } else {
            out.push_back(' ');
        }
    }
}


std::string stripComments(const std::string& str, bool whitespace) {
    std::string ret;
    ret.reserve(str.length());

    // Function to use for stripping text.
    // TODO: make into template param for faster code?
    stripFunction strip = whitespace ? stripWithWhitespace : stripWithoutWhitespace;

    char currentChar, nextChar;
    bool insideString = false;
    int commentType = COMMENT_TYPE_NONE;

    size_t offset = 0;
    for (size_t i = 0; i < str.length(); i++) {
        currentChar = str[i];

        if (i < str.length() - 1) {
            nextChar = str[i + 1];
        } else {
            nextChar = '\0';
        }

        // If we're not in a comment, check for a quote.
        if (commentType == COMMENT_TYPE_NONE && currentChar == '"') {
            bool escaped = false;

            // If the previous character was a single slash, and the one before
            // that was not (i.e. the previous character is escaping this quote
            // and is not itself escaped), then the quote is escaped.
            if (i >= 2 && str[i - 1] == '\\' && str[i - 2] != '\\') {
                escaped = true;
            }

            if (!escaped) {
                insideString = !insideString;
            }
        }

        if (insideString) {
            continue;
        }

        if (commentType == COMMENT_TYPE_NONE && currentChar == '/' && nextChar == '/') {
            // std::cerr << "[no comment] found single comment, adding slice from " << offset << " to " << i << std::endl;
            // std::cerr << "str:\n" << ret << std::endl;
            ret.append(str, offset, i - offset);
            offset = i;
            commentType = COMMENT_TYPE_SINGLE;

            // Skip second '/'
            i++;
        } else if (commentType == COMMENT_TYPE_SINGLE && currentChar == '\r' && nextChar == '\n') {
            // std::cerr << "[single comment] exiting comment (1), stripping slice from " << offset << " to " << i << std::endl;
            // std::cerr << "str:\n" << ret << std::endl;

            // Skip '\r'
            i++;

            commentType = COMMENT_TYPE_NONE;
            strip(str, offset, i, ret);
            offset = i;

            continue;
        } else if (commentType == COMMENT_TYPE_SINGLE && currentChar == '\n') {
            // std::cerr << "[single comment] exiting comment (2), stripping slice from " << offset << " to " << i << std::endl;
            // std::cerr << "str:\n" << ret << std::endl;

            commentType = COMMENT_TYPE_NONE;
            strip(str, offset, i, ret);
            offset = i;
        } else if (commentType == COMMENT_TYPE_NONE && currentChar == '/' && nextChar == '*') {
            // std::cerr << "[no comment] found multi comment, adding slice from " << offset << " to " << i << std::endl;
            // std::cerr << "str:\n" << ret << std::endl;

            ret.append(str, offset, i - offset);
            offset = i;
            commentType = COMMENT_TYPE_MULTI;

            // Skip the '*'
            i++;
            continue;
        } else if (commentType == COMMENT_TYPE_MULTI && currentChar == '*' && nextChar == '/') {
            // std::cerr << "[multi comment] exiting comment, stripping slice from " << offset << " to " << (i + 1) << std::endl;
            // std::cerr << "str:\n" << ret << std::endl;

            // Skip '*'
            i++;

            commentType = COMMENT_TYPE_NONE;
            strip(str, offset, i + 1, ret);
            offset = i + 1;
            continue;
        }
    }

    // std::cerr << "[done] adding substring from " << offset << " to end" << std::endl;
    ret.append(str, offset, str.length() - offset);
    return ret;
}
