#include "Streamer.h"

Streamer::Streamer(std::string addr, unsigned short port) {
    this->running = true;
    this->servPort = Socket::resolveService(std::to_string(port), "udp");
    try {
        this->sock = new UDPSocket(port);
    } catch (SocketException &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    this->servAddr = addr;
    jpegqual = ENCODE_QUALITY;
    this->compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    this->compression_params.push_back(jpegqual);
#ifdef DEBUG
    this->last_cycle = clock();
#endif
}

bool Streamer::sendFrame(cv::Mat frame) {
    if (!this->running)
        return false;
    cv::imencode(".jpg", frame, encoded, compression_params);

    int total_pack = 1 + (encoded.size() - 1) / PACK_SIZE;
    int ibuf[1];
    ibuf[0] = total_pack;
    // Basically send a "header" with how many packets to listen for so we can stitch the image back together on the other side
    // The viewer will do a try: pass if it misses a packet and attempts to read in the header
    // you will occasionally drop 2 frames but this was rare in my experience
    try {
        sock->sendTo(ibuf, sizeof(int), servAddr, servPort);
        for (int i = 0; i < total_pack; ++i)
            sock->sendTo(&encoded[i * PACK_SIZE], PACK_SIZE, servAddr, servPort);
    } catch (SocketException &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }
    // Uncomment the #define DEBUG if you want to print statistics about your stream to the senders stdout
#ifdef DEBUG
    clock_t nextCycle = clock();
    double dur = (nextCycle - last_cycle) / (double) CLOCKS_PER_SEC;
    stats.clear();
    stats << "\tFPS: " << (1 / dur) << " \tkbps:" << (PACK_SIZE * total_pack / dur / 1024 * 8) << "\n" << nextCycle-last_cycle;
    last_cycle = nextCycle;
#endif
    return true;
}

void Streamer::close() {
    this->sock->disconnect();
    running = false;
}

#ifdef DEBUG
std::string Streamer::getStats() {
    return this->stats.str();
}
#endif
