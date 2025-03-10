//
// Created by Shaun on 7/03/2025.
//

#ifndef IMESHBUFFER_H
#define IMESHBUFFER_H

class IMeshBuffer {
public:
    virtual void bind() = 0;
    virtual void draw() = 0;
    virtual ~IMeshBuffer() = default;
};


#endif //IMESHBUFFER_H
