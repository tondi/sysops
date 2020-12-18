#ifndef MESSAGE_H
#define MESSAGE_H

//#define SERVER_KEY_PATHNAME "/tmp/mqueue_server_key"
#define PROJECT_ID 'M'
#define QUEUE_PERMISSIONS 0660

#define REGISTER 1
#define TIME 2
#define END 3
#define STOP 4

#define TO_STRING(num) ( \
    num == REGISTER ? "REGISTER" : ( \
    num == TIME ? "TIME" : ( \
    num == END ? "END" : ( \
    num == STOP ? "STOP" : "UNKNOWN" \
    ))))

#define BUFFER_SIZE 256
#define CLIENTS_MAX_NUM 10

struct message_text {
    int pid;
    char buf [BUFFER_SIZE];
};

struct message {
    long message_type;
    struct message_text message_text;
};

#endif
