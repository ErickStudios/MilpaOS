void init_functions(server_t* serv) {
    serv->createRequest = &request_msg;
    serv->createThread = &proalloc;
    serv->fetchRequest = &getparam_msg;
    serv->findFirstRequest = &findfirst_msg;
    serv->inputCon = &getc;
    serv->joinRequest = &join_msg;
    //serv->joinThread = join_thread;
    serv->printCon = &vprintk;
    serv->respondRequest = &respond_msg;
    serv->teletypeCon = &vputc;

    serv->malloc = &malloc;
    serv->free = &free;
    serv->openFile = &fs_get_file;
    serv->readFile = &fs_read_file;

    globKserv = serv;
}