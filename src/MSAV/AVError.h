#ifndef MSAV_AVERROR_H
#define MSAV_AVERROR_H

namespace MSAV {

class AVError {
public:
    AVError();
    ~AVError();

    enum ErrorCode {
        NoError,

        OpenError,
    };
};

}
#endif //MSAV_AVERROR_H
