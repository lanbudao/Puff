#ifndef PUFF_AVERROR_H
#define PUFF_AVERROR_H

namespace Puff {

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
#endif //PUFF_AVERROR_H
