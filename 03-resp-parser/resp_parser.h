// RES is the protocol that redis use to communicate with the client
// *<number of elements>\r\n
// $<lenght of the 1st element>\r\n
// <content>\r\n
#ifndef RESP_PARSER_H
#define RESP_PARSER_H

int parse_resp(const char* input, char* argv[], int max_args);

#endif