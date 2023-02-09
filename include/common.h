#pragma once

// template command for models and formats
struct command_t {
    uint code;
    qint32 par1;
    qint32 par2;
    qint32 time;
};

// list of socket commands
enum command_type   {
    com_init,
    com_buffer,
    com_ping,
    com_package
};

// connecting of commands and model owners
struct STRCommand {
    uint code;
    uint model_owner;
    QString description;
};

