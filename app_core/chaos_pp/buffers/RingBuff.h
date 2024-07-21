/*
 * RingBuf.h
 *
 *  Created on: Jul 16, 2024
 *      Author: admin
 */

#ifndef CHAOSPP_RINGBUFF_H_
#define CHAOSPP_RINGBUFF_H_

#include "RingBase.h"

class RingBuff : public RingBase
{
public:
	RingBuff() = default;
	~RingBuff() override final;

    bool init(void* const buffer, const reg size);
    bool installBuffer(void* const buffer);

    /*
     * *********************************************************
     * GET
     * *********************************************************
     */
    reg get(void* const buffer, const reg size);
    u8 getc();

    template <class T> inline T get();

    /*
      * *********************************************************
      * PEEK
      * *********************************************************
      */
    reg peek(void* const buffer, const reg size);
    u8 peekc();

    template <class T> inline T peek();

    /*
      * *********************************************************
      * PUT
      * *********************************************************
      */
    reg put(void* const buffer, const reg size);
    bool putc(const u8 c);

    template <class T> inline bool put(const T value);

private:
	void* buffer = nullptr;
	bool isHeap = false;
};



template <class T>
inline T RingBuff::get()
{
	T value;
	if (get(&value, sizeof(T)) != sizeof(T)) {
		return T();
	}
	return value;
}

template <class T>
inline T RingBuff::peek()
{
	T value;
	if (peek(&value, sizeof(T)) != sizeof(T)) {
		return T();
	}
	return value;
}

template <class T>
inline bool RingBuff::put(const T value)
{
	return put(&value, sizeof(T)) == sizeof(T);
}



#endif /* CHAOSPP_RINGBUFF_H_ */
