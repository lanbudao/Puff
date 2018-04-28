#ifndef PUFF_SIZE_H
#define PUFF_SIZE_H

#include "AVGlobal.h"

class PU_AV_EXPORT Size
{
public:
    Size(): wid(0), hei(0) {}
    Size(int w, int h) {wid = w; hei = h;}

    inline int width() const {return wid;}
    inline void setWidth(int w) {wid = w;}
    inline int height() const {return hei;}
    inline void setHeight(int h) {hei = h;}
    inline bool isNull() {return wid == 0 && hei == 0;}
    inline bool isValid() {return wid >= 0 && hei >= 0;}

    friend inline bool operator == (const Size &s1, const Size &s2) {return s1.wid == s2.wid && s1.hei == s2.hei;}
    friend inline bool operator != (const Size &s1, const Size &s2) {return s1.wid != s2.wid || s1.hei != s2.hei;}
    friend inline const Size operator + (const Size &s1, const Size &s2) {return Size(s1.wid + s2.wid, s1.hei + s2.hei);}
    friend inline const Size operator - (const Size &s1, const Size &s2) {return Size(s1.wid - s2.wid, s1.hei - s2.hei);}

private:
    int wid, hei;
};

#endif //PUFF_SIZE_H
